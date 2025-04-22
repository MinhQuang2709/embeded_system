#include <Wire.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <BH1750.h>
#include <DHT.h>

// Constants
#define NODE_ID 1 
#define DEST_ID 3 

#define LORA_CS 5
#define LORA_RST 14
#define LORA_IRQ 2

#define DHTPIN 4
#define DHTTYPE DHT22

#define DUST_LED_PIN 27
#define DUST_SENSOR_PIN 34  // Chân analog input từ GP2Y1010AU0F

// Objects
RH_RF95 LoRa(LORA_CS, LORA_IRQ);
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();
  lightMeter.begin();

  // LoRa setup
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, HIGH);
  delay(10);
  digitalWrite(LORA_RST, LOW);
  delay(10);
  digitalWrite(LORA_RST, HIGH);
  delay(10);

  if (!LoRa.init()) {
    Serial.println("LoRa init failed");
    while (1);
  }

  LoRa.setTxPower(23, false);
  LoRa.setFrequency(433.0);
  Serial.println("LoRa Node 1 Started");

  // Dust sensor setup
  pinMode(DUST_LED_PIN, OUTPUT);
}

float readDustSensor() {
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(280);
  int adcValue = analogRead(DUST_SENSOR_PIN);
  delayMicroseconds(40);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(9680);

  float voltage = adcValue * (3.3 / 4095.0);
  float dustDensity = (voltage - 0.1) * 1000.0 / 0.5; // theo datasheet
  return dustDensity < 0 ? 0 : dustDensity;
}

void loop() {
  float lux = lightMeter.readLightLevel();
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float dust = readDustSensor();

  char payload[100];
  snprintf(payload, sizeof(payload),
           "{ \"from\": %d, \"to\": %d, \"lux\": %.2f, \"temp\": %.2f, \"hum\": %.2f, \"dust\": %.2f }",
           NODE_ID, DEST_ID, lux, temp, hum, dust);

  LoRa.send((uint8_t*)payload, strlen(payload));
  LoRa.waitPacketSent();

  Serial.println("Sent: " + String(payload));
  delay(60000);
}

