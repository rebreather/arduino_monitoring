#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
 
const char* ssid     = "SK_WiFi4660";
const char* password = "1104071678";
 
// HTTP 로 연결될 포트 설정
AsyncWebServer server(80);
 
void setup()
{
    Serial.begin(115200);
    delay(10);
    
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
 
    WiFi.begin(ssid, password);
 
    // 와이파이망에 연결
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  
 
    setApi();
    startServer();
}
 
// HTTP route 및 method 구현
// method : HTTP_POST, HTTP_GET, HTTP_PUT, HTTP_ANY
void setApi() {    
    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello World");
    });
}
 
// HTTP 서버 시작
void startServer() {
    server.begin();
}
 
void loop()
{
    
}
