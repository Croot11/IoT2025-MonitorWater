#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define RXD2 16  // Chân RX của UART2
#define TXD2 17  // Chân TX của UART2
#define PUMP1_PIN 5  // Chân GPIO để điều khiển relay bơm 1
#define PUMP2_PIN 18 // Chân GPIO để điều khiển relay bơm 2
#define BUTTON_PIN 19 // Chân GPIO cho nút bấm (giả sử GPIO 19)
#define P1 25 //chân GPIO để điều khiển bơm 1
#define P2 26 //chân GPIO để điều khiển bơm 2

const char* ssid = "Labee";
const char* password = "labmitsu";
const char* serverUrl = "https://duongxuanngoc.top/main/api";  // URL API để gửi dữ liệu phần trăm nước
const char* tankApiUrl = "https://duongxuanngoc.top/main/api_infortank";  // URL API để lấy thông số bể
const char* pumpApiUrl = "https://duongxuanngoc.top/main/api_pump";  // URL API để lấy trạng thái bơm
const char* checkConnectApiUrl = "https://duongxuanngoc.top/main/api_checkconnect/";  // URL API để báo trạng thái kết nối
const char* changePump = "http://duongxuanngoc.top/toggle-pump/";
const char* urlconnectesp = "https://duongxuanngoc.top/api/esp_heartbeat"; 

unsigned long timeUpdate = 0;
unsigned long lastPostTime = 0;
unsigned long lastPumpUpdateTime = 0;

// Biến để kiểm tra kết nối
unsigned long lastDataTimeTB1 = 0;  // Thời điểm nhận dữ liệu cuối cùng của TB1
unsigned long lastDataTimeTB2 = 0;  // Thời điểm nhận dữ liệu cuối cùng của TB2
bool isConnectedTB1 = true;  // Trạng thái kết nối của TB1
bool isConnectedTB2 = true;  // Trạng thái kết nối của TB2
const unsigned long connectionTimeout = 20000;  // Thời gian timeout (20 giây)

// Lưu dữ liệu từ UART
int h_TB1 = 0;  // Khoảng cách từ miệng bể xuống mặt nước của TB1 (cm)
int water_quality_TB1 = 0;  // water_quality của TB1 (từ CL)
int h_TB2 = 0;  // Khoảng cách từ miệng bể xuống mặt nước của TB2 (cm)

// Lưu dữ liệu từ API
int depth_TB1 = 0, capacity_TB1 = 0;  // depth (m), capacity (lít) của TB1
int depth_TB2 = 0, capacity_TB2 = 0;  // depth (m), capacity (lít) của TB2

// Phần trăm nước tính toán
int water_percentage_TB1 = 0;  // Phần trăm nước của TB1
int water_percentage_TB2 = 0;  // Phần trăm nước của TB2
int capacity_p1 = 0;
int capacity_p2 = 0;

bool buttonPressed = false;  // Trạng thái nút bấm
bool buttonPressed_p1 = false;  // Trạng thái nút bấm
bool buttonPressed_p2 = false;  // Trạng thái nút bấm

