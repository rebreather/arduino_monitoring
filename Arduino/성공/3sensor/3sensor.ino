#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "PMS.h"
#include <SoftwareSerial.h>
#include "HTTPClient.h" // HTTP 클라이언트
#include "ESP32Servo.h";


#define DHTPIN 27    
#define DHTTYPE DHT11 
 
#define SERVO_PIN 21
Servo myservo;


String sido = "서울"; // 서울, 부산, 대구, 인천, 광주, 대전, 울산, 경기, 강원,충북, 충남, 전북, 전남, 경북, 경남, 제주, 세종 중 입력
String key = "EwAP2y2XNiNjKn%2FbN8wHdslKYX2WCOFqBBB3zmbT5Wq2i%2F4l%2BUHC9iejSOpXUB47A%2ByfEAoyomBdm%2FfkZH47xQ%3D%3D";
String url = "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty?serviceKey=EwAP2y2XNiNjKn%2FbN8wHdslKYX2WCOFqBBB3zmbT5Wq2i%2F4l%2BUHC9iejSOpXUB47A%2ByfEAoyomBdm%2FfkZH47xQ%3D%3D&returnType=xml&numOfRows=100&pageNo=1&sidoName=%EC%84%9C%EC%9A%B8&ver=1.0";
float o3, pm10, pm25, so2, co, no2 = 0;       // 오존, 미세먼지, 초미세먼지

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial soft(17, 16);

PMS pms(soft);
PMS::DATA data;

const char* ssid = "GY2679";
const char* password = "whtfuck12*";

AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht.readTemperature();

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    return String(t);

  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    return String(h);

  }
}

//실내

String readPM1_0(){       
  int pm1_0 = data.PM_AE_UG_1_0;

    if (isnan(pm1_0)) {    
    Serial.println("Failed to read from Dust sensor!");
    return "--";
  }
  else {
    // Serial.println(pm1_0);
    return String(pm1_0);
  }
  
}

String readPM2_5(){
  int pm2_5 = data.PM_AE_UG_2_5;

    if (isnan(pm2_5)) {    
    Serial.println("Failed to read from Dust sensor!");
    return "--";
  }
  else {
    return String(pm2_5);

  }  
}


String readPM10_0(){
  int pm10_0 = data.PM_AE_UG_10_0;

    if (isnan(pm10_0)) {    
    Serial.println("Failed to read from Dust sensor!");
    return "--";
  }
  else {
    return String(pm10_0);

  }
}

//api
String readpm10(){

    if (isnan(pm10)) {    
    Serial.println("Failed to read from Dust API");
    return "--";
  }
  else {
    //Serial.println(pm10);
    return String(pm10);
  }
}

String readpm25(){

    if (isnan(pm25)) {    
    Serial.println("Failed to read from Dust API");
    return "--";
  }
  else {
    //Serial.println(pm25);
    return String(pm25);
  }
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
  integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
     background-color: #F0FFF0;
    }
    h2 { font-size: 30px; }
    p { font-size: 25px; }
    .units { font-size: 20px; }
    .dust {
        width: 372px;
        height: 136px;
        }
    .dht-labels{
      font-size: 25px;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Indoor Temp and Humi</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <BR>
  <h2> Indoor Dust Monitoring</h2>
  <p>
    <span class="dht-labels">PM 1.0: </span> 
    <span id="pm1_0">%PM1_0%</span>
    <sup class="units">ug/m3</sup>
  </p>
  <p>
    <span class="dht-labels">PM 2.5: </span> 
    <span id="pm2_5">%PM2_5%</span>
    <sup class="units">ug/m3</sup>
  </p>
  <p>
    <span class="dht-labels">PM 10.0: </span> 
    <span id="pm10_0">%PM10_0%</span>
    <sup class="units">ug/m3</sup>
  </p>
  <BR>
  
  <img alt="DUST.png" 
src="data:image/png;base64,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" />  
    
    <h2>Outdoor Dust Monitoring</H2>
  <P>
  <span class> pm10 :  </span>
  <span id = "pm10">%pm10%</span>
  <sup class="units">ug/m3</sup>
  </P>

  <P>
  <span class> pm2.5 :  </span>
  <span id = "pm10">%pm25%</span>
  <sup class="units">ug/m3</sup>
  </P>
</body>

<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm1_0").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm1_0", true);
  xhttp.send();
}, 1000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm2_5").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm2_5", true);
  xhttp.send();
}, 1000 ); 

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm10_0").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm10_0", true);
  xhttp.send();
}, 1000 );

  setInterval(function ( ) { //실외
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm10").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm10", true);
  xhttp.send();
}, 1000 );

  setInterval(function ( ) { //실외
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm25").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm25", true);
  xhttp.send();
}, 1000 );
</script>
</html>)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "PM1_0"){ //실내
    return readPM1_0();
  }
  else if(var == "PM2_5"){ //실내
    return readPM2_5();
  }
  else if(var == "PM10_0"){ //실내
    return readPM10_0();
    }
  else if (var == "pm10"){ //실외
    return String(pm10);
  }
  else if(var == "pm25"){ //실외
    return String(pm25);
  }
  return String();
}

