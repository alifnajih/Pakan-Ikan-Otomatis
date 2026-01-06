#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Servo.h>

// Inisialisasi LCD dan RTC
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;
Servo servo;

int setButton = 2;
int upButton = 10;
int downButton = 11;
int okButton = 12;
int relayPin = 5;
int servoPin = 9;

int feedTime1Hour = 8;  // Waktu default jam pemberian pakan 1
int feedTime1Minute = 0;
int feedTime2Hour = 16; // Waktu default jam pemberian pakan 2
int feedTime2Minute = 0;

int currentSetting = 0; // 0: tidak dalam mode set, 1: set waktu 1, 2: set waktu 2
bool isSettingHour = true; // true untuk jam, false untuk menit

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(setButton, INPUT_PULLUP);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Matikan relay
  servo.attach(servoPin);
  servo.write(0); // Servo pada posisi tertutup

  if (!rtc.begin()) {
    lcd.print("RTC Error!");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("Fish Feeder System");
  delay(2000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();
  
  // Tampilkan waktu saat ini
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  printTime(now.hour(), now.minute());

  // Periksa apakah waktunya memberi pakan
  if ((now.hour() == feedTime1Hour && now.minute() == feedTime1Minute) || 
      (now.hour() == feedTime2Hour && now.minute() == feedTime2Minute)) {
    giveFeed();
  }

  // Mode pengaturan waktu pemberian pakan
  if (digitalRead(setButton) == LOW) {
    currentSetting = (currentSetting == 0) ? 1 : 0;
    delay(300);
  }

  if (currentSetting != 0) {
    setFeedTime();
  }

  delay(500);
}

// Fungsi untuk memberikan pakan (aktifkan relay dan servo)
void giveFeed() {
  digitalWrite(relayPin, HIGH); // Nyalakan relay
  lcd.setCursor(0, 3);
  lcd.print("Feeding...");
  
  // Gerakan servo untuk membuka pakan
  servo.write(90); // Buka servo (sesuaikan dengan mekanisme Anda)
  delay(5000); // Waktu pemberian pakan (misal 5 detik)
  
  servo.write(0); // Tutup servo
  digitalWrite(relayPin, LOW);  // Matikan relay
  lcd.setCursor(0, 3);
  lcd.print("              ");
}

// Fungsi pengaturan waktu pemberian pakan
void setFeedTime() {
  lcd.setCursor(0, 1);
  if (currentSetting == 1) {
    lcd.print("Set Feed 1 Time: ");
    lcd.setCursor(17, 1);
    printTime(feedTime1Hour, feedTime1Minute);
  } else if (currentSetting == 2) {
    lcd.print("Set Feed 2 Time: ");
    lcd.setCursor(17, 1);
    printTime(feedTime2Hour, feedTime2Minute);
  }

  // Pengaturan jam atau menit
  if (digitalRead(upButton) == LOW) {
    if (isSettingHour) {
      if (currentSetting == 1) feedTime1Hour = (feedTime1Hour + 1) % 24;
      else feedTime2Hour = (feedTime2Hour + 1) % 24;
    } else {
      if (currentSetting == 1) feedTime1Minute = (feedTime1Minute + 1) % 60;
      else feedTime2Minute = (feedTime2Minute + 1) % 60;
    }
    delay(100);
  }
  
  if (digitalRead(downButton) == LOW) {
    if (isSettingHour) {
      if (currentSetting == 1) feedTime1Hour = (feedTime1Hour - 1 + 24) % 24;
      else feedTime2Hour = (feedTime2Hour - 1 + 24) % 24;
    } else {
      if (currentSetting == 1) feedTime1Minute = (feedTime1Minute - 1 + 60) % 60;
      else feedTime2Minute = (feedTime2Minute - 1 + 60) % 60;
    }
    delay(100);
  }

  if (digitalRead(okButton) == LOW) {
    if (isSettingHour) {
      isSettingHour = false;  // Pindah ke menit
    } else {
      isSettingHour = true;   // Kembali ke jam
      currentSetting++;       // Pindah ke pengaturan berikutnya
      if (currentSetting > 2) currentSetting = 0;  // Kembali ke mode normal setelah setting selesai
    }
    delay(100);
  }
}

// Fungsi untuk menampilkan waktu (HH:MM)
void printTime(int hour, int minute) {
  if (hour < 10) lcd.print('0');
  lcd.print(hour);
  lcd.print(':');
  if (minute < 10) lcd.print('0');
  lcd.print(minute);
}