void setup_wifi() {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

// Hàm đọc dữ liệu từ Serial2
void readUARTData() {
    if (Serial2.available()) {
        String receivedData = Serial2.readStringUntil('\n');  // Đọc đến khi gặp ký tự xuống dòng
        receivedData.trim();  // Xóa khoảng trắng thừa

        StaticJsonDocument<200> jsonDoc;
        DeserializationError error = deserializeJson(jsonDoc, receivedData);

        if (!error) {
            // Kiểm tra nếu có TB1 (khoảng cách h) và CL (water_quality)
            if (jsonDoc.containsKey("TB1")) {
                h_TB1 = jsonDoc["TB1"].as<int>();  // Lưu khoảng cách h của TB1 (cm)
                Serial.println("Updated TB1: h=" + String(h_TB1) + " cm");
                lastDataTimeTB1 = millis();  // Cập nhật thời điểm nhận dữ liệu của TB1
                if (!isConnectedTB1) {
                    isConnectedTB1 = true;
                    reportConnectionStatus(1, true);  // Báo trạng thái kết nối của TB1
                }
            }
            if (jsonDoc.containsKey("CL")) {
                water_quality_TB1 = jsonDoc["CL"].as<int>();  // Lưu water_quality của TB1
                Serial.println("Updated TB1: water_quality=" + String(water_quality_TB1));
                lastDataTimeTB1 = millis();  // Cập nhật thời điểm nhận dữ liệu của TB1
                if (!isConnectedTB1) {
                    isConnectedTB1 = true;
                    reportConnectionStatus(1, true);  // Báo trạng thái kết nối của TB1
                }
            }
            // Kiểm tra nếu có TB2 (khoảng cách h)
            if (jsonDoc.containsKey("TB2")) {
                h_TB2 = jsonDoc["TB2"].as<int>();  // Lưu khoảng cách h của TB2 (cm)
                Serial.println("Updated TB2: h=" + String(h_TB2) + " cm");
                lastDataTimeTB2 = millis();  // Cập nhật thời điểm nhận dữ liệu của TB2
                if (!isConnectedTB2) {
                    isConnectedTB2 = true;
                    reportConnectionStatus(2, true);  // Báo trạng thái kết nối của TB2
                }
            }

            // Tính toán lại phần trăm nước sau khi nhận dữ liệu mới từ UART
            calculateWaterPercentage();
        } else {
            Serial.println("JSON Parsing Error!");
        }
    }
}

// Hàm kiểm tra kết nối của TB1 và TB2
void checkConnection() {
    unsigned long currentTime = millis();

    // Kiểm tra TB1
    if (currentTime - lastDataTimeTB1 > connectionTimeout) {
        if (isConnectedTB1) {  // Nếu trước đó đang kết nối
            isConnectedTB1 = false;
            Serial.println("TB1: Connection Lost!");
            reportConnectionStatus(1, false);  // Báo trạng thái mất kết nối của TB1
        }
    }

    // Kiểm tra TB2
    if (currentTime - lastDataTimeTB2 > connectionTimeout) {
        if (isConnectedTB2) {  // Nếu trước đó đang kết nối
            isConnectedTB2 = false;
            Serial.println("TB2: Connection Lost!");
            reportConnectionStatus(2, false);  // Báo trạng thái mất kết nối của TB2
        }
    }
}

// Hàm báo trạng thái kết nối lên API
void reportConnectionStatus(int device_id, bool status) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(checkConnectApiUrl) + String(device_id);  // Tạo URL: api_checkconnect/1 hoặc api_checkconnect/2
        http.begin(url);
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> jsonDoc;
        jsonDoc["status"] = status;  // true: kết nối, false: mất kết nối
        String jsonData;
        serializeJson(jsonDoc, jsonData);

        int httpResponseCode = http.POST(jsonData);  // Gửi POST thay vì PUT
        Serial.print("Device " + String(device_id) + " - Connection Status Update - Status Code: ");
        Serial.println(httpResponseCode);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("Device " + String(device_id) + " - Response: ");
            Serial.println(response);
        } else {
            Serial.println("Failed to report connection status for Device " + String(device_id));
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected! Cannot report connection status.");
    }
}

