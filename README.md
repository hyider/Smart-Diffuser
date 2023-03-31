IoT 기반 디퓨저 개발
===================
## 목차
### [개발 1주차](https://github.com/hyider/Smart-Diffuser/edit/main/README.md#%EA%B0%9C%EB%B0%9C-1%EC%A3%BC%EC%B0%A8-1)
* [소프트웨어 구조](https://github.com/hyider/Smart-Diffuser/edit/main/README.md#%EC%86%8C%ED%94%84%ED%8A%B8%EC%9B%A8%EC%96%B4-%EA%B5%AC%EC%A1%B0)
* [Library](https://github.com/hyider/Smart-Diffuser/edit/main/README.md#library)
* [Function](https://github.com/hyider/Smart-Diffuser/edit/main/README.md#function)

개발 1주차 
===========
ESP8266의 WiFi연결 상태를 주변 WiFi로 연결  
메인 웹 페이지에서 로그인 후 디퓨저와 계정 연동 확인
  
### 기기와 계정 인증 방식
* ESP8266의 MAC Address를 사용하여 웹에서 기기 등록
* ESP8266의 WiFi 설정 시 계정 등록
  
ESP8266이 주변 와이파이를 스캔하여 웹에서 리스트들을 출력하고  
사용자가 리스트 밑의 입력칸에 연결할 WiFi 이름과 암호를 입력하면 연결 가능

## 소프트웨어 구조
![SWS](https://v1.padlet.pics/1/image.webp?t=c_limit%2Cdpr_1%2Ch_856%2Cw_1003&url=https%3A%2F%2Fstorage.googleapis.com%2Fpadlet-uploads%2F2000607965%2F0a160e42c3e7b8187a3d6543434478fc%2F__________.jpg%3FExpires%3D1680701447%26GoogleAccessId%3D778043051564-q79bsd8mc40b0bl82ikkrtc3jdofe4dg%2540developer.gserviceaccount.com%26Signature%3DnJqmR0G8nt6kvBR1ua5ZXtNjBZ36wSf%252B07tBW7XjL5xqqIQnlT7IygND1a709HcQWQefMNPTB7hlx7T6svjLj2OtVLAWko3zFgobKWbG8ZgBtF76jXvLbhQsZMCwxVgN0jT3fWbJ4nCpq0NjfEFeHI6fNEn8xR%252BKtloAsVlUsmo%253D%26original-url%3Dhttps%253A%252F%252Fpadlet-uploads.storage.googleapis.com%252F2000607965%252F0a160e42c3e7b8187a3d6543434478fc%252F__________.jpg)

## Library
* ### ESP8266WiFi.h  
* ### ESP8266WebServer.h  
* ### [Libraries Reference - WiFi](https://www.arduino.cc/reference/en/libraries/)
* ### [Libraries Reference - ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)

## Function
### connectNetwork()
> 주변 WiFi를 검색하여 HTML 코드로 저장하는 작업을 수행.
> ```
> void connectNetwork()
> {
>   Serial.println(ssid);
>     Serial.println(pass);
>     
>     /* 메인 웹 */
>     String web = "";
>     web += "<html>";
>     web += "<head><title>Search Network</title><meta charset=\"utf-8\"></head>";
>     web += "<body>";
>     web += "<h1 align = \"center\">네트워크 검색</h1>";
>     web += "<table border=\"1\" bordercolor=\"black\" width=\"500\" heigh=\"300\" align=\"center\">";
>     web += "<tr align=\"center\"><p><td colspan=\"5\">네트워크 목록</td></p>";
>     web += "<tr align=\"center\"><td>번호</td><td>이름</td><td>신호</td><td>보안</td></tr>";
>     web += printList();
>     web += "</table><br>";
>     web += "<p align=\"center\"><button onClick=\"window.location.reload()\">검색</button></p>";
>     web += "<br><br>";
>     web += "<form>";
>     web += "<h2 align=\"center\">네트워크 연결</h2>";
>     web += "<form method=\"POST\" action=\"/\">";
>     web += "<p align=\"center\"><label for=\"id\">이름&nbsp;&nbsp;</label><input type=\"text\" id=\"id\" name=\"id\"></p>";
>     web += "<p align=\"center\"><label for=\"pw\">암호&nbsp;&nbsp;</label><input type=\"password\" id=\"pw\" name=\"pw\"></p>";
>     web += "<p align=\"center\"><input type=\"submit\" value=\"연결\"></p>";
>     web += "</form>";
>     web += "</body>";
>     web += "</html>";
>   
>     server.send(200, "text/html", web);
>   }
> ```

### printList()

> 주변 WiFi를 검색하여 리스트를 출력하는 함수. WiFi의 이름, 신호강도, 암호화 타입을 읽기 좋게 출력함.
> ```
> String printList() {
>   /* 스캔된 와이파이의 수를 저장하는 변수 */
>   byte numNetworks = WiFi.scanNetworks();
>
>   /* 스캔된 와이파이들을 웹 상에서 출력하기 위한 반복문 */
> String web = "";
>   for(int i=0; i<numNetworks; i++)
>   {
>     web += "<tr align=\"center\"><td>(";
>     web += i+1;
>     web += ")</td><td>";
>     /* 와이파이 이름 저장 */
>     web += WiFi.SSID(i);  
>     web += "</td><td>";
>     /* 와이파이 신호, 강도에 따라 이름 부여 */
>     if(WiFi.RSSI(i) > -55){ 
>       dBm = "매우 강함";
>     } else if (WiFi.RSSI(i) > -60) {
>       dBm = "강함";
>     } else if (WiFi.RSSI(i) > -70) {
>       dBm = "양호";
>     } else if (WiFi.RSSI(i) > -80) {
>       dBm = "약함";
>     } else {
>       dBm = "매우 약함";
>     }
>     web += dBm;
>     web += "</td><td>";
>     /* 와이파이의 암호화 종류 표시, 종류에 따라 이름 부여 */
>     switch(WiFi.encryptionType(i)){
>       case 2:
>         encryptionString = "TKIP(WPA)";
>         break;
>       case 4:
>         encryptionString = "CCMP(WPA)";
>         break;
>       case 5:
>         encryptionString = "WEP";
>         break;
>       case 7:
>         encryptionString = "NONE";
>         break;
>       case 8:
>         encryptionString = "AUTO";
>         break;
>       /* 모르는 암호화 종류일 때 WTF 출력 */
>       default:
>         encryptionString = "WTF";
>         break;
>     }
>     web += encryptionString;
>     web += "</td>";
>   }
> 
>   return web;
> }
> ```
    
### resultNetwork()
> 선택한 WiFi를 연결했을 때 결과가 나오는 함수.  
> 연결 실패 구현중...
> ```
> void resultNetwork(){
>   if(server.arg(0) != NULL){
>     /* 와이파이 연결을 위해 입력된 와이파이 이름과 암호를 변수를 넘겨받아 각 변수에 저장 */
>     ssid = server.arg("id");
>     pass = server.arg("pw");
> 
>     Serial.println("Received SSID: " + ssid);
>     Serial.println("Received PASS: " + pass);
> 
>     WiFi.begin(ssid, pass);
> 
>     /* 와이파이 상태를 측정하여 연결이 안되는 경우 계속 점 표시 */
>     while(WiFi.status() != WL_CONNECTED){
>       delay(500);
>       Serial.print(".");
>     }
> 
>     /* 와이파이 연결이 성공한 경우 진행 */
>     Serial.println("");
>     Serial.println("WiFi connected");
>     Serial.println("IP Address: ");
>     Serial.println(WiFi.localIP());
> 
>     /* 연결이 됐음을 알려주는 페이지 */
>     String suc = "";
>     suc += "<html>";
>     suc += "<head><title>Connect Network</title><meta charset=\"utf-8\"></head>";
>     suc += "<body>";
>     suc += "<h1 align=\"center\">네트워크 ";
>     suc += ssid;
>     suc += " 연결 성공</h1><br>";
>     suc += "<p align=\"center\"><button onClick=\"widnow.location.href=\"/\"\">메인</button></p><br>";
>     suc += "</body>";
>     suc += "</html>";
>     
>     server.send(200, "text/html", suc);
>   }
> }
> ```



