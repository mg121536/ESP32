/******************************************************************************
 *  File: IPS2550_WebApp.ino
 *  Description: Reads analog signals (sin and cos) from rotary encoder and
 *               calculates angle of rotation in degrees.
 *  Target: ESP32-S3-DEV-KIT-N16R8-M
 *  Date: 2025/XX/XX
 *  License: MinebeaMitsumi Inc.
 ******************************************************************************/

 #include <Arduino.h>
 #include <IPS2550_WebApp.h>
 #include <IPS2550_WebApp_Cfg.h>
 
 #if ESP32_SERIAL
 #include <IPS2550_WebApp_Serial.h>
 #endif
 
 #if ESP32_BLUETOOTH
 #include <IPS2550_WebApp_Bluetooth.h>
 #endif
 
 #if ESP32_WIFI
 #include <IPS2550_WebApp_Wifi.h>
 #endif
 
 #if ESP32_DEBUG
 #include <math.h>
 #endif
 
 #if 0
 #include "driver/adc.h"
 #include "esp_adc_cal.h"
 #endif
 
 // GPIO pins for analog input (COS and SIN)
 const int A_pin = ESP32_CFG_PORT_GPIO_COS;
 const int B_pin = ESP32_CFG_PORT_GPIO_SIN;
 
 // Analog values
 int A_val = 0U;  // COS
 int B_val = 0U;  // SIN
 
 // Timer for non-blocking sensor read
 unsigned long lastSensorReadTime = 0;
 const unsigned long sensorInterval = 1;  // ms
 
 #if ESP32_DEBUG
 int A_val_max = 0U;
 int B_val_max = 0U;
 int A_val_min = 4095U;
 int B_val_min = 4095U;
 
 float angle = 0.0;
 float pre_angle = 0.0;
 float rota_dif_angle = 0.0;
 float mult_angle = 0.0;
 float Angle = 0.0;
 #endif
 
 #if 0
 esp_adc_cal_characteristics_t adc_chars;
 #endif
 
 /******************************************************************************
  * Bluetooth
  ******************************************************************************/
 #if ESP32_BLUETOOTH
 
 BLECharacteristic* pCharacteristic;
 bool deviceConnected = false;
 
 // BLE Server callbacks
 class MyServerCallbacks : public BLEServerCallbacks {
     void onConnect(BLEServer* pServer) override {
         deviceConnected = true;
         Serial.println("Client connected");
     }
 
     void onDisconnect(BLEServer* pServer) override {
         deviceConnected = false;
         Serial.println("Client disconnected");
     }
 };
 
 // BLE initialization
 void Ble_init() {
     BLEDevice::init("ESP32_BLE_Device");
 
     BLEServer* pServer = BLEDevice::createServer();
     pServer->setCallbacks(new MyServerCallbacks());
 
     BLEService* pService = pServer->createService(SERVICE_UUID);
 
     pCharacteristic = pService->createCharacteristic(
         CHARACTERISTIC_UUID,
         BLECharacteristic::PROPERTY_READ |
         BLECharacteristic::PROPERTY_WRITE |
         BLECharacteristic::PROPERTY_NOTIFY
     );
 
     pCharacteristic->setValue("0");
     pCharacteristic->addDescriptor(new BLE2902());
 
     pService->start();
 
     BLEAdvertising* pAdvertising = pServer->getAdvertising();
     pAdvertising->start();
 }
 
 #endif /* ESP32_BLUETOOTH */
 
 /******************************************************************************
  * Wi-Fi
  ******************************************************************************/
 #if ESP32_WIFI
 
 const char* ssid = "ESP32_AP";
 const char* password = "12345678";
 
 WebServer server(80);
 WebSocketsServer webSocket = WebSocketsServer(81);
 
 // HTML page for WebSocket
 const char* htmlPage = R"rawliteral(
 <!DOCTYPE html>
 <html>
 <body style="background:black; color:white;">
 <h2>ESP32 WebSocket Example</h2>
 <script>
   let ws = new WebSocket("ws://" + location.hostname + ":81/");
   ws.onmessage = function(event) {
     console.log("Received: " + event.data);
   };
   ws.onopen = () => ws.send("Hello from iPhone!");
 </script>
 </body>
 </html>
 )rawliteral";
 
 // WebSocket event handler
 void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t* payload, size_t length) {
     if (type == WStype_TEXT) {
         Serial.printf("[%u] get Text: %s\n", client_num, payload);
         webSocket.sendTXT(client_num, "Hello from ESP32");
     }
 }
 
 // Wi-Fi initialization
 void Wifi_init() {
     WiFi.softAP(ssid, password);
     Serial.print("AP IP address: ");
     Serial.println(WiFi.softAPIP());
 
     server.on("/", []() {
         server.send(200, "text/html", htmlPage);
     });
 
     server.begin();
     webSocket.begin();
     webSocket.onEvent(onWebSocketEvent);
 }
 
 #endif /* ESP32_WIFI */
 
 /******************************************************************************
  * Setup
  ******************************************************************************/
 void setup() {
     delay(ESP32_CFG_SETUP_DELAY);
     Serial.begin(ESP32_CFG_SERIAL_BAUDRATE);
     analogReadResolution(12);  // 12-bit ADC
 
 #if ESP32_BLUETOOTH
     Ble_init();
 #endif
 
 #if ESP32_WIFI
     Wifi_init();
 #endif
 }
 
 /******************************************************************************
  * Main Loop
  ******************************************************************************/
 void loop() {
     unsigned long now = millis();  // ← 毎ループで更新！
 
 #if ESP32_WIFI
     server.handleClient();
     webSocket.loop();
 #endif
 
     // 非ブロッキングなセンサ読み取り処理
     if (now - lastSensorReadTime >= sensorInterval) {
         lastSensorReadTime = now;
 
         A_val = analogRead(A_pin);
         B_val = analogRead(B_pin);
 
 #if ESP32_DEBUG
         // デバッグ用の最小・最大記録
         A_val_max = max(A_val_max, A_val);
         A_val_min = min(A_val_min, A_val);
         B_val_max = max(B_val_max, B_val);
         B_val_min = min(B_val_min, B_val);
 
         // アナログ値から角度を計算（度単位）
         angle = atan2((float)(B_val - VAL_ANALOG_RANGE_MID), (float)(A_val - VAL_ANALOG_RANGE_MID));
         angle = angle * 180.0 / PI;
 
         // 回転差分を計算（±180度のまたぎ考慮）
         if ((angle > 0) && (angle < 45) && (pre_angle > 135) && (pre_angle < 180)) {
             rota_dif_angle = angle + (180 - pre_angle);
         } else if ((angle > 135) && (angle < 180) && (pre_angle > 0) && (pre_angle < 45)) {
             rota_dif_angle = -angle - (180 - pre_angle);
         } else if (angle > pre_angle) {
             rota_dif_angle = angle - pre_angle;
         } else if (angle < pre_angle) {
             rota_dif_angle = pre_angle - angle;
         } else {
             rota_dif_angle = 0;
         }
 
         // 回転角度を累積・正規化
         mult_angle += rota_dif_angle;
         pre_angle = angle;
 
         if (mult_angle > 640) {
             mult_angle -= 640;
         } else if (mult_angle < 0) {
             mult_angle += 180;
         }
 
         Angle = mult_angle / 4;  // 最終的なスケーリング角度
 
         // デバッグ出力
         debugLog("A_max:");
         debugLog(A_val_max);
         debugLog(", A_min:");
         debugLog(A_val_min);
         debugLog(", B_max:");
         debugLog(B_val_max);
         debugLog(", B_min:");
         debugLog(B_val_min);
         debugLog(", Angle:");
         debugLog(Angle);
 #endif
 
 #if ESP32_BLUETOOTH
         // BLE送信
         if (deviceConnected) {
             String sendStr = "A:" + String(A_val) + " B:" + String(B_val) + "\n";
             pCharacteristic->setValue(sendStr.c_str());
             pCharacteristic->notify();
         }
 #endif
 
 #if ESP32_WIFI
         // WebSocket送信
         String dataStr = "A:" + String(A_val) + " B:" + String(B_val);
         webSocket.broadcastTXT(dataStr);
 #endif
 
         // シリアル出力
         Serial.print("A:");
         Serial.print(A_val);
         Serial.print(" B:");
         Serial.println(B_val);
     }
 }
 
 /******************************************************************************
  * Debug logging (only when ESP32_DEBUG is enabled)
  ******************************************************************************/
 void debugLog(const String& msg) {
 #if ESP32_DEBUG
     Serial.println(msg);
 #endif
 }
 