// Hàm lấy thông số bể từ API
void fetchTankParameters() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(tankApiUrl);  // Sử dụng API mới để lấy thông số bể
        int httpResponseCode = http.GET();

        if (httpResponseCode == 200) {
            String payload = http.getString();
            Serial.println("Tank API Response: " + payload);

            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                // Lấy thông số cho TB1 (device_id = 1) và TB2 (device_id = 2)
                for (JsonObject device : doc.as<JsonArray>()) {
                    int device_id = device["device_id"];
                    if (device_id == 1) {
                        depth_TB1 = device["depth"];  // depth (m)
                        capacity_TB1 = device["capacity"];  // capacity (lít)
                        Serial.println("TB1 Parameters: depth=" + String(depth_TB1) + " m, capacity=" + String(capacity_TB1) + " lít");
                    }
                    if (device_id == 2) {
                        depth_TB2 = device["depth"];  // depth (m)
                        capacity_TB2 = device["capacity"];  // capacity (lít)
                        Serial.println("TB2 Parameters: depth=" + String(depth_TB2) + " m, capacity=" + String(capacity_TB2) + " lít");
                    }
                }

                // Tính toán lại phần trăm nước sau khi cập nhật thông số bể
                calculateWaterPercentage();
            } else {
                Serial.println("Tank JSON Parsing Error!");
            }
        } else {
            Serial.printf("HTTP GET Request for Tank Parameters Failed! Error Code: %d\n", httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
}

// Hàm tính toán phần trăm nước
void calculateWaterPercentage() {
    // Tính cho TB1
    if (depth_TB1 != 0) {
        float depth_cm_TB1 = depth_TB1 * 100;  // Chuyển depth từ mét sang cm
        float water_height_TB1 = depth_cm_TB1 - h_TB1;  // Độ cao nước thực tế (cm)
        water_percentage_TB1 = (water_height_TB1 / depth_cm_TB1) * 100;  // Phần trăm nước
        capacity_p1 = capacity_TB1*(water_percentage_TB1);
        if (water_percentage_TB1 < 0) water_percentage_TB1 = 0;  // Đảm bảo không âm
        if (water_percentage_TB1 > 100) water_percentage_TB1 = 100;  // Đảm bảo không vượt 100
        Serial.println("TB1: Water Percentage = " + String(water_percentage_TB1) + "%  " + String(capacity_p1) +"    "+ String(capacity_TB1));
    }

    // Tính cho TB2
    if (depth_TB2 != 0) {
        float depth_cm_TB2 = depth_TB2 * 100;  // Chuyển depth từ mét sang cm
        float water_height_TB2 = depth_cm_TB2 - h_TB2;  // Độ cao nước thực tế (cm)
        water_percentage_TB2 = (water_height_TB2 / depth_cm_TB2) * 100;  // Phần trăm nước
        capacity_p2 = capacity_TB2*(water_percentage_TB2/100);
        if (water_percentage_TB2 < 0) water_percentage_TB2 = 0;  // Đảm bảo không âm
        if (water_percentage_TB2 > 100) water_percentage_TB2 = 100;  // Đảm bảo không vượt 100
        Serial.println("TB2: Water Percentage = " + String(water_percentage_TB2) + "%  " + String(capacity_p2));
    }
}

// Hàm gửi dữ liệu lên server qua HTTP POST
void sendDataToServer() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);  // Gửi dữ liệu lên serverUrl
        http.addHeader("Content-Type", "application/json");

        // Gửi dữ liệu của TB1
        if (water_percentage_TB1 != 0 || water_quality_TB1 != 0) {  // Chỉ gửi nếu có dữ liệu
            StaticJsonDocument<200> jsonDocTB1;
            jsonDocTB1["device_id"] = 1;  // TB1 có device_id = 1
            jsonDocTB1["water_level"] = water_percentage_TB1;  // Gửi phần trăm nước
            jsonDocTB1["water_quality"] = water_quality_TB1;  // TB1 có water_quality
            jsonDocTB1["capacity"] = capacity_p1;  // TB1 có water_quality

            String jsonDataTB1;
            serializeJson(jsonDocTB1, jsonDataTB1);

            int httpResponseCode = http.POST(jsonDataTB1);
            Serial.print("TB1 - Status Code: ");
            Serial.println(httpResponseCode);

            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.print("TB1 - Response: ");
                Serial.println(response);
            }
        }

        // Gửi dữ liệu của TB2
        if (water_percentage_TB2 != 0) {  // Chỉ gửi nếu có dữ liệu
            StaticJsonDocument<200> jsonDocTB2;
            jsonDocTB2["device_id"] = 2;  // TB2 có device_id = 2
            jsonDocTB2["water_level"] = water_percentage_TB2;  // Gửi phần trăm nước
            jsonDocTB2["water_quality"] = 0;  // TB2 không có water_quality
            jsonDocTB2["capacity"] = capacity_p2;

            String jsonDataTB2;
            serializeJson(jsonDocTB2, jsonDataTB2);

            int httpResponseCode = http.POST(jsonDataTB2);
            Serial.print("TB2 - Status Code: ");
            Serial.println(httpResponseCode);

            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.print("TB2 - Response: ");
                Serial.println(response);
            }
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }
}

// Hàm lấy trạng thái bơm từ API và điều khiển bơm
void updatePumpStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(pumpApiUrl);
        int httpResponseCode = http.GET();

        if (httpResponseCode == 200) {
            String payload = http.getString();
            Serial.println("Pump API Response: " + payload);

            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                int pump1_status = !doc[0]["status"]; // Trạng thái bơm 1
                int pump2_status = doc[1]["status"]; // Trạng thái bơm 2

                digitalWrite(PUMP1_PIN, pump1_status);
                digitalWrite(PUMP2_PIN, pump2_status);

                Serial.printf("Pump 1: %s\n", pump1_status ? "ON" : "OFF");
                Serial.printf("Pump 2: %s\n", pump2_status ? "ON" : "OFF");
            } else {
                Serial.println("Pump JSON Parsing Error!");
            }
        } else {
            Serial.printf("HTTP GET Request for Pump Failed! Error Code: %d\n", httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
}

void controlPump(int pump){
  if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(changePump) + String(pump);
        http.begin(url);  
        http.addHeader("Content-Type", "application/json");  // Header HTTP

        int httpResponseCode = http.PUT("");  // Gửi yêu cầu HTTP PUT

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Phản hồi từ server:");
            Serial.println(response);
        } else {
            Serial.print("Lỗi HTTP: ");
            Serial.println(httpResponseCode);
        }

        http.end();  // Đóng kết nối HTTP
    } else {
        Serial.println("WiFi chưa kết nối!");
    }  
}

