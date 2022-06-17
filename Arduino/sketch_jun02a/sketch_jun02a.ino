#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "PMS.h"
#include <SoftwareSerial.h>
#include "ESP32Servo.h"

#define SERVO_PIN 21
Servo myservo;

SoftwareSerial soft(17, 16);

PMS pms(soft);
PMS::DATA data;

const char* ssid = "GY2679";
const char* password = "whtfuck12*";

AsyncWebServer server(80);


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
</body>

<script>

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
</script>
</html>)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);

  if(var == "PM1_0"){
    return readPM1_0();
  }
  else if(var == "PM2_5"){
    return readPM2_5();
  }
  else if(var == "PM10_0"){
    return readPM10_0();
    }
  return String();
}



void setup(){

  myservo.setPeriodHertz(50); //서보모터의 PWM 주기를 지정
  myservo.attach(SERVO_PIN, 500, 2400); //핀 번호, 최소 펄스 폭, 최대 펄스 폭
  myservo.write(10); //주어진 각도의 위치로 서보모터 회전
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  soft.begin(9600);
  
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
  
  server.on("/pm1_0", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM1_0().c_str());
  });
  server.on("/pm2_5", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM2_5().c_str());
  });
  server.on("/pm10_0", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPM10_0().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){

    if (data.PM_AE_UG_10_0 > 10 && pm10 < 40){ // 실내가 40 이상, 실외가 40 이하 일 때 창문 열기
    myservo.write(175);
    delay(1000);    
    }
    if (data.PM_AE_UG_10_0 < 5 && pm10 > 40 ){ //실내가 40 이하, 실외가 40 이상일 때 창문 닫기
    myservo.write(10);
    delay(1000);    
    }

    
    if(pms.read(data)){
      
    Serial.print("PM 1.0(ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5(ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0(ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }




}
