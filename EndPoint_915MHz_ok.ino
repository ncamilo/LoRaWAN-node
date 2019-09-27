// MIT License
// https://github.com/gonzalocasas/arduino-uno-dragino-lorawan/blob/master/LICENSE
// Based on examples from https://github.com/matthijskooijman/arduino-lmic
// Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman

#include <Arduino.h>
// Tentativa de ligação do GPS
#include <dummy.h>  // silence warnings from Arduino IDE
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include "DHTesp.h"
//#include <TheThingsNetwork.h>
#include <CayenneLPP.h>

#include "lmic.h"
#include <hal/hal.h>
#include <SPI.h>
#include <SSD1306.h>

#define LEDPIN 25

#define OLED_I2C_ADDR 0x3C
#define OLED_RESET    16
#define OLED_SDA      4
#define OLED_SCL      15

// show debug statements; comment next line to disable debug statements
//#define DEBUG

// Definição de ABP ou OTA
#define ABP
//#define OTA

// Config freqPlan with TTN_FP_EU868 or TTN_FP_US915 or TTN_FP_AU915
#define freqPlan TTN_FP_US915
  
// DHTEsp instance
DHTesp dht;
TempAndHumidity newValues;

// Pin number for DHT11 data pin
int dhtPin = 13;

// CayenneLpp instance
CayenneLPP lpp(51);
DynamicJsonDocument jsonBuffer(512);

// General variables
char device[8];
unsigned int counter = 0;
char TTN_response[30];
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 30;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,
    //.dio = {26, 33, 32}  // Pins for the Heltec ESP32 Lora board / TTGO Lora32 with 3D metal antenna V1
    .dio = {26, 34, 35}    // Pins for the Heltec ESP32 Lora board / TTGO Lora32 with 3D metal antenna V2
};

// GPS Section
// The TinyGPS++ object
TinyGPSPlus gps;
typedef union {
  float f[2];               // Assigning fVal.f will also populate fVal.bytes;
  unsigned char bytes[8];   // Both fVal.f and fVal.bytes share the same 4 bytes of memory.
} floatArr2Val;
floatArr2Val latlong;
float latitude;
float longitude;
float _altitude;
char s[16];                 // used to sprintf for OLED display
#define GPS_RX 22
#define GPS_TX 23
HardwareSerial GPSSerial(1);

// Display LCD instance
SSD1306 display (OLED_I2C_ADDR, OLED_SDA, OLED_SCL);

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse the bytes.
// This should also be in little endian format, see above.
// For TTN issued EUIs the last bytes should be 0xD5, 0xB3, 0x70.
// Chose LSB mode on the console and then copy it here.
static const u1_t PROGMEM APPEUI[8]={ /* TTN Aplication EUI */ }; 

// LoRaWAN NwkSKey, your network session key, 16 bytes
static const PROGMEM u1_t NWKSKEY[16] = { /* TTN Network Section Key */ };

// Config Device End User Identifier  
static const u1_t PROGMEM DEVEUI[8]={ /* TTN Device EUI */ };

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
// Choose MSB mode
static const u1_t PROGMEM APPKEY[16] = { /* TTN same as Network Section Key */ };

// Config device address
static const u4_t DEVADDR = 0x /* TTN Device Address */ ; // <-- Change this address for every node!

// LoRaWAN AppSKey, application session key, 16 bytes
static const u1_t PROGMEM APPSKEY[16] = { /* TTN App Section Key */ };

// Definitions OTA or ABP
#ifdef OTA
// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
#endif

#ifdef ABP
// Configure ABP
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8 );}
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8 );}
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}
#endif

void do_send(osjob_t* j){

    #ifdef DEBUG
      Serial.println(F("Enter do_send"));
    #endif

    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Get coordenates
        get_coords();

        // Payload to send (uplink)
        lpp.reset();
        lpp.addTemperature(1, newValues.temperature);
        lpp.addRelativeHumidity(2, newValues.humidity);
        lpp.addGPS(3, latitude, longitude, _altitude);
        lpp.addDigitalOutput(4, 1);    // channel 4, set digital output high
        
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, lpp.getBuffer(), lpp.getSize(), 0);

        #ifdef DEBUG
          Serial.print(F("Packet queued: "));
          Serial.println(LMIC.freq);
          Serial.print(F("Sending uplink packet: "));
          Serial.println(counter);
        #endif
        
        digitalWrite(LEDPIN, HIGH);
        display.clear();
        display.drawString (0, 0, "Sending uplink packet...");
        display.drawString (0, 50, String (++counter));
        display.drawString (0, 25, device);
        display.display ();
    }
    // Next TX is scheduled after TX_COMPLETE event.
    #ifdef DEBUG
      Serial.println(F("Leave do_send"));
    #endif
}

void get_coords () {
  while (GPSSerial.available())
    gps.encode(GPSSerial.read());
  latitude  = gps.location.lat();
  longitude = gps.location.lng();
  _altitude = gps.altitude.meters();
  
  // Only update if location is valid and has changed
  if ((latitude && longitude) && latitude != latlong.f[0]
      && longitude != latlong.f[1]) {
    latlong.f[0] = latitude;
    latlong.f[1] = longitude;
    for (int i = 0; i < 8; i++)
      Serial.print(latlong.bytes[i], HEX);
    Serial.println();
  }
}

