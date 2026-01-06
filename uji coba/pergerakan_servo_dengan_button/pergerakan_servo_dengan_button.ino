#include <Servo.h>    // Pustaka servo
#include <RTClib.h>   // Pustaka RTC untuk waktu

Servo myServo;        // Objek servo
RTC_DS3231 rtc;       // Objek RTC
int servoPin = 9;     // Pin servo

// Tombol input
const int buttonSet = 2;
const int buttonUp = 3;
const int buttonDown = 4;
const int buttonOk = 5;

// Variabel untuk tombol dan jumlah gerakan
int jumlahGerakan = 0;
int maxGerakan = 10;
bool settingMode = false;

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi RTC
  if (!rtc.begin()) {
    Serial.println("RTC tidak terdeteksi");
    while (1);
  }

  // Inisialisasi servo
  myServo.attach(servoPin);

  // Inisialisasi tombol input
  pinMode(buttonSet, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonOk, INPUT_PULLUP);

  Serial.println("Tekan SET untuk mengatur jumlah gerakan");
}

void loop() {
  DateTime now = rtc.now(); // Membaca waktu saat ini

  // Mengecek apakah tombol set ditekan
  if (digitalRead(buttonSet) == LOW) {
    settingMode = true;
    aturJumlahGerakan();
  }

  // Mengecek apakah sekarang pukul 7 pagi
  if (now.hour() == 7 && now.minute() == 0) {
    gerakkanServo();
  }
}

// Fungsi untuk mengatur jumlah pergerakan servo
void aturJumlahGerakan() {
  while (settingMode) {
    // Menambah nilai gerakan jika tombol UP ditekan
    if (digitalRead(buttonUp) == LOW) {
      jumlahGerakan++;
      if (jumlahGerakan > maxGerakan) {
        jumlahGerakan = maxGerakan;
      }
      Serial.print("Jumlah gerakan: ");
      Serial.println(jumlahGerakan);
      delay(200); // Debounce
    }

    // Mengurangi nilai gerakan jika tombol DOWN ditekan
    if (digitalRead(buttonDown) == LOW) {
      jumlahGerakan--;
      if (jumlahGerakan < 0) {
        jumlahGerakan = 0;
      }
      Serial.print("Jumlah gerakan: ");
      Serial.println(jumlahGerakan);
      delay(200); // Debounce
    }

    // Mengakhiri setting mode jika tombol OK ditekan
    if (digitalRead(buttonOk) == LOW) {
      settingMode = false;
      Serial.print("Nilai akhir jumlah gerakan: ");
      Serial.println(jumlahGerakan);
      delay(200); // Debounce
    }
  }
}

// Fungsi untuk menggerakkan servo dari 0 ke 20 derajat
void gerakkanServo() {
  for (int i = 0; i < jumlahGerakan; i++) {
    myServo.write(20);  // Menggerakkan servo ke 20 derajat
    delay(1000);        // Tunggu 1 detik
    myServo.write(0);   // Menggerakkan servo kembali ke 0 derajat
    delay(1000);        // Tunggu 1 detik
  }
}
