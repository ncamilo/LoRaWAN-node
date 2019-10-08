# LoRaWAN-node
<BR />
<BR />
Para criar um acesso de um nó na rede TTN (The Things Network) proceda da seguinte forma:<BR />
<BR />
1.	Acesse o site: https://www.thethingsnetwork.org/<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66339735-1e670400-e91a-11e9-8320-912501b7b63f.png" width="800"><BR />
2.	Clique em <i>Sign Up</i>, crie uma conta e faça a ativação da conta no e-mail indicado.
3.	Faça o <i>Login</i> e clique em <i>Console</i>:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66339772-363e8800-e91a-11e9-9e26-e0beb767aafe.png" width="800"><BR>
4.	Na tela do Console clique em <i>Applications</i>:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66340029-c086ec00-e91a-11e9-9822-ecc2ddd8d197.png" width="800"><BR>
5.	Clique em <i>add application</i>:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66340071-d4cae900-e91a-11e9-966d-488ee0917d2d.png" width="800"><BR>
6.	Preencha os campos:<BR />
a.	<i>Application ID</i>: nome da aplicação.<BR />
b.	<i>Description</i>: descrição da sua aplicação.<BR />
c.	<i>Application EUI</i> será adicionado pelo servidor TTN.<BR />
d.	<i>Handler registration</i>: ttn-handler-brazil.<BR />
e.	Clique em <i>Add aplication</i>.<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66340387-6f2b2c80-e91b-11e9-9384-60424d10a50e.png" width="800"><BR />
7.	Na tela <i>Application overview</i> clique em <i>register device</i>:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66340505-adc0e700-e91b-11e9-97b8-9ec57d50d7a7.png" width="800"><BR />
8.	Preencha os campos:<BR />
a.	<i>Device ID</i>: identificação do nó.<BR />
<div align="left">b.	Em <i>Device EUI</i> clique no ícone <img src="https://user-images.githubusercontent.com/8071543/66399994-00031600-e9b7-11e9-98f8-ae4f069c071d.png" width="40px" height="40px"> para gerar uma chave automaticamente.</div><BR />
c.	Clique em <i>Register</i>.<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66341132-02b12d00-e91d-11e9-83ec-a14826fc4976.png" width="800"><BR>
9.	Existem dois métodos de ativação:<BR />
a.	OTAA: <i>Over-the-air Activation</i> (Ativação pelo Ar) que é a forma mais segura de se conectar à rede TTN onde o dispositivo faz um procedimento de <i>Join</i> na rede usando um <i>DevAddr</i> (endereço do dispositivo) dinâmico que é associado pela própria rede e as chaves de segurança que também são negociadas pela rede TTN.<BR />
b.	ABP: em casos devemos codificar o <i>DevAddr</i> e as chaves de segurança no dispositivo, isto é a <i>Activation by Personalization</i> (Ativação por Personalização), este tipo de ativação é mais simples pois não executa o <i>Join</i> e facilita o envio dos dados, sendo assim, vamos utilizar este método de ativação.<BR />
c.	Para fazer esta configuração na tela <i>Device overview</i> clique em <i>Settings</i>:<BR /><BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66341189-1bb9de00-e91d-11e9-9461-870583575c13.png" width="800"><BR />
10.	Já na tela <i>Device settings</i> edite os seguintes itens:<BR />
a.	Descrição.<BR />
b.	Clique em ABP.<BR />
c.	Desmarque a opção <i>Frame Counter Checks</i>.<BR />
d.	Clique em <i>Save</i>.<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66341688-304aa600-e91e-11e9-82e7-b20066a2aaba.png" width="800"><BR /><BR /><BR />
11.	Chegaremos na tela <i>Device Overview</i> com o <i>Activation method</i> ABP e todas as chaves necessárias para programar nosso nó sensor.<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66341951-a6e7a380-e91e-11e9-8abd-2dd2b5c62885.png" width="800"><BR />
12.	Na tela <i>Application overview</i> clique em <i>Payloads formats</i> em seguida selecione <i>Cayenne LPP</i> e clique em <i>Save</i>.<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66342695-6ab54280-e920-11e9-8333-4a36c8f97c94.png" width="800"><BR />
<BR><BR>
Agora que já temos nossa aplicação criada e nosso nó sensor criado podemos passar a parte da programação do nó, para isto foi utilizado um WiFi LoRa 32 versão V2 como microcontrolador, um sensor DHT11 e um módulo GPS para localização como podemos ver no diagrama abaixo:<BR><BR />
<img src="https://user-images.githubusercontent.com/8071543/66342937-edd69880-e920-11e9-8170-a525ad97945b.png" width="800"><BR />
Para fazer a programação iremos precisar da IDE Arduino instalada, as bibliotecas do ESP32 e as bibliotecas da Heltec LoRa, podemos ver links para estas instalações abaixo:<BR />
•	Acesse o endereço https://www.arduino.cc/en/Main/Software para instalar a IDE do Arduino;<BR />
•	Para instalar as bibliotecas do ESP32 na IDE do Arduino veja o seguinte tutorial: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/;<BR />
•	As bibliotecas da placa WiFi LoRa 32 da Heltec podem ser instaladas seguindo o seguinte tutorial: https://docs.heltec.cn/#/en/user_manual/how_to_install_esp32_Arduino;<BR /><BR /><BR />
Com estes softwares todos instalados vamos instalar as bibliotecas necessárias para o nó LoRaWAN:<BR />
1.	Faça o download deste projeto:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66392724-11442680-e9a7-11e9-9bf5-18eb476e4db2.png" width="800"><BR />
2.	Descompacte os arquivos em uma pasta e copie o conteúdo da pasta <i>arduino-lmic-master</i> para a pasta <i>Documentos/Arduino/libraries</i> do seu computador:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66392799-45b7e280-e9a7-11e9-868b-af46bc73634e.png" width="800"><BR />
3.	Abra o arquivo <i>EndPoint_915MHz_ok.ino</i> e vamos inserir as chaves da rede TTN para configuração do nó. Na página <i>Device overview</i> clicamos nos ícones:<BR /><BR />
<div align="left">a.	<img src="https://user-images.githubusercontent.com/8071543/66393325-9bd95580-e9a8-11e9-9aca-1914b4d6362e.png" width="40">  para alterar para o modo <i>C-style</i> de todas as chaves;</div><BR />
<div align="left">b.	<img src="https://user-images.githubusercontent.com/8071543/66393367-bf9c9b80-e9a8-11e9-93f5-aa45eb0af34b.png" width="40">  somente em <i>Application EUI</i> clica-se neste ícone para alterar para o modo <i>LSB</i>;</div><BR />
<div align="left">c.	<img src="https://user-images.githubusercontent.com/8071543/66393413-db07a680-e9a8-11e9-9243-0e689925b82d.png" width="40">  ajuda a copiar a chave para ser inserida no programa do nó.</div><BR /><BR />
As chaves são mostradas como podemos ver na figura a seguir:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66393549-39348980-e9a9-11e9-84dd-439004d23e0e.png" width="800"><BR />
4.	Podemos ver as chaves copiadas na próxima figura:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66393618-67b26480-e9a9-11e9-8434-0bf93586e35d.png" width="800"><BR />
5.	Clique no menu <i>Ferramentas/Placa/WiFi LoRa 32(V2)</i> para selecionar o ESP32 LoRa da Heltec, selecione a porta de comunicação correta e depois clique em carregar para fazer o <i>upload do programa para o microcontrolador ESP32, veja o exemplo:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66393692-9597a900-e9a9-11e9-94cd-2af881e112a5.png" width="800"><BR />
6.	Finalmente na tela <i>Application data</i> podemos ver o nó trabalhando e enviando os dados para nossa aplicação:<BR /><BR />
<img src="https://user-images.githubusercontent.com/8071543/66393933-2a020b80-e9aa-11e9-90f5-771c8ef36ccb.png" width="800"><BR /><BR /><BR />