void onEvent (ev_t ev) {

  getTemperature();
  Serial.print(os_getTime());
  Serial.print(": ");
  switch(ev) {
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      display.clear();
      display.drawString (0, 0, "EV_TXCOMPLETE event!");
      display.drawString (0, 25, device);
      display.display ();

      if( ( LMIC.txrxFlags & ( TXRX_DNW1 | TXRX_DNW2 ) ) != 0 ) {
        Serial.println( "Received downlink: ");
        //if( LMIC.dataLen != 0 )
        if( LMIC.dataLen ) { 
          // data received in rx slot after tx
          Serial.write( LMIC.frame + LMIC.dataBeg, LMIC.dataLen );
          Serial.println();
          Serial.print(F("Received "));
          Serial.print(LMIC.dataLen);
          Serial.print(F(" bytes of payload: 0x"));
          for (int i = 0; i < LMIC.dataLen; i++) {
            if (LMIC.frame[LMIC.dataBeg + i] < 10) {
                Serial.print(F("0"));
                Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);
            }
          }  
        }
        Serial.println();
      }

      if (LMIC.txrxFlags & TXRX_ACK) {
        Serial.println(F("Received ack"));
        display.drawString (0, 20, "Received ACK.");
        display.display ();
      }
  
      if (LMIC.dataLen) {
        int i = 0;
        // data received in rx slot after tx
        Serial.print(F("Data Received: "));
        Serial.write(LMIC.frame + LMIC.dataBeg, LMIC.dataLen);
        Serial.println();
        Serial.println(LMIC.rssi);
  
        display.drawString (0, 9, "Received DATA.");
        for ( i = 0 ; i < LMIC.dataLen ; i++ )
          TTN_response[i] = LMIC.frame[LMIC.dataBeg+i];
        TTN_response[i] = 0;
      }
      
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
      digitalWrite(LEDPIN, LOW);
      display.drawString (0, 50, String (counter - 1));
      display.display ();
      // Schedule next transmission
      //os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING: -> Joining..."));
      display.drawString(0, 16, "OTAA joining....");
      display.display();
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      display.clear();
      display.drawString(0, 0, "Joined!");
      display.display();
      // Disable link check validation (automatically enabled
      // during join, but not supported by TTN at this time).
      LMIC_setLinkCheckMode(0);
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
    default:
      #ifdef DEBUG
        Serial.println(F("Unknown event"));
      #endif
      break;
  }
  #ifdef DEBUG
    Serial.println(F("Leave onEvent"));
  #endif
}

bool getTemperature() {
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return false;
  }
  #ifdef DEBUG
    Serial.println(" T:" + String(newValues.temperature) + " H:" + String(newValues.humidity));
  #endif
  
  return true;
}

void setup() {
    Serial.begin(115200);
                         
    Serial.println(F("Starting..."));
    delay(500);  // Give time to the serial monitor to pick up

    // Prepara o DEVADDR para apresentação no display
    sprintf (device, "0x%08x", DEVADDR);

    // Initialize temperature sensor
    dht.setup(dhtPin, DHTesp::DHT11);
    Serial.println("DHT initiated");
    
    // Use the Blue pin to signal transmission.
    pinMode(LEDPIN, OUTPUT);

    //Set up and reset the OLED
    pinMode(OLED_RESET, OUTPUT);
    digitalWrite(OLED_RESET, LOW);
    delay(50);
    digitalWrite(OLED_RESET, HIGH);

    GPSSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
    GPSSerial.setTimeout(2);

    display.init ();
    //display.flipScreenVertically();
    display.setFont (ArialMT_Plain_10);
    display.setTextAlignment (TEXT_ALIGN_LEFT);
    display.drawString (0, 0, "Starting....");
    display.display ();

    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
      // On AVR, these values are stored in flash and only copied to RAM
      // once. Copy them to a temporary buffer here, LMIC_setSession will
      // copy them into a buffer of its own again.
      uint8_t appskey[sizeof(APPSKEY)];
      uint8_t nwkskey[sizeof(NWKSKEY)];
      memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
      memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
      LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
      // If not running an AVR with PROGMEM, just use the arrays directly
      LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    #if defined(CFG_eu868) // EU channel setup
      // Set up the channel used by the Things Network and compatible with
      // our gateway.
      // Setting up channels should happen after LMIC_setSession, as that
      // configures the minimal channel set.
      LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    
    #elif defined(CFG_us915) // US channel setup
      // Instead of using selectSubBand, which will cycle through a sub-band of 8
      // channels. We'll configure the device to only use one frequency.
      // First disable all sub-bands
      for (int b = 0; b < 8; ++b) {
        LMIC_disableSubBand(b);
      }      
    #elif defined(CFG_au915)
      #ifdef DEBUG
        Serial.println(F("Loading AU915/AU921 Configuration..."));
      #endif
      
      // Set to AU915 sub-band 2
      LMIC_selectSubBand(1); 

      // First, disable channels 0-7
      for (int channel = 0; channel < 8; ++channel) {
        LMIC_disableChannel(channel);
      }
      // Now, disable channels 16-72 (is there 72 ??)
      for (int channel = 16; channel < 72; ++channel) {
         LMIC_disableChannel(channel);
      }
    #endif

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Start job
    do_send(&sendjob);     // Will fire up also the join
    LMIC_startJoining();
}

void loop() {
    os_runloop_once();
}
