#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <ESP_Google_Sheet_Client.h>
#include <GS_SDHelper.h>
#include "time.h"

#define WIFI_SSID "TP-LINK_FF5DB2"
#define PROJECT_ID "embeded-system-457300"
#define CLIENT_EMAIL "embeded-system@embeded-system-457300.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDRGKDLkD4t0fQH\neruJwuvxob36S/+eqkWlskv3gjHrtsOh9p4epGBRSPzrq591VHQWlOjzi0qaBRnX\nYYzGVN5oDpyetnIemoHWzd7Ktc4AfNNkWzLQbnJLoyI5eze6w5z7sK+/9RfEzimb\n6y09YoWwlIlsk4pdL8enZ82W26BtrT759oWsFMoBbZS2ZRim4PcVSq/jueGImVb9\nIosx6cTAG2+dd6i6XgmIUybBrYoI82lBvyYbSYwgLxlH0tqx0SlyeMhP+GyFarXA\nC84Jf3l6xYIeTkMDKosQ/IOCYDnlpoBp8NpX77KAHl3Va6M/7KCEZB1lsunkPrLG\nzjQdQSybAgMBAAECggEAQCgC2kLRWUTEz/CwDzZwJ609ecmMzpWnFg6tcW2QvKbQ\nwubFIFjGNtqiw/reS+VUU0NwcJ2qD7UF90h2pboPnbTyCF6piEtXEF4NgHwlD5A9\nWlsDLeHdjuiojSOJ93OivLU7T7Y3i0rIiI8xy9Zpd8MzoU1p1GrfIzXd5m5P/KZv\n3nBBTkKNZHDw/1L77TopdqnDYwLFzymFX84q0AniZENP2Gtn+pcdDk+dxPJoXIJU\n0SMC+paFhv9vHqIP8agis1L6lJCSW1OVcvNCGPDlEleh8Z3WXVPbdl7As/8gqpy7\nPCr+z0kwzSB6p15BqHPbJBI+utCgl0SL1lCQ813sSQKBgQD2J/FUkwZ121PUBygi\nxYlUfvP2wP6Lm1k/H5SWMqaad3Rn3qkhXjO0QSqtJb7FveMM0QVLgjQmPYQt5cxI\nf8yGIBdG2RAnMqj1vYARjR2CMIMTY0Ve7NN9jcgFNDNqQQWOCOSQQIsdrSPkzqGa\nPfNz7F09CAh1L6sSZIUsyeXWAwKBgQDZdUe7t+1XRjxPw509yCjO7MqpxbYPm7ed\n/PEuMC9Ri0vbNac3h96Wq97OdCCNtDtRRpUZJM/cMzrAh5yHHD2nwK+Pb6YTydsB\nNGKDTWY2Kkbb+LmqpkkxBCXIJ4Nq5m+q038FWNyZ4xK3GsCgbuLk1JvEahLu+YAN\nJWT52203iQKBgG6TgtV7Nmu7Gu0kSeNU2Vwbq3wtNiuAnZa9mQHfvveksaX3YAKd\nTpyqqYEec5RguJ4JR1ekJckH2VjE4f+vaTDXOZ2Z5Ke3Gi9J8T3xJlBVeow7C+6Y\ndGjyR+gOiwowK06isAoc9GFgZb9jEzU2nyZ0B25jLWl3AGJCxZBZIeYBAoGAViC6\ncaIukeOSnbEHhWHSr+pfdpCFRHdTaXK2f6m8BBscx0zf1wGsapbWH4UlI5rf55gC\nE7xdy4RK39iWaXkI/nk1z6TT7YCEilyaLBEqy4dBG3zO5MUJti1DXZww09VDIzCz\nXYXNkfG7BuTEqIZivWlr7IZqy1XsnshRzxhWkKkCgYAEBERVPN2KHX0JEHErIIf8\n1ZbCR0JwsXEEeXJJWWB0rW69mGr9XIqUKLwMxPmg2Yz6IMl7IX7vBvHUAurmqG5U\nGo3SlzF6jZYOm5O2lNsNgMsFiF7lGQahRpF+Rme54++vQ/23Hscbe7hC4eSp37RM\n4CHpKNNnUF0tpru0p6cX1Q==\n-----END PRIVATE KEY-----\n";
const char spreadsheetId[] = "1QB4osGvf5eOsOVI6wNXkWKuiM5S9FtsIskgGcEGt7NQ";
const char* ntpServer = "pool.ntp.org";

