# 🔗 ESP32 LoRa Mesh - Sensor Network with Google Sheets Logging

Dự án triển khai mạng **LoRa Mesh** gồm 3 node ESP32 sử dụng module **SX1278**, truyền dữ liệu cảm biến qua giao thức **flooding**. Hệ thống bao gồm:

- 📡 **Sensor Node**: Đọc dữ liệu từ các cảm biến (DHT22, BH1750, GY-101).
- 🧠 **Hub Node**: Đóng vai trò trung gian, forward gói tin trong mạng mesh.
- ☁️ **Gateway Node**: Nhận dữ liệu và đẩy lên **Google Sheets**.

---

## 🔧 Phần cứng sử dụng

| Thành phần        | Mô tả                          |
|------------------|--------------------------------|
| ESP32            | Vi điều khiển chính            |
| SX1278           | Module LoRa truyền thông       |
| DHT22            | Cảm biến nhiệt độ/độ ẩm        |
| BH1750           | Cảm biến ánh sáng              |
| GY-101 (G1)      | Cảm biến bụi PM                |
| Nguồn 5V hoặc pin| Tuỳ thiết kế di động/cố định   |

---

## 🌐 Kiến trúc hệ thống

text
[SENSOR NODE]
   └──> [HUB NODE] ──> [GATEWAY NODE] ──> Google Sheets

- Flooding algorithm được dùng để đảm bảo truyền tin qua nhiều hops.
- Mỗi gói dữ liệu có định danh (ID) để tránh lặp lại.