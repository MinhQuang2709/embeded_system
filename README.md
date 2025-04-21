# 🔌 ESP32 LoRa Mesh Sensor Network

Mô tả ngắn gọn: Hệ thống thu thập dữ liệu môi trường sử dụng mạng **LoRa Mesh** gồm 3 node ESP32, truyền dữ liệu cảm biến đến Google Sheets. Ứng dụng trong giám sát môi trường từ xa, học tập và nghiên cứu mạng không dây.

---

## 📑 Mục Lục

- [👋 Giới Thiệu](#-giới-thiệu)  
- [📐 Thông Số Kỹ Thuật](#-thông-số-kỹ-thuật)  
- [🧰 Danh Sách Linh Kiện](#-danh-sách-linh-kiện)  
- [🔧 Sơ Đồ Nguyên Lý, Kết Nối và PCB](#-sơ-đồ-nguyên-lý-kết-nối-và-pcb)  
- [🔩 Hướng Dẫn Lắp Ráp](#-hướng-dẫn-lắp-ráp)  
- [💻 Lập Trình Firmware](#-lập-trình-firmware)    

---

## 👋 Giới Thiệu

Dự án xây dựng mạng cảm biến không dây sử dụng **ESP32 + SX1278** theo topology **mesh (flooding)** gồm:

- 1 **Sensor Node**: thu thập dữ liệu từ DHT22, BH1750, GP2Y1010AU0F
- 1 **Hub Node**: truyền tiếp gói tin giữa các node
- 1 **Gateway Node**: nhận dữ liệu và gửi lên Google Sheets

### Đối tượng sử dụng:
- Sinh viên, kỹ sư, nhà nghiên cứu về IoT & mạng mesh

### Mục tiêu:
- Học tập, nghiên cứu mô hình LoRa mesh và ứng dụng thu thập dữ liệu từ xa

---

## 📐 Thông Số Kỹ Thuật

| Thành phần | Thông tin |
|------------|-----------|
| MCU        | ESP32-WROOM-32 |
| Giao tiếp  | LoRa 433MHz (SX1278), WiFi |
| Cảm biến   | DHT22 (Nhiệt độ/độ ẩm), BH1750 (ánh sáng), GP2Y1010AU0F (bụi) |
| Nguồn vào  | 5V qua USB hoặc nguồn DC |
| Kích thước | Tuỳ theo thiết kế PCB của từng node |

---

## 🧰 Danh Sách Linh Kiện

| Tên linh kiện    | Số lượng | Ghi chú                        |
|------------------|----------|--------------------------------|
| ESP32 DevKit v1  | 3        | Mỗi node 1 chiếc               |
| SX1278 LoRa      | 3        | Mỗi node 1 chiếc               |
| DHT22            | 1        | Gắn ở sensor node              |
| BH1750           | 1        | Gắn ở sensor node              |
| GP2Y1010AU0F     | 1        | Gắn ở sensor node              |
| Điện trở 150Ω    | 1        | Cho GP2Y1010AU0F               |
| Tụ 220uF         | 1        | Cho GP2Y1010AU0F               |
| LED, nút nhấn    | Tuỳ chọn | Debug trạng thái hoặc điều khiển |
| Breadboard/dây   | Tuỳ      | Lắp mạch thử                   |

---

## 🔧 Sơ Đồ Nguyên Lý, Kết Nối và PCB

### Kết nối ESP32 với SX1278 LoRa

| SX1278 Pin | ESP32 Pin |
|------------|------------|
| VCC        | 3.3V       |
| GND        | GND        |
| SCK        | GPIO18     |
| MISO       | GPIO19     |
| MOSI       | GPIO23     |
| NSS (CS)   | GPIO5      |
| RESET      | GPIO14     |
| DIO0       | GPIO26     |

> 📌 **Lưu ý**: Tất cả các kết nối sử dụng mức điện áp 3.3V. Không cấp 5V vào SX1278.

---

### Kết nối Cảm biến với ESP32

#### DHT22

| DHT22 Pin | ESP32 Pin |
|-----------|-----------|
| VCC       | 3.3V hoặc 5V |
| GND       | GND       |
| DATA      | GPIO4     |

#### BH1750 (I2C)

| BH1750 Pin | ESP32 Pin |
|------------|-----------|
| VCC        | 3.3V      |
| GND        | GND       |
| SDA        | GPIO21    |
| SCL        | GPIO22    |

#### GP2Y1010AU0F (Cảm biến bụi)

| GP2Y1010 Pin | ESP32 Pin | Ghi chú                        |
|--------------|------------|-------------------------------|
| VLED         | 5V         | Qua điện trở 150Ω nối tiếp LED |
| LED-GND      | GND        |                               |
| LED          | GPIO15     | Điều khiển LED qua transistor nếu có |
| Vo           | GPIO34     | Đọc analog                    |
| GND          | GND        |                               |
| Vcc          | 5V         |                               |

> 📌 **Khuyến nghị**: Nối tụ 220uF giữa Vcc và GND gần module để ổn định điện áp.

---

## 🔩 Hướng Dẫn Lắp Ráp

1. Hàn các linh kiện nhỏ trước: điện trở, tụ điện
2. Gắn module SX1278, ESP32
3. Hàn cảm biến vào các chân tương ứng
4. Kiểm tra kết nối bằng đồng hồ đo
5. Cấp nguồn kiểm tra dòng tiêu thụ
6. Flash firmware từng node để kiểm tra

---

## 💻 Lập Trình Firmware

- **Ngôn ngữ:** C++ (Arduino)
- **Thư viện sử dụng:** `LoRa.h`, `DHT.h`, `BH1750.h`, `WiFi.h`, `HTTPClient.h`, `ArduinoJson`
- **Cấu trúc firmware:**

```bash
firmware/
├── sensor_node/
│   └── main.ino
├── hub_node/
│   └── main.ino
└── gateway_node/
    └── main.ino
