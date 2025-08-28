#include <WiFi.h>

const char* ssid = "ESP32S3_Test";
const char* password = "12345678";  // 设置 Wi-Fi 密码

void setup() {
    Serial.begin(115200);

    // 配置 AP 模式
    WiFi.softAP(ssid, password);

    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
}

void loop() {
    // 保持 AP 模式运行
    delay(1000);
}
