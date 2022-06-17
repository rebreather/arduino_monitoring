#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "PMS.h"
#include <SoftwareSerial.h>
#include "HTTPClient.h" // HTTP 클라이언트


#define DHTPIN 27    
#define DHTTYPE DHT11  

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
    Serial.println(t);
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
    Serial.println(h);
    return String(h);
  }
}

String readPM1_0(){
  int pm1_0 = data.PM_AE_UG_1_0;

    if (isnan(pm1_0)) {    
    Serial.println("Failed to read from Dust sensor!");
    return "--";
  }
  else {
    Serial.println(pm1_0);
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
    Serial.println(pm2_5);
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
    Serial.println(pm10_0);
    return String(pm10_0);
  }  
}

//api
String readpm10(){

    if (isnan(pm10)) {    
    Serial.println("Failed to read from Dust sensor!");
    return "--";
  }
  else {
    Serial.println(pm10);
    return String(pm10);
  }  
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 30px; }
    p { font-size: 25px; }
    .units { font-size: 20px; }
    .dht-labels{
      font-size: 25px;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
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
  <BR>
  <h2> Fine Dust Monitoring</h2>
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

    <h2>API TEST </H2>
  <P>
  <span class> pm10 :  </span>
  <span id = "pm10">%pm10%</span>
  </P>

   <P>
  <span class> pm25 :  </span>
  <span id = "pm10">%pm25%</span>
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
}, 10000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm1_0").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm1_0", true);
  xhttp.send();
}, 10000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm2_5").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm2_5", true);
  xhttp.send();
}, 10000 );

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm10_0").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm10_0", true);
  xhttp.send();
}, 10000 );

  setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm10").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm10", true);
  xhttp.send();
}, 10000 );

  setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pm25").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pm25", true);
  xhttp.send();
}, 10000 );
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
  else if(var == "PM1_0"){
    return readPM1_0();
  }
  else if(var == "PM2_5"){
    return readPM2_5();
  }
  else if(var == "PM10_0"){
    return readPM10_0();
    }
  else if (var == "pm10"){
    return String(pm10);
  }
  else if(var == "pm25"){
    return String(pm25);
  }
  return String();
}

float getNumber(String str, String tag, int from) {
  float num = -1;
  int f = str.indexOf(tag, from) + tag.length(); //str에서 tag<>를 from 이후로 찾고(첫번째 명령어) tag의 길이를 더하면(두번째 명령어)
                                                 // tag의 값이 시작하는 인덱스 값이 나온다. 그것을 f에 저장함
  int t = str.indexOf("<", f); //t는 f의 인덱스 이후로 오는 <의 인덱스 값
  String s = str.substring(f, t); //f의 인덱스부터 시작해서 t 인덱스 이전까지의 문자열들을 s에 저장함
  
  return s.toFloat(); // 문자열 s를 toFloat함수를 이용해 숫자로 변환하여 getNumber 함수가 이 값을 돌려주게끔 함.
}


void setup(){
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
  Serial.println("WiFi Connected. IP: ");
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


  // Start server
  server.begin();
}
 
void loop(){
    if(pms.read(data)){
      
    Serial.print("PM 1.0(ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5(ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0(ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }

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
          int cityIndex = payload.indexOf("강남구");

          o3 = getNumber(payload, "<o3Value>", cityIndex); // 오존
          so2 = getNumber(payload, "<so2Value>", cityIndex); // 이산화황
          co = getNumber(payload, "<coValue>", cityIndex); // 일산화탄소
          no2 = getNumber(payload, "<no2Value>", cityIndex); // 이산화질소
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
    Serial.print("o3: "); Serial.print(o3,3); Serial.println("ug/m3"); // 오존
    Serial.print("co: "); Serial.print(co,3); Serial.println("ug/m3"); // 일산화탄소
    Serial.print("so2: "); Serial.print(so2,3); Serial.println("ug/m3"); // 이산화황
    Serial.print("no2: "); Serial.print(no2,3); Serial.println("ug/m3"); // 이산화질소
    Serial.print("pm10: "); Serial.print(pm10); Serial.println("ug/m3"); //미세먼지
    Serial.print("pm2.5: "); Serial.print(pm25); Serial.println("ug/m3"); // 초미세먼지
   // Serial.println(dataTime);

    delay(10000);


    
  }
  
  
}
