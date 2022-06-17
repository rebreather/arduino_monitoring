#include <WiFi.h> 
#include <HTTPClient.h> 

const char* ssid = "GY2679";
const char* password = "whtfuck12*";
const String endpoint = "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=1159068000";
String line = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to the WiFi network");
  get_weather();
}

void get_weather() {
  if ((WiFi.status() == WL_CONNECTED)) { 
    Serial.println("Starting connection to server...");
    HTTPClient http;
    http.begin(endpoint);       
    int httpCode = http.GET();  
    if (httpCode > 0) {        
      line = http.getString();
    }
    else {
      Serial.println("Error on HTTP request");
    }
//    Serial.println(line); 
    parsing();
    http.end(); //Free the resources
  }
}

void loop() {

}

// 온도 저장 변수
float temp0;
float temp1;
float temp2;

void parsing() {
  String announce_time;  
  int tm_start= line.indexOf(F("<tm>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int tm_end= line.indexOf(F("</tm>"));  
  announce_time = line.substring(tm_start + 4, tm_end); // +4: "<tm>"스트링의 크기 4바이트, 4칸 이동
  Serial.print(F("announce_time: ")); Serial.println(announce_time);
  String hour;
  int hour_start= line.indexOf(F("<hour>"));
  int hour_end= line.indexOf(F("</hour>"));
  hour = line.substring(hour_start + 6, hour_end);
  Serial.print(F("hour: ")); Serial.println(hour);
  String temp;
  int temp_start= line.indexOf(F("<temp>"));
  int temp_end= line.indexOf(F("</temp>"));
  temp = line.substring(temp_start + 6, temp_end);
  Serial.print(F("temp: ")); Serial.println(temp);
  temp0 = temp.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("temp0: ")); Serial.println(temp0);
  String wfEn;
  int wfEn_start= line.indexOf(F("<wfEn>"));
  int wfEn_end= line.indexOf(F("</wfEn>"));
  wfEn = line.substring(wfEn_start + 6, wfEn_end);
  Serial.print(F("weather: ")); Serial.println(wfEn);
  int del_index = line.indexOf(F("</data>")); 
  line.remove(0, del_index + 7);                     // 시작 인덱스 부터 "</data>" 스트링 포함 삭제
  hour_start= line.indexOf(F("<hour>"));
  hour_end= line.indexOf(F("</hour>"));
  hour = line.substring(hour_start + 6, hour_end);
  Serial.print(F("hour: ")); Serial.println(hour);
  temp_start= line.indexOf(F("<temp>"));
  temp_end= line.indexOf(F("</temp>"));
  temp = line.substring(temp_start + 6, temp_end);
  Serial.print(F("temp: ")); Serial.println(temp);
  temp1 = temp.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("temp1: ")); Serial.println(temp1);
  wfEn_start= line.indexOf(F("<wfEn>"));
  wfEn_end= line.indexOf(F("</wfEn>"));
  wfEn = line.substring(wfEn_start + 6, wfEn_end);
  Serial.print(F("weather: ")); Serial.println(wfEn);
  del_index = line.indexOf(F("</data>"));
  line.remove(0, del_index + 7);                     // 시작 인덱스 부터 "</data>" 스트링 포함 삭제
  hour_start= line.indexOf(F("<hour>"));
  hour_end= line.indexOf(F("</hour>"));
  hour = line.substring(hour_start + 6, hour_end);
  Serial.print(F("hour: ")); Serial.println(hour);
  temp_start= line.indexOf(F("<temp>"));
  temp_end= line.indexOf(F("</temp>"));
  temp = line.substring(temp_start + 6, temp_end);
  Serial.print(F("temp: ")); Serial.println(temp);
  temp1 = temp.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("temp2: ")); Serial.println(temp1);
  wfEn_start= line.indexOf(F("<wfEn>"));
  wfEn_end= line.indexOf(F("</wfEn>"));
  wfEn = line.substring(wfEn_start + 6, wfEn_end);
  Serial.print(F("weather: ")); Serial.println(wfEn);
  line = ""; // 스트링 변수 line 데이터 추출 완료 
}