void connectesp(){
  if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin(urlconnectesp);
        http.addHeader("Content-Type", "application/json"); // Định dạng JSON

        // Tạo JSON data
        StaticJsonDocument<200> doc;
        doc["esp_id"] = 1;

        String jsonData;
        serializeJson(doc, jsonData);

        // Gửi POST request
        int httpResponseCode = http.POST(jsonData);

        // Kiểm tra phản hồi
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Dữ liệu đã gửi thành công!");
            Serial.println("Phản hồi từ server:");
            Serial.println(response);
        } else {
            Serial.print("Lỗi khi gửi dữ liệu. HTTP Response code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("⚠ WiFi chưa kết nối!");
    }
}
void setup() {
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

    // Cấu hình chân điều khiển bơm
    pinMode(PUMP1_PIN, OUTPUT);
    pinMode(PUMP2_PIN, OUTPUT);
    digitalWrite(PUMP1_PIN, LOW);  // Tắt bơm 1 ban đầu
    digitalWrite(PUMP2_PIN, LOW);  // Tắt bơm 2 ban đầu
    
    // Cấu hình chân nút bấm
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Sử dụng pull-up để tránh nhiễu
    pinMode(P1, INPUT_PULLUP);
    pinMode(P2, INPUT_PULLUP);

    setup_wifi();

    // Lấy thông số bể lần đầu tiên khi khởi động
    fetchTankParameters();
}

void loop() {
    // Cập nhật dữ liệu từ UART mỗi 1 giây
    if (millis() - timeUpdate > 1000) {
        readUARTData();  // Đọc dữ liệu từ Serial2 và tính toán lại phần trăm nước
        checkConnection();  // Kiểm tra kết nối của TB1 và TB2
        connectesp();
        timeUpdate = millis();
    }

    // Kiểm tra nút bấm để cập nhật thông số bể
    if (digitalRead(BUTTON_PIN) == LOW) {  // Nút bấm được nhấn (LOW vì dùng INPUT_PULLUP)
        if (!buttonPressed) {  // Chỉ xử lý khi nút vừa được nhấn (tránh lặp lại)
            buttonPressed = true;
            Serial.println("Button Pressed! Fetching tank parameters...");
            fetchTankParameters();  // Lấy thông số bể từ API
        }
    } else {
        buttonPressed = false;  // Reset trạng thái khi thả nút
    }
    if (digitalRead(P1) == LOW) {  // Nút bấm được nhấn (LOW vì dùng INPUT_PULLUP)
        if (!buttonPressed_p1) {  // Chỉ xử lý khi nút vừa được nhấn (tránh lặp lại)
            buttonPressed_p1 = true;
            Serial.println("Button Pressed! change status...");
            controlPump(1);  // thay đổi trạng thái từ API
        }
    } else {
        buttonPressed_p1 = false;  // Reset trạng thái khi thả nút
    }
    if (digitalRead(P2) == LOW) {  // Nút bấm được nhấn (LOW vì dùng INPUT_PULLUP)
        if (!buttonPressed_p2) {  // Chỉ xử lý khi nút vừa được nhấn (tránh lặp lại)
            buttonPressed_p2 = true;
            Serial.println("Button Pressed! change status...");
            controlPump(2);  // thay đổi trạng thái từ API
        }
    } else {
        buttonPressed_p2 = false;  // Reset trạng thái khi thả nút
    }
    // Gửi dữ liệu lên server mỗi 60 giây
    if (millis() - lastPostTime > 6000) {
        sendDataToServer();
        lastPostTime = millis();
    }

    // Cập nhật trạng thái bơm mỗi 5 giây
    if (millis() - lastPumpUpdateTime > 5000) {
        updatePumpStatus();
        lastPumpUpdateTime = millis();
    }
}