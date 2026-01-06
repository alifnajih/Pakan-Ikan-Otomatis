#define BLYNK_TEMPLATE_ID "TMPL6eYpmgSh9"
#define BLYNK_TEMPLATE_NAME "Control Mesin"
#define BLYNK_AUTH_TOKEN "yWrRXIkGpGI65y7HPDeKc9XiFlg7QndM"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiManager.h>  // Library untuk WiFiManager
#include <Ticker.h>

// Blynk Auth Token
char auth[] = "yWrRXIkGpGI65y7HPDeKc9XiFlg7QndM";

// Pin untuk SSR dan tombol reset
#define SSR_PIN D8
#define RESET_PIN D7

WiFiManager wifiManager;
Ticker ticker;  // Timer untuk blink indikator

// Fungsi untuk blink LED saat sedang konfigurasi Wi-Fi
void blinkLED() {
  digitalWrite(SSR_PIN, !digitalRead(SSR_PIN));
}

// Fungsi setup
void setup() {
  Serial.begin(115200);

  // Set pin mode
  pinMode(SSR_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);
  digitalWrite(SSR_PIN, LOW);  // Awal SSR dalam keadaan mati

  // Jika tombol reset ditekan, masuk ke WiFi Manager mode
  if (digitalRead(RESET_PIN) == LOW) {
    Serial.println("Reset Wi-Fi settings...");
    wifiManager.resetSettings();
  }

  // Mulai WiFi Manager
  if (!wifiManager.autoConnect("ESP8266_Config")) {
    Serial.println("Failed to connect, restarting...");
    ESP.restart();
  }

  Serial.println("Connected to Wi-Fi");

  // Inisialisasi Blynk
  Blynk.config(auth);
  Blynk.connect();

  // Setup ticker untuk blink indikator selama setup Wi-Fi
  ticker.attach(0.5, blinkLED);
}

// Fungsi utama loop
void loop() {
  Blynk.run();  // Menjalankan Blynk

  // Jika tombol fisik ditekan, reset Wi-Fi
  if (digitalRead(RESET_PIN) == LOW) {
    Serial.println("Tombol reset ditekan, mengatur ulang Wi-Fi...");
    wifiManager.resetSettings();
    ESP.restart();
  }
}

// Fungsi untuk menerima kontrol dari aplikasi Blynk (Virtual Pin V1)
BLYNK_WRITE(V1) {
  int state = param.asInt();  // Membaca nilai dari Blynk
  digitalWrite(SSR_PIN, state);  // Mengontrol SSR

  // Menambahkan notifikasi di Serial Monitor
  if (state == 1) {
    Serial.println("SSR dihidupkan melalui Blynk.");
  } else {
    Serial.println("SSR dimatikan melalui Blynk.");
  }
}
