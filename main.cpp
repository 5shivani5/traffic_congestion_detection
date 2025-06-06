#include <WiFi.h>
#include <HTTPClient.h>
#include "LoRa_E32.h"
#include <HardwareSerial.h>

// WiFi credentials
const char* ssid = "Shivi";
const char* password = "voormvoorm";

// ✅ Local Flask server URL (Replace with your PC's IP)
const char* serverURL = "http://192.168.183.125:5000/latest";// Example: 192.168.183.125

// UART2 for LoRa
HardwareSerial mySerial(2); // RX2=GPIO16, TX2=GPIO17
LoRa_E32 e32ttl(&mySerial, 5, 18, 19); // M0=5, M1=18, AUX=19

void setup() {
    Serial.begin(115200);
    delay(1000);
    mySerial.begin(9600, SERIAL_8N1, 16, 17);  // LoRa UART

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    // Initialize LoRa
    Serial.println("Initializing LoRa module...");
    e32ttl.begin();

    Configuration configuration;
    configuration.ADDL = 0x03;
    configuration.ADDH = 0x00;
    configuration.CHAN = 0x04;
    configuration.OPTION.fec = FEC_0_OFF;
    configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
    configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
    configuration.OPTION.transmissionPower = POWER_20;
    configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
    configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
    configuration.SPED.uartBaudRate = UART_BPS_9600;
    configuration.SPED.uartParity = MODE_00_8N1;

    ResponseStatus rs = e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    Serial.println(rs.getResponseDescription());

    if (rs.code != SUCCESS) {
        Serial.println("Failed to configure LoRa module!");
        while (1) {}
    }

    Serial.println("Setup complete. Starting data fetch loop...");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);  // Fetch from local Flask server
        int httpCode = http.GET();

        if (httpCode == 200) {
            String result = http.getString();
            result.trim();
            Serial.println("Fetched from Flask: " + result);

            ResponseStatus status = e32ttl.sendMessage(result);
            if (status.code == SUCCESS) {
                Serial.println("Sent via LoRa: " + result);
            } else {
                Serial.println("LoRa send failed: " + status.getResponseDescription());
            }
        } else {
            Serial.printf("HTTP GET failed, code: %d\n", httpCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected. Reconnecting...");
        WiFi.reconnect();
    }

    delay(10000); // Fetch every 30 seconds
}
