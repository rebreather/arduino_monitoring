#include "PMS.h"
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WebServer.h>
#include "ESPAsyncWebServer.h"

SoftwareSerial soft(17, 16); // 센서의 RX, TX와 연결

unsigned char pms[32]; //32byte의 data가 들어옴
String PM1_0, PM2_5, PM10; //총 3가지 data

int buzzer = 15;

const char* ssid = "GY2679";
const char* password = "whtfuck12*";

WebServer server(80);

String SendHTML(int PM1_0, int PM2_5, int PM10)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Wireless Weather Station</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "<script>\n";
  ptr += "setInterval(loadDoc,1000);\n";
  ptr += "function loadDoc() {\n";
  ptr += "var xhttp = new XMLHttpRequest();\n";
  ptr += "xhttp.onreadystatechange = function() {\n";
  ptr += "if (this.readyState == 4 && this.status == 200) {\n";
  ptr += "document.body.innerHTML =this.responseText}\n";
  ptr += "};\n";
  ptr += "xhttp.open(\"GET\", \"/\", true);\n";
  ptr += "xhttp.send();\n";
  ptr += "}\n";
  ptr += "</script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Fine Dust Monitoring</h1>\n";
 
  ptr += "<p>PM1.0: ";
  ptr += PM1_0;
  ptr += " ug/m3</p>";
 
  ptr += "<p>PM2.5: ";
  ptr += PM2_5;
  ptr += " ug/m3</p>";
 
  ptr += "<p>PM10: ";
  ptr += PM10;
  ptr += " ug/m3</p>";
 
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handle_OnConnect()
{
  server.send(200, "text/html", SendHTML(PM1_0.toFloat(), PM2_5.toFloat(), PM10.toFloat()));
} 
 
void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);
  soft.begin(9600); // PMS7003 센서의 기본 통신 속도는 9600 보드레이트

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println("WiFi Connected. IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
  delay(2000);

}

void loop(){
  if(soft.available()>=32){ // 미세먼지 센서가 제대로 된 값을 보내줄 때
    for (int j = 0; j<32; j++){
      pms[j] = soft.read();
      }
    PM1_0 = (pms[10]<<8) | pms[11];
    PM2_5 = (pms[12]<<8) | pms[13];
    PM10 = (pms[14]<<8) | pms[15];

  Serial.printf("PM1.0: %s, PM2.5: %s, PM10: %s \r\n", PM1_0, PM2_5, PM10);  
  }
  delay(1000);
  if (PM2_5.toFloat() >= 10)
  {
    digitalWrite(buzzer, HIGH);
  }
  server.handleClient();
}
