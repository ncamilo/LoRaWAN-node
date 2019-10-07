# LoRaWAN-node
Source code for LoRaWAN node with ESP32.

Para criar um acesso de um nó na rede TTN (The Things Network) proceda da seguinte forma:

1.	Acesse o site: https://www.thethingsnetwork.org/
![TTN - login](https://user-images.githubusercontent.com/8071543/66339735-1e670400-e91a-11e9-8320-912501b7b63f.png)

2.	Clique em Sign Up, crie uma conta e faça a ativação da conta no e-mail indicado.
3.	Faça o Login e clique em Console:<BR>
![TTN - Console](https://user-images.githubusercontent.com/8071543/66339772-363e8800-e91a-11e9-9e26-e0beb767aafe.png)<BR>
4.	Na tela do Console clique em Applications<BR>
![TTN - Add application](https://user-images.githubusercontent.com/8071543/66340029-c086ec00-e91a-11e9-9822-ecc2ddd8d197.png)<BR>
5.	Clique em add application<BR>
![TTN - Add application button](https://user-images.githubusercontent.com/8071543/66340071-d4cae900-e91a-11e9-966d-488ee0917d2d.png)<BR>
6.	Preencha os campos<BR>
a.	Application ID: nome da aplicação.<BR>
b.	Description: descrição da sua aplicação.<BR>
c.	Application EUI será adicionado pelo servidor TTN.<BR>
d.	Handler registration: ttn-handler-brazil.<BR>
e.	Clique em Add aplication.<BR>
![TTN - Add application configuration](https://user-images.githubusercontent.com/8071543/66340387-6f2b2c80-e91b-11e9-9384-60424d10a50e.png)<BR>
7.	Na tela Application overview clique em register device:
![TTN - Add application overview](https://user-images.githubusercontent.com/8071543/66340505-adc0e700-e91b-11e9-97b8-9ec57d50d7a7.png)<BR>
8.	Preencha os campos:<BR>
a.	Device ID: identificação do nó.<BR>
b.	Em Device EUI clique no ícone   para gerar uma chave automaticamente.<BR>
c.	Clique em Register.<BR>
![TTN - Add application Register device](https://user-images.githubusercontent.com/8071543/66341132-02b12d00-e91d-11e9-83ec-a14826fc4976.png)<BR>
9.	Existem dois métodos de ativação:<BR>
a.	OTAA: Over-the-air Activation (Ativação pelo Ar) que é a forma mais segura de se conectar à rede TTN onde o dispositivo faz um procedimento de Join na rede usando um DevAddr (endereço do dispositivo) dinâmico que é associado pela própria rede e as chaves de segurança que também são negociadas pela rede TTN.<BR>
b.	ABP: em casos devemos codificar o DevAddr e as chaves de segurança no dispositivo, isto é a Activation by Personalization (Ativação por Personalização), este tipo de ativação é mais simples pois não executa o Join e facilita o envio dos dados, sendo assim, vamos utilizar este método de ativação.<BR>
c.	Para fazer esta configuração na tela Device overview clique em Settings:<BR><BR>
![TTN - Add application Device Overview](https://user-images.githubusercontent.com/8071543/66341189-1bb9de00-e91d-11e9-9461-870583575c13.png)<BR>
10.	Já na tela Device settings edite os seguintes itens:<BR>
a.	Descrição.<BR>
b.	Clique em ABP.<BR>
c.	Desmarque a opção Frame Counter Checks.<BR>
d.	Clique em Save.<BR>
![Device settings](https://user-images.githubusercontent.com/8071543/66341688-304aa600-e91e-11e9-82e7-b20066a2aaba.png)<BR>
![TTN - Add application Device settings](https://user-images.githubusercontent.com/8071543/66341301-558ae480-e91d-11e9-8e26-65b35fec83ba.png)<BR>
11.	Chegaremos na tela Device Overview com o Activation method ABP e todas as chaves necessárias para programar nosso nó sensor.<BR>
![Device overview ABP](https://user-images.githubusercontent.com/8071543/66341951-a6e7a380-e91e-11e9-8abd-2dd2b5c62885.png)<BR>
	12.	Na tela Application overview clique em Payloads formats em seguida selecione Cayenne LPP e clique em Save.<BR>
 















Agora que já temos nossa aplicação criada e nosso nó sensor criado podemos passar a parte da programação do nó, para isto foi utilizado um WiFi LoRa 32 versão V2 como microcontrolador, um sensor DHT11 e um módulo GPS para localização como podemos ver no diagrama abaixo:<BR>
 
Para fazer a programação iremos precisar da IDE Arduino instalada, as bibliotecas do ESP32 e as bibliotecas da Heltec LoRa, podemos ver links para estas instalações abaixo:<BR>
•	Acesse o endereço https://www.arduino.cc/en/Main/Software para instalar a IDE do Arduino;<BR>
•	Para instalar as bibliotecas do ESP32 na IDE do Arduino veja o seguinte tutorial: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/;<BR>
•	As bibliotecas da placa WiFi LoRa 32 da Heltec podem ser instaladas seguindo o seguinte tutorial: https://docs.heltec.cn/#/en/user_manual/how_to_install_esp32_Arduino;<BR>
Com estes softwares todos instalados vamos instalar as bibliotecas necessárias para o nó LoRaWAN:<BR>
1.	Acesse o repositório do projeto em: https://github.com/ncamilo/LoRaWAN-node e faça o download do projeto:<BR>
 
2.	Descompacte os arquivos em uma pasta e copie o conteúdo da pasta arduino-lmic-master para a pasta Documentos/Arduino/libraries do seu computador:<BR>
 
3.	Abra o arquivo EndPoint_915MHz_ok.ino e vamos inserir as chaves da rede TTN para configuração do nó. Na página Device overview clicamos nos ícones:<BR>
a.	  para alterar para o modo C-style de todas as chaves; <BR>
b.	  somente em Application EUI clica-se neste ícone para alterar para o modo LSB; <BR>
c.	  ajuda a copiar a chave para ser colada no programa do nó:<BR>

As chaves são mostradas como podemos ver na figura a seguir:<BR>
 

















4.	Podemos ver as chaves copiadas na próxima figura:<BR>
 
















5.	Clique no menu Ferramentas/Placa/WiFi LoRa 32(V2) para selecionar o ESP32 LoRa da Heltec, selecione a porta de comunicação correta e depois clique em carregar para fazer o upload do programa para o microcontrolador ESP32, veja o exemplo:<BR>
 














6.	Finalmente na tela Application data podemos ver o nó trabalhando e enviando os dados para nossa aplicação:<BR>

 















