#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Inisialisasi LCD dengan alamat 0x27 dan ukuran 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin untuk sensor DS18B20
const int oneWireBus = 8;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

float temperatureC;

void setup() {
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Inisialisasi Serial Monitor
  Serial.begin(9600);

  // Inisialisasi sensor DS18B20
  sensors.begin();

  // Tampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Monitoring Suhu");
  lcd.setCursor(0, 1);
  lcd.print("Suhu (C):");
}

void loop() {
  // Minta sensor untuk melakukan pembacaan suhu
  sensors.requestTemperatures();
  // Simpan nilai suhu dalam Celsius
  temperatureC = sensors.getTempCByIndex(0);

  // Tampilkan nilai suhu di LCD
  lcd.setCursor(10, 1);
  lcd.print(temperatureC, 2); // Tampilkan dengan 2 desimal
  lcd.print("     "); // Hapus karakter yang tersisa

  // Tampilkan nilai suhu di Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(temperatureC, 2);
  Serial.println(" C");

  // Tunggu 1 detik sebelum pembacaan berikutnya
  delay(1000);
}
