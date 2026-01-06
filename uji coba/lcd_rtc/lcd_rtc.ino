#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// Inisialisasi LCD I2C dengan alamat 0x27 dan ukuran 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Inisialisasi RTC DS3231
RTC_DS3231 rtc;

void setup() {
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Inisialisasi komunikasi I2C dengan RTC
  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC Tidak Terhubung");
    while (1); // Berhenti jika RTC tidak ditemukan
  }
  
  // Cek apakah RTC berjalan dengan baik
  if (rtc.lostPower()) {
    lcd.setCursor(0, 0);
    lcd.print("Mengatur Waktu RTC");
    // Set waktu awal RTC ke waktu compile jika RTC kehilangan daya
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now(); // Baca waktu dari RTC
  
  // Tampilkan waktu (jam:menit:detik) di baris pertama
  lcd.setCursor(0, 0);
  lcd.print("Waktu: ");
  if (now.hour() < 10) lcd.print('0'); // Tambahkan 0 jika jam kurang dari 10
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10) lcd.print('0');
  lcd.print(now.second());

  // Tampilkan tanggal (DD/MM/YYYY) di baris kedua
  lcd.setCursor(0, 1);
  lcd.print("Tanggal: ");
  if (now.day() < 10) lcd.print('0');
  lcd.print(now.day());
  lcd.print("/");
  if (now.month() < 10) lcd.print('0');
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());

  // Tampilkan hari di baris ketiga
  lcd.setCursor(0, 2);
  lcd.print("Hari: ");
  lcd.print(getDayOfWeek(now.dayOfTheWeek()));

  delay(1000); // Update tampilan setiap 1 detik
}

// Fungsi untuk mendapatkan nama hari dalam seminggu
String getDayOfWeek(int day) {
  switch (day) {
    case 0: return "Minggu";
    case 1: return "Senin";
    case 2: return "Selasa";
    case 3: return "Rabu";
    case 4: return "Kamis";
    case 5: return "Jumat";
    case 6: return "Sabtu";
    default: return "";
  }
}
