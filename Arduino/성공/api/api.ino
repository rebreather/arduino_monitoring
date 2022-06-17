#include <WiFi.h> // ESP 8266 와이파이 라이브러리
#include <HTTPClient.h> // HTTP 클라이언트

String sido = "서울"; // 서울, 부산, 대구, 인천, 광주, 대전, 울산, 경기, 강원,
                     // 충북, 충남, 전북, 전남, 경북, 경남, 제주, 세종 중 입력
String key = "kM44gBj8ydNifA7DoBJ1jFAU9e4qSa0Bw1wyX3rXI%2F8K1HfwnaJIqBFs3as430WCwY%2F4VyKUPJBVH1JBSkgelg%3D%3D";

String url = "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty?serviceKey=kM44gBj8ydNifA7DoBJ1jFAU9e4qSa0Bw1wyX3rXI%2F8K1HfwnaJIqBFs3as430WCwY%2F4VyKUPJBVH1JBSkgelg%3D%3D&returnType=xml&numOfRows=100&pageNo=1&sidoName=%EC%84%9C%EC%9A%B8&ver=1.0";

float o3, pm10, pm25, so2, co, no2 = 0;
      // 오존, 미세먼지, 초미세먼지
      
float dataTime;

void setup()
{

  Serial.begin(115200);
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

float getNumber(String str, String tag, int from) {
  float num = -1;
  int f = str.indexOf(tag, from) + tag.length(); //str에서 tag<>를 from 이후로 찾고(첫번째 명령어) tag의 길이를 더하면(두번째 명령어)
                                                 // tag의 값이 시작하는 인덱스 값이 나온다. 그것을 f에 저장함
  int t = str.indexOf("<", f); //t는 f의 인덱스 이후로 오는 <의 인덱스 값
  String s = str.substring(f, t); //f의 인덱스부터 시작해서 t 인덱스 이전까지의 문자열들을 s에 저장함
  
  return s.toFloat(); // 문자열 s를 toFloat함수를 이용해 숫자로 변환하여 getNumber 함수가 이 값을 돌려주게끔 함.
}
