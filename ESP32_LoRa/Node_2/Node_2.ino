#include <SPI.h>
#include <RH_RF95.h>

#define NODE_ID 2   
#define LORA_CS 5
#define LORA_RST 14
#define LORA_IRQ 2

RH_RF95 LoRa(LORA_CS, LORA_IRQ);

void setup() {
  Serial.begin(115200);
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, HIGH);

  if (!LoRa.init()) 
  {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setTxPower(20, false);
  LoRa.setFrequency(433.0);
  Serial.println("✅ LoRa Node 2 Started");
}

void loop() {
  if (LoRa.available()) 
  {
    uint8_t buf[128];  
    uint8_t len = sizeof(buf);

    if (LoRa.recv(buf, &len)) 
    {
      buf[len] = '\0'; 
      String receivedData = String((char*)buf);
      Serial.println("📥 Received: " + receivedData);
      Serial.print("RSSI: ");
      Serial.print(LoRa.lastRssi());
      Serial.println(" dBm");

      // Giải mã chuỗi JSON thô
      int from, to;
      float lux, temp, hum, dust;
      int matched = sscanf(receivedData.c_str(),
        "{ \"from\": %d, \"to\": %d, \"lux\": %f, \"temp\": %f, \"hum\": %f, \"dust\": %f }",
        &from, &to, &lux, &temp, &hum, &dust
      );

      if (matched >= 3) 
      {
        if (to == NODE_ID) 
        {
          Serial.println("Data intended for this node, processing...");
        } 
        else 
        {
          // Forward đến node tiếp theo với ID mới là NODE_ID
          char newPayload[128];
          snprintf(newPayload, sizeof(newPayload),
            "{ \"from\": %d, \"to\": %d, \"lux\": %.2f, \"temp\": %.2f, \"hum\": %.2f, \"dust\": %.2f }",
            NODE_ID, to, lux, temp, hum, dust);

          Serial.println("➡️ Forwarding to node " + String(to) + ": " + String(newPayload));

          LoRa.send((uint8_t*)newPayload, strlen(newPayload));
          LoRa.waitPacketSent();
        }
      } 

      else 
      {
        Serial.println("Không thể phân tích dữ liệu!");
      }
    }
  }
}
