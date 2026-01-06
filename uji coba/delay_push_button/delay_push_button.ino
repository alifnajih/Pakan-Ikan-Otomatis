#include <Servo.h>

Servo myServo;  // Membuat objek servo

const int setPin = 2;      // Pin untuk tombol Set
const int upPin = 10;       // Pin untuk tombol Up
const int downPin = 11;     // Pin untuk tombol Down
const int okPin = 12;       // Pin untuk tombol OK
const int servoPin = 9;    // Pin untuk servo

unsigned long delayTime = 1000;  // Nilai delay default (dalam milidetik)
unsigned long tempDelay = 1000;  // Nilai delay sementara saat setting
bool settingMode = false;        // Status mode pengaturan
int pos = 0;                     // Posisi servo

void setup() {
  myServo.attach(servoPin);   // Menghubungkan servo ke pin
  pinMode(setPin, INPUT);     // Tombol Set
  pinMode(upPin, INPUT);      // Tombol Up
  pinMode(downPin, INPUT);    // Tombol Down
  pinMode(okPin, INPUT);      // Tombol OK
  Serial.begin(9600);         // Mengaktifkan komunikasi serial (untuk debugging)
}

void loop() {
  // Mengecek apakah tombol Set ditekan untuk masuk ke mode pengaturan
  if (digitalRead(setPin) == HIGH) {
    settingMode = true;  // Masuk ke mode pengaturan
    setDelay();          // Memanggil fungsi pengaturan delay
  }

  // Jika tidak dalam mode pengaturan, gerakkan servo dengan delay
  if (!settingMode) {
    // Servo bergerak dari 0 hingga 180 derajat
    for (pos = 0; pos <= 30; pos += 1) {
      myServo.write(pos);   // Memutar servo ke posisi 'pos'
      delay(delayTime);     // Menggunakan nilai delay yang diset
    }
    
    // Servo kembali dari 180 hingga 0 derajat
    for (pos = 30; pos >= 0; pos -= 1) {
      myServo.write(pos);   // Memutar servo kembali ke posisi 0
      delay(delayTime);     // Menggunakan nilai delay yang diset
    }
  }
}

// Fungsi untuk mengatur delay dengan tombol Up, Down, OK
void setDelay() {
  Serial.println("Masuk ke mode pengaturan delay...");
  tempDelay = delayTime;  // Set nilai sementara berdasarkan nilai delay saat ini
  bool delaySet = false;  // Status apakah delay sudah diatur

  while (!delaySet) {
    // Mengecek tombol Up untuk menambah delay
    if (digitalRead(upPin) == HIGH) {
      tempDelay += 100;  // Menambah 100ms
      Serial.print("Delay saat ini: ");
      Serial.println(tempDelay);
      delay(200);  // Debounce
    }

    // Mengecek tombol Down untuk mengurangi delay
    if (digitalRead(downPin) == HIGH) {
      tempDelay -= 100;  // Mengurangi 100ms
      if (tempDelay < 100) {
        tempDelay = 100;  // Batas minimal 100ms
      }
      Serial.print("Delay saat ini: ");
      Serial.println(tempDelay);
      delay(200);  // Debounce
    }

    // Mengecek tombol OK untuk mengonfirmasi nilai delay
    if (digitalRead(okPin) == HIGH) {
      delayTime = tempDelay;  // Set nilai delay yang baru
      Serial.print("Delay diset ke: ");
      Serial.println(delayTime);
      delaySet = true;  // Keluar dari loop pengaturan
      delay(200);       // Debounce
    }
  }

  // Keluar dari mode pengaturan
  settingMode = false;
  Serial.println("Keluar dari mode pengaturan delay...");
}
