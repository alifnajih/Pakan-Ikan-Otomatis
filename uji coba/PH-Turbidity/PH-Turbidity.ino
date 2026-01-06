#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD dengan alamat 0x27 dan ukuran 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin untuk sensor
const int pH_pin = A1;
const int turbidity_pin = A0;

// Variabel untuk menyimpan nilai sensor
float pHValue = 0.0;
float turbidityValue = 0.0;

void setup() {
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Inisialisasi Serial Monitor untuk debugging
  Serial.begin(9600);

  // Tampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Monitoring pH & Turb");
  lcd.setCursor(0, 1);
  lcd.print("pH:");
  lcd.setCursor(0, 2);
  lcd.print("Turbidity:");
}

void loop() {
  // Baca nilai analog dari sensor pH
  int pH_adc = analogRead(pH_pin);
  // Konversi nilai ADC ke pH
  pHValue = (pH_adc * 5.0 / 1024) * 3.5; // Skala kalibrasi pH (sesuaikan)

  // Baca nilai analog dari sensor turbidity
  int turb_adc = analogRead(turbidity_pin);
  // Konversi nilai ADC ke kekeruhan (sesuaikan dengan data sheet sensor)
  turbidityValue = turb_adc * (5.0 / 1024) * 100; // Skala kekeruhan

  // Tampilkan nilai pH di LCD
  lcd.setCursor(4, 1);
  lcd.print(pHValue, 2); // Tampilkan dengan 2 desimal
  lcd.print("      "); // Hapus karakter yang tersisa

  // Tampilkan nilai turbidity di LCD
  lcd.setCursor(10, 2);
  lcd.print(turbidityValue, 2); // Tampilkan dengan 2 desimal
  lcd.print("      "); // Hapus karakter yang tersisa

  // Debugging: Cetak nilai sensor ke Serial Monitor
  Serial.print("pH: ");
  Serial.print(pHValue, 2);
  Serial.print("  Turbidity: ");
  Serial.println(turbidityValue, 2);

  // Tunggu 1 detik sebelum pembacaan berikutnya
  delay(1000);
}
