#include <WiFi.h>

// Wi-Fi 网络配置
const char* ssid = "spotpear_Test";         // 替换为你的 Wi-Fi SSID  ChinaNet-JTKB
const char* password = "12345678"; // 替换为你的 Wi-Fi 密码 a3w54htk

void setup() {
  Serial.begin(115200);

  // 连接到 Wi-Fi 网络
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // 等待连接完成
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // 连接成功
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 获取当前 Wi-Fi 信号强度 (RSSI)
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  // 分析信号质量
  if (rssi >= -50) {
    Serial.println("Excellent signal");
  } else if (rssi >= -70) {
    Serial.println("Good signal");
  } else if (rssi >= -80) {
    Serial.println("Weak signal");
  } else {
    Serial.println("Very weak signal");
  }

  // 延迟 5 秒
  delay(1000);
}
