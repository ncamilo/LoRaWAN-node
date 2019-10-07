# LoRaWAN-node
Source code for LoRaWAN node with ESP32.

Para criar um acesso de um nó na rede TTN (The Things Network) proceda da seguinte forma:

1.	Acesse o site: https://www.thethingsnetwork.org/
 
2.	Clique em Sign Up, crie uma conta e faça a ativação da conta no e-mail indicado.
3.	Faça o Login e clique em Console:
 
4.	Na tela do Console clique em Applications
 
5.	Clique em add application
 



6.	Preencha os campos
a.	Application ID: nome da aplicação.
b.	Description: descrição da sua aplicação.
c.	Application EUI será adicionado pelo servidor TTN.
d.	Handler registration: ttn-handler-brazil
e.	Clique em Add aplication
 













7.	Na tela Application overview clique em register device:
 







8.	Preencha os campos:
a.	Device ID: identificação do nó.
b.	Em Device EUI clique no ícone   para gerar uma chave automaticamente.
c.	Clique em Register device.
 
9.	Existem dois métodos de ativação:
a.	OTAA: Over-the-air Activation (Ativação pelo Ar) que é a forma mais segura de se conectar à rede TTN onde o dispositivo faz um procedimento de Join na rede usando um DevAddr (endereço do dispositivo) dinâmico que é associado pela própria rede e as chaves de segurança que também são negociadas pela rede TTN.
b.	ABP: em casos devemos codificar o DevAddr e as chaves de segurança no dispositivo, isto é a Activation by Personalization (Ativação por Personalização), este tipo de ativação é mais simples pois não executa o Join e facilita o envio dos dados, sendo assim, vamos utilizar este método de ativação.
c.	Para fazer esta configuração na tela Device overview clique em Settings:
 

















10.	Já na tela Device settings edite os seguintes itens:
a.	Descrição.
b.	Clique em ABP.
c.	Desmarque a opção Frame Counter Checks.
d.	Clique em Save.
 
11.	Chegaremos na tela Device Overview com o Activation method ABP e todas as chaves necessárias para programar nosso nó sensor.
 











12.	Na tela Application overview clique em Payloads formats em seguida selecione Cayenne LPP e clique em Save.
 















Agora que já temos nossa aplicação criada e nosso nó sensor criado podemos passar a parte da programação do nó, para isto foi utilizado um WiFi LoRa 32 versão V2 como microcontrolador, um sensor DHT11 e um módulo GPS para localização como podemos ver no diagrama abaixo:
 
Para fazer a programação iremos precisar da IDE Arduino instalada, as bibliotecas do ESP32 e as bibliotecas da Heltec LoRa, podemos ver links para estas instalações abaixo:
•	Acesse o endereço https://www.arduino.cc/en/Main/Software para instalar a IDE do Arduino;
•	Para instalar as bibliotecas do ESP32 na IDE do Arduino veja o seguinte tutorial: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/;
•	As bibliotecas da placa WiFi LoRa 32 da Heltec podem ser instaladas seguindo o seguinte tutorial: https://docs.heltec.cn/#/en/user_manual/how_to_install_esp32_Arduino;
Com estes softwares todos instalados vamos instalar as bibliotecas necessárias para o nó LoRaWAN:
1.	Acesse o repositório do projeto em: https://github.com/ncamilo/LoRaWAN-node e faça o download do projeto:
 
2.	Descompacte os arquivos em uma pasta e copie o conteúdo da pasta arduino-lmic-master para a pasta Documentos/Arduino/libraries do seu computador:
 
3.	Abra o arquivo EndPoint_915MHz_ok.ino e vamos inserir as chaves da rede TTN para configuração do nó. Na página Device overview clicamos nos ícones:
a.	  para alterar para o modo C-style de todas as chaves; 
b.	  somente em Application EUI clica-se neste ícone para alterar para o modo LSB; 
c.	  ajuda a copiar a chave para ser colada no programa do nó:

As chaves são mostradas como podemos ver na figura a seguir:
 

















4.	Podemos ver as chaves copiadas na próxima figura:
 
















5.	Clique no menu Ferramentas/Placa/WiFi LoRa 32(V2) para selecionar o ESP32 LoRa da Heltec, selecione a porta de comunicação correta e depois clique em carregar para fazer o upload do programa para o microcontrolador ESP32, veja o exemplo:
 














6.	Finalmente na tela Application data podemos ver o nó trabalhando e enviando os dados para nossa aplicação:

 















