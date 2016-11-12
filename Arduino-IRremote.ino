#define WEBDUINO_AUTH_REALM "Weduino Authentication Example"
#include <SPI.h>
#include <Ethernet2.h>
#include <WebServer.h>
#include <IRremote.h>

IRsend irsend;

static uint8_t mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x31, 0x7B };  //MACアドレス
static uint8_t ip[] = { 192, 168, 11, 27 };  //IPアドレス(グローバルIPアドレスでもOK)

#define PREFIX ""
WebServer webserver(PREFIX, 80);  //80番ポートを使用


void setup() {
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&defaultCmd);
  webserver.addCommand("index.html", &defaultCmd);
  webserver.addCommand("mode1.html", &sendCmd1);
  webserver.addCommand("mode2.html", &sendCmd2);
  webserver.begin();
}

//LEDを光らせる周波数
int khz = 38;
//ライト電源ON
unsigned int irSignal_1[] = {3500,1700, 450,400, 450,400, 450,1300, 450,1250, 450,400, 500,1200, 500,400, 450,400, 500,400, 450,1250, 450,450, 450,350, 500,1250, 450,400, 500,1250, 450,400, 450,1300, 450,400, 450,400, 500,1250, 450,400, 450,400, 500,400, 450,400, 450,400, 450,400, 500,1250, 450,1300, 450,400, 450,1250, 450,400, 500,400, 450,1250, 450,400, 500,1250, 450,400, 450,400, 500,1250, 450,400, 500,350, 500}; //AnalysIR Batch Export (IRremote) - RAW
//ライト電源OFF
unsigned int irSignal_2[] = {3500,1700, 450,450, 450,400, 450,1250, 450,1300, 450,400, 450,1300, 450,400, 450,400, 450,400, 500,1200, 500,400, 450,400, 500,1250, 450,400, 450,1300, 450,400, 450,1300, 450,400, 450,400, 450,1300, 450,400, 450,400, 450,400, 450,450, 450,1250, 450,1250, 500,1250, 450,1300, 450,400, 450,1250, 500,400, 450,400, 450,400, 450,1300, 450,1250, 450,400, 500,400, 450,1250, 450,400, 500,400, 450};



void loop() {
  char buff[64];
  int len = 64;
  webserver.processConnection(buff, &len);
}


void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool) {

  if (server.checkCredentials("a2VudGFybzp0ZXN0")) {   //ログインチェック(Base64でエンコード)　ユーザー名:kentaro パスワード:test

    server.httpSuccess();

    if (type != WebServer::HEAD) {

      P(htmlfile) = "<html>"
                        "<head>"
                            "<meta charset='UTF-8'>"
                            "<title>Arduino IR Remote</title>"
                            "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                            "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'>"
                            "<style> html { margin-top: 10px; margin-left: 10px; margin-right: 10px; }</style>"
                        "</head>"
                        "<body>"
                            "<a class='btn btn-primary btn-block btn-lg' href='mode1.html'>ライトON</a>"
                            "<a class='btn btn-danger btn-block btn-lg' href='mode2.html'>ライトOFF</a>"
                        "</body>"
                    "</html>";
      server.printP(htmlfile);
    }
  }

  else {
    server.httpUnauthorized();
  }

}

void sendCmd1(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  if (type == WebServer::GET) {
       server.httpSuccess();
       irsend.sendRaw(irSignal_1, sizeof(irSignal_1) / sizeof(irSignal_1[0]), khz);
       delay(40);
       Serial.println(" ON");
       P(redirect) = "<html>"
                        "<head>"
                            "<meta charset='UTF-8'>"
                            "<title>Arduino IR Remote</title>"
                            "<meta http-equiv='refresh' content='0; URL=../index.html' />"
                        "</head>"
                        "<body>"
                            "<h1>sending・・・</h1>"
                        "</body>"
                    "</html>";
      server.printP(redirect);
  }
}

void sendCmd2(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  if (type == WebServer::GET) {
       server.httpSuccess();
       irsend.sendRaw(irSignal_2, sizeof(irSignal_2) / sizeof(irSignal_2[0]), khz);
       delay(40);
       Serial.println(" OFF");
       P(redirect) = "<html>"
                        "<head>"
                            "<meta charset='UTF-8'>"
                            "<title>Arduino IR Remote</title>"
                            "<meta http-equiv='refresh' content='0; URL=../index.html' />"
                        "</head>"
                        "<body>"
                            "<h1>sending・・・</h1>"
                        "</body>"
                    "</html>";
      server.printP(redirect);
  }
}
