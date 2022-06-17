#include <WiFi.h> // ESP 8266 와이파이 라이브러리
#include <HTTPClient.h> // HTTP 클라이언트
#include "SPIFFS.h"


void setup()
{

  Serial.begin(115200);

    if (!SPIFFS.begin(true)) {
    Serial.println("# ERROR: can not mount SPIFFS");
    while (1) ;
  }
  
  Serial.println();

  // 와이파이 접속
  WiFi.begin("GY2679", "whtfuck12*"); // 공유기 이름과 비밀번호

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력
}

void loop() {

  File f = SPIFFS.open("/text_example.txt");
  if (!f) {  // can not open file
    Serial.println("# ERROR: can not open file");
    delay(5000);
    return;
  }

  while (f.available()) {
    Serial.write(f.read());
  }
  Serial.println("");

  f.close();
  delay(5000);


 
 }
