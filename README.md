# 🔌 ESP32 LoRa Mesh Sensor Network

Mô tả ngắn gọn: Hệ thống thu thập dữ liệu môi trường sử dụng mạng **LoRa Mesh** gồm 3 node ESP32, truyền dữ liệu cảm biến đến Google Sheets. Ứng dụng trong giám sát môi trường từ xa, học tập và nghiên cứu mạng không dây.

---

## 📑 Mục Lục

- [👋 Giới Thiệu](#-giới-thiệu)  
- [📐 Thông Số Kỹ Thuật](#-thông-số-kỹ-thuật)  
- [🧰 Danh Sách Linh Kiện](#-danh-sách-linh-kiện)  
- [🔧 Sơ Đồ Nguyên Lý và PCB](#-sơ-đồ-nguyên-lý-và-pcb)  
- [🔩 Hướng Dẫn Lắp Ráp](#-hướng-dẫn-lắp-ráp)  
- [💻 Lập Trình Firmware](#-lập-trình-firmware)  
- [▶️ Cách Sử Dụng](#️-cách-sử-dụng)  
- [🧪 Kiểm Thử](#-kiểm-thử)  
- [🎥 Ảnh/Video Demo](#-ảnhvideo-demo)  
- [🤝 Đóng Góp](#-đóng-góp)  
- [📜 Giấy Phép](#-giấy-phép)

---

## 👋 Giới Thiệu

Dự án xây dựng mạng cảm biến không dây sử dụng **ESP32 + SX1278** theo topology **mesh (flooding)** gồm:

- 1 **Sensor Node**: thu thập dữ liệu từ DHT22, BH1750, GY-101
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
| Cảm biến   | DHT22 (Nhiệt độ/độ ẩm), BH1750 (ánh sáng), GY-101 (bụi) |
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
| GY-101 (G1)      | 1        | Gắn ở sensor node              |
| LED, nút nhấn    | Tuỳ chọn | Debug trạng thái hoặc điều khiển |
| Breadboard/dây   | Tuỳ      | Lắp mạch thử                   |

👉 *[Link đến file BOM.csv đầy đủ nếu có]*

---

## 🔧 Sơ Đồ Nguyên Lý và PCB

📎 [Schematic (PDF)](link_tới_file.pdf)  
📎 [PCB Layout (Gerber)](link_tới_gerber.zip)  
📎 [File thiết kế (KiCad)](link_tới_file_kicad.zip)

![Schematic Preview](docs/schematic_preview.png)

---

## 🔩 Hướng Dẫn Lắp Ráp

1. Hàn các linh kiện nhỏ trước: điện trở, tụ điện
2. Gắn module SX1278, ESP32
3. Hàn cảm biến vào các chân tương ứng
4. Kiểm tra kết nối bằng đồng hồ đo
5. Cấp nguồn kiểm tra dòng tiêu thụ
6. Flash firmware từng node để kiểm tra

📎 *[Xem video hướng dẫn lắp ráp tại đây](link_tới_video)*

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
