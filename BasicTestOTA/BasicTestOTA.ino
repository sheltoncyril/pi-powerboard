#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <wifi_credentials.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

#define REL1 D8
#define REL2 D7
#define REL3 D6
#define REL4 D5
#define ST_LED D0
uint8_t pins[] = {REL1, REL2, REL3, REL4};
int del= 500;
const char* ssid = STASSID;
const char* password = STAPSK;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(50);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("pi-powerboard");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  for(int i =0; i<sizeof(pins);i++){
    pinMode(pins[i],OUTPUT);
  }
  pinMode(ST_LED,OUTPUT);
  digitalWrite(ST_LED, HIGH);
  delay(50);
  digitalWrite(ST_LED, LOW);
  delay(50);
  digitalWrite(ST_LED, HIGH);
  delay(50);
  digitalWrite(ST_LED, LOW);
}

void loop() {
  ArduinoOTA.handle();
  digitalWrite(ST_LED, HIGH);
  delay(50);
  digitalWrite(ST_LED, LOW);
  delay(50);
  digitalWrite(ST_LED, HIGH);
  delay(50);
  digitalWrite(ST_LED, LOW);
  
  for(int i =0; i<sizeof(pins);i++){
    digitalWrite(pins[i],HIGH);
    delay(del);
  }
  digitalWrite(ST_LED, HIGH);
  delay(50);
  digitalWrite(ST_LED, LOW);
  delay(50);
  digitalWrite(ST_LED, HIGH);
  delay(50);
  digitalWrite(ST_LED, LOW);
  delay(del);
  for(int i =0; i<sizeof(pins);i++){
    digitalWrite(pins[i],LOW);
    delay(del);
  }
}