#define LORA_CS   5
#define LORA_RST  14
#define LORA_IRQ  2
#define NODE_ID   3

RH_RF95 lora(LORA_CS, LORA_IRQ);
WiFiClientSecure secureClient;

void ensureWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting...");
    WiFi.disconnect();
    WiFi.reconnect();
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 20) {
      delay(500);
      Serial.print(".");
      attempt++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi reconnected: " + WiFi.localIP().toString());
    } else {
      Serial.println("\nReconnect failed!");
    }
  }
}

void sendDataToGoogleSheets(float lux, float dust, float temp, float humid) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Không lấy được thời gian!");
    return;
  }

  timeinfo.tm_hour = (timeinfo.tm_hour + 7) % 24;  // UTC+7
  char timeStr[16];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

  FirebaseJson valueRange, response;
  valueRange.add("majorDimension", "ROWS");
  valueRange.set("values/[0]/[0]", timeStr);     // Thời gian
  valueRange.set("values/[0]/[1]", lux);         // Ánh sáng
  valueRange.set("values/[0]/[2]", dust);        // Bụi
  valueRange.set("values/[0]/[3]", temp);        // Nhiệt độ
  valueRange.set("values/[0]/[4]", humid);       // Độ ẩm

  if (GSheet.values.append(&response, spreadsheetId, "Sheet1", &valueRange)) {
    Serial.println("Đã gửi dữ liệu lên Google Sheet!");
    response.toString(Serial, true);
  } else {
    Serial.println("Gửi thất bại: " + GSheet.errorReason());
  }
}

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token status: %s\n", GSheet.getTokenStatus(info).c_str());
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Node Gateway ESP32 + LoRa + Google Sheets");

  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi kết nối: " + WiFi.localIP().toString());
  secureClient.setInsecure();

  // Google Sheets init
  configTime(0, 0, ntpServer);
  GSheet.setTokenCallback(tokenStatusCallback);
  GSheet.setPrerefreshSeconds(600);
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

  // LoRa init
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, HIGH); delay(10);
  digitalWrite(LORA_RST, LOW); delay(10);
  digitalWrite(LORA_RST, HIGH); delay(10);

  if (!lora.init()) {
    Serial.println("LoRa init thất bại!");
    while (1);
  }
  lora.setFrequency(433.0);
  Serial.println("LoRa sẵn sàng!");
}

void loop() {
  ensureWiFiConnection();

  if (lora.available()) 
  {
    uint8_t buffer[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t length = sizeof(buffer);

    if (lora.recv(buffer, &length)) 
    {
      buffer[length] = '\0';
      String receivedMessage = String((char*)buffer);
      Serial.println("Nhận: " + receivedMessage);

      int senderId, recipientId;
      float lux, dust, temp, humid;

      if (sscanf(receivedMessage.c_str(),
        "{ \"from\": %d, \"to\": %d, \"lux\": %f, \"dust\": %f, \"temp\": %f, \"humid\": %f }",
        &senderId, &recipientId, &lux, &dust, &temp, &humid) == 6)
      {
        if (recipientId == NODE_ID) 
        {
          Serial.println("Dữ liệu đúng node!");
          sendDataToGoogleSheets(lux, dust, temp, humid);
        } 
        else 
        {
          Serial.println("Không đúng ID node này!");
        }
      } 
      else 
      {
        Serial.println("Lỗi parse dữ liệu!");
      }
    }
  }
}