float getNumber(String str, String tag, int from) {
  float num = -1;
  int f = str.lastIndexOf(tag, from) + tag.length(); //str에서 tag<>를 from 이후로 찾고(첫번째 명령어) tag의 길이를 더하면(두번째 명령어)
                                                 // tag의 값이 시작하는 인덱스 값이 나온다. 그것을 f에 저장함
  int t = str.indexOf("<", f); //t는 f의 인덱스 이후로 오는 <의 인덱스 값
  String s = str.substring(f, t); //f의 인덱스부터 시작해서 t 인덱스 이전까지의 문자열들을 s에 저장함
  
  return s.toFloat(); // 문자열 s를 toFloat함수를 이용해 숫자로 변환하여 getNumber 함수가 이 값을 돌려주게끔 함.
}


void setup(){
  
  myservo.setPeriodHertz(50); //서보모터의 PWM 주기를 지정
  myservo.attach(SERVO_PIN, 500, 2400); //핀 번호, 최소 펄스 폭, 최대 펄스 폭
  myservo.write(10); //주어진 각도의 위치로 서보모터 회전
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  soft.begin(9600);

  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.print("WiFi Connected. IP: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  }); 
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  
  server.on("/pm1_0", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM1_0().c_str());
  });
  server.on("/pm2_5", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM2_5().c_str());
  });
  server.on("/pm10_0", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM10_0().c_str());
  });

 server.on("/pm10",HTTP_GET,[](AsyncWebServerRequest *request) {
        request->send_P(200, "text/plain", readpm10().c_str());
  });
  
 server.on("/pm25",HTTP_GET,[](AsyncWebServerRequest *request) {
        request->send_P(200, "text/plain", readpm25().c_str());
  });

  // Start server
  server.begin();

}
 
void loop(){
        

    if(pms.read(data)){ //실내 측정

      if (data.PM_AE_UG_10_0 >= 3 || pm10 <= 15){ // 실내가 40 이상, 실외가 40 이하 일 때 창문 열기
        myservo.write(100);
        delay(10000);    
        }
        
      if (data.PM_AE_UG_10_0 < 2 || pm10 > 15){ //실내가 40 이하, 실외가 40 이상일 때 창문 닫기
        myservo.write(10);
        delay(10000); 
        }
  }
  
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int pm2_5 = data.PM_AE_UG_2_5;
    int pm10_0 = data.PM_AE_UG_10_0;
  

   if (WiFi.status() == WL_CONNECTED) // 와이파이가 접속되어 있는 경우
  {
    WiFiClient client; // 와이파이 클라이언트 객체
    HTTPClient http; // HTTP 클라이언트 객체   
    

    if (http.begin(client, url)) {  // HTTP
      // 서버에 연결하고 HTTP 헤더 전송
      int httpCode = http.GET();

      // httpCode 가 음수라면 에러
      if (httpCode > 0) { // 에러가 없는 경우

        
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString(); // 받은 XML 데이터를 String에 저장
          int cityIndex = payload.indexOf("양천구");

          //o3 = getNumber(payload, "<o3Value>", cityIndex); // 오존
          //so2 = getNumber(payload, "<so2Value>", cityIndex); // 이산화황
          //co = getNumber(payload, "<coValue>", cityIndex); // 일산화탄소
          //no2 = getNumber(payload, "<no2Value>", cityIndex); // 이산화질소
          pm10 = getNumber(payload, "<pm10Value>", cityIndex); // 미세먼지
          pm25 = getNumber(payload, "<pm25Value>", cityIndex); // 초미세먼지

        }
      } else {
        Serial.printf("[HTTP] GET... 실패, 에러코드: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
    else {
      Serial.printf("[HTTP] 접속 불가\n");
    }
    
    Serial.println();
    //Serial.print("o3: "); Serial.print(o3,3); Serial.println("ug/m3"); // 오존
    //Serial.print("co: "); Serial.print(co,3); Serial.println("ug/m3"); // 일산화탄소
    //Serial.print("so2: "); Serial.print(so2,3); Serial.println("ug/m3"); // 이산화황
    //Serial.print("no2: "); Serial.print(no2,3); Serial.println("ug/m3"); // 이산화질소
        
    Serial.print("Indoor Temperature: ");
    Serial.println(t);        
    Serial.print("Indoor Humidity: ");
    Serial.println(h);

        
    Serial.print("Indoor pm2.5: ");
    Serial.println(pm2_5);

        
    Serial.print("Indoor pm10_0: ");
    Serial.println(pm10_0);

    
    Serial.print("Outdoor pm10: "); Serial.print(pm10); Serial.println("ug/m3"); //미세먼지
    Serial.print("Outdoor pm2.5: "); Serial.print(pm25); Serial.println("ug/m3"); // 초미세먼지

    delay(10000);

  }  
  
}
