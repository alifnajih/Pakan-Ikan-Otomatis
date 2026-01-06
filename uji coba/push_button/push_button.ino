#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C dengan alamat 0x27 dan ukuran 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin push button
const int incrementButton = 2;
const int decrementButton = 10;
const int resetButton = 11;
const int holdButton = 12;

int counter = 0;          // Variabel untuk menyimpan nilai counter
bool hold = false;        // Menyimpan status apakah counter di-hold atau tidak

void setup() {
  // Inisialisasi pin sebagai input
  pinMode(incrementButton, INPUT_PULLUP);  // Menggunakan internal pull-up resistor
  pinMode(decrementButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(holdButton, INPUT_PULLUP);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Menampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Counter Program");
  delay(2000);  // Tampilkan pesan selama 2 detik
  lcd.clear();
  
  updateLCD();
}

void loop() {
  // Jika tombol hold ditekan, ubah status hold
  if (digitalRead(holdButton) == LOW) {
    delay(200);  // Debouncing
    hold = !hold;  // Toggle status hold
    updateLCD();
  }

  // Jika counter tidak di-hold
  if (!hold) {
    // Jika tombol increment ditekan
    if (digitalRead(incrementButton) == LOW) {
      delay(200);  // Debouncing
      counter++;
      updateLCD();
    }

    // Jika tombol decrement ditekan
    if (digitalRead(decrementButton) == LOW) {
      delay(200);  // Debouncing
      counter--;
      updateLCD();
    }

    // Jika tombol reset ditekan
    if (digitalRead(resetButton) == LOW) {
      delay(200);  // Debouncing
      counter = 0;
      updateLCD();
    }
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter Value:");

  lcd.setCursor(0, 1);
  lcd.print("Value: ");
  lcd.print(counter);  // Tampilkan nilai counter

  lcd.setCursor(0, 2);
  lcd.print("Status: ");
  if (hold) {
    lcd.print("Hold");
  } else {
    lcd.print("Running");
  }
}
