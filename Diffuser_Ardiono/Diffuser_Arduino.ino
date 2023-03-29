#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String ssid = "park_th";
String pass = "19991021";

ESP8266WebServer server(80);

String strEndDeviceIP;
IPAddress endDeviceIP;
String strEndDeviceAPIP;
IPAddress endDeviceAPIP;

String encryptionString = "";
String dBm = "";

void connectNetwork()
{
  Serial.println(ssid);
  Serial.println(pass);

  /* 메인 웹 */
  String web = "";
  web += "<html>";
  web += "<head><title>Search Network</title><meta charset=\"utf-8\"></head>";
  web += "<body>";
  web += "<h1 align = \"center\">네트워크 검색</h1>";
  web += "<table border=\"1\" bordercolor=\"black\" width=\"500\" heigh=\"300\" align=\"center\">";
  web += "<tr align=\"center\"><p><td colspan=\"5\">네트워크 목록</td></p>";
  web += "<tr align=\"center\"><td>번호</td><td>이름</td><td>신호</td><td>보안</td></tr>";
  web += printList();
  web += "</table><br>";
  web += "<p align=\"center\"><button onClick=\"window.location.reload()\">검색</button></p>";
  web += "<br><br>";
  web += "<form>";
  web += "<h2 align=\"center\">네트워크 연결</h2>";
  web += "<form method=\"POST\" action=\"/\">";
  web += "<p align=\"center\"><label for=\"id\">이름&nbsp;&nbsp;</label><input type=\"text\" id=\"id\" name=\"id\"></p>";
  web += "<p align=\"center\"><label for=\"pw\">암호&nbsp;&nbsp;</label><input type=\"password\" id=\"pw\" name=\"pw\"></p>";
  web += "<p align=\"center\"><input type=\"submit\" value=\"연결\"></p>";
  web += "</form>";
  web += "</body>";
  web += "</html>";

  server.send(200, "text/html", web);
}

String printList() {
  /* 스캔된 와이파이의 수를 저장하는 변수 */
  byte numNetworks = WiFi.scanNetworks();

  /* 스캔된 와이파이들을 웹 상에서 출력하기 위한 반복문 */
  String web = "";
  for(int i=0; i<numNetworks; i++)
  {
    web += "<tr align=\"center\"><td>(";
    web += i+1;
    web += ")</td><td>";
    /* 와이파이 이름 저장 */
    web += WiFi.SSID(i);  
    web += "</td><td>";
    /* 와이파이 신호, 강도에 따라 이름 부여 */
    if(WiFi.RSSI(i) > -55){ 
      dBm = "매우 강함";
    } else if (WiFi.RSSI(i) > -60) {
      dBm = "강함";
    } else if (WiFi.RSSI(i) > -70) {
      dBm = "양호";
    } else if (WiFi.RSSI(i) > -80) {
      dBm = "약함";
    } else {
      dBm = "매우 약함";
    }
    web += dBm;
    web += "</td><td>";
    /* 와이파이의 암호화 종류 표시, 종류에 따라 이름 부여 */
    switch(WiFi.encryptionType(i)){
      case 2:
        encryptionString = "TKIP(WPA)";
        break;
      case 4:
        encryptionString = "CCMP(WPA)";
        break;
      case 5:
        encryptionString = "WEP";
        break;
      case 7:
        encryptionString = "NONE";
        break;
      case 8:
        encryptionString = "AUTO";
        break;
      /* 모르는 암호화 종류일 때 WTF 출력 */
      default:
        encryptionString = "WTF";
        break;
    }
    web += encryptionString;
    web += "</td>";
  }

  return web;
}

void resultNetwork(){
  if(server.arg(0) != NULL){
    /* 와이파이 연결을 위해 입력된 와이파이 이름과 암호를 변수를 넘겨받아 각 변수에 저장 */
    ssid = server.arg("id");
    pass = server.arg("pw");

    Serial.println("Received SSID: " + ssid);
    Serial.println("Received PASS: " + pass);

    WiFi.begin(ssid, pass);

    /* 와이파이 상태를 측정하여 연결이 안되는 경우 계속 점 표시 */
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }

    /* 와이파이 연결이 성공한 경우 진행 */
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());

    /* 연결이 됐음을 알려주는 페이지 */
    String suc = "";
    suc += "<html>";
    suc += "<head><title>Connect Network</title><meta charset=\"utf-8\"></head>";
    suc += "<body>";
    suc += "<h1 align=\"center\">네트워크 ";
    suc += ssid;
    suc += " 연결 성공</h1><br>";
    suc += "<p align=\"center\"><button onClick=\"widnow.location.href=\"/\"\">메인</button></p><br>";
    suc += "</body>";
    suc += "</html>";
    
    server.send(200, "text/html", suc);
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, pass);
  
  endDeviceAPIP = WiFi.softAPIP();
  strEndDeviceAPIP = WiFi.softAPIP().toString();

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(endDeviceAPIP);
  Serial.println();

  server.on("/", connectNetwork);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
