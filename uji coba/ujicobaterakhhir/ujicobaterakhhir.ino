#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <EEPROM.h>
#define ONE_WIRE_BUS 8  
#include <Servo.h>


#define trigPin2 4
#define echoPin2 3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27,20,4);
DS3231  rtc(SDA, SCL);
Servo myservo;

int addr1 = 0;
int addr2 = 1;
int addr3 = 2;
int addr4 = 3;
int addr5 = 4;

Time  t;
int jampagi,jamsiang,jamsore;

float TempC;
float pHValue;
int tds;
int adctds;
float voltage = 0;   // Tegangan yang terbaca dari sensor

int adcPH;
int TempCx;
int pHValuex;
int ntux;

float ntufix;  

long duration2, distance2;  

int btset = 2;
int btup = 10;
int btdown = 11;
int btok = 12;

int btsetx;
int btupx;
int btdownx;
int btokx;

float batasamo;
float batasph;
float amoniak;
float jumlahgerakan;

int lampu1 = A2;
int lampu2 = 6;
int relay = 5;
int buzzer = 13;    
int tanda = 0;
int kolam;

Servo servo1;
Servo servo2;



unsigned long previousMillisFeed = 0; 
unsigned long feedInterval = 20000;  // Delay for feeding in milliseconds

unsigned long previousMillisBeep = 0; 
unsigned long beepInterval = 500;  // Delay for beep in milliseconds

unsigned long previousMillisLCD = 0; 
unsigned long lcdUpdateInterval = 1000;  // LCD update interval


void setup(){  
  
jampagi = EEPROM.read(addr1);
jamsiang = EEPROM.read(addr2);
jamsore = EEPROM.read(addr3);
batasamo = EEPROM.read(addr4);
jumlahgerakan = EEPROM.read(addr5);

pinMode(relay,OUTPUT);
pinMode(lampu1,OUTPUT);
pinMode(lampu2,OUTPUT);
digitalWrite(relay,LOW);
digitalWrite(lampu1,LOW);
digitalWrite(lampu2,LOW);

pinMode(btset,INPUT_PULLUP);
pinMode(btup,INPUT_PULLUP);
pinMode(btdown,INPUT_PULLUP);
pinMode(btok,INPUT_PULLUP);

pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);  

Wire.begin();  
sensors.begin();
Serial.begin(115200);
lcd.init();  
lcd.clear(); 
lcd.backlight();

rtc.begin();
// rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAmy
// rtc.setTime(11, 7, 0);     // Set the time to 12:00:00 (24hr format)
// rtc.setDate(10, 10, 2024);   // Set the date to January 1st, 2014

servo1.attach(9);
servo2.attach(7);
servo1.write(90);
servo2.write(90);
}

void loop(){

btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);

if(btsetx == 0){
delay(1000);
lcd.clear();
setjampagi();
setjamsiang();
setjamsore();
setbatasamo();
setbatasph();
aturjumlahgerakan();
}
  
 t = rtc.getTime();


  //ketinggian pakan ikan
  digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  

  if(distance2 > 90){
    distance2 = 90;
  }
  
  kolam = 100 - distance2; 

if(kolam < 50){
  digitalWrite(lampu1,HIGH);
  servo2.write(0);
  beep(50);
  }

 if((kolam >= 50)&&(kolam <= 90)){
  digitalWrite(lampu1,LOW);
  servo2.write(0);
  }

 if(kolam > 90){
  digitalWrite(lampu1,LOW);
  servo2.write(90);
  }
     
 adcPH = analogRead(A1) * (5.0 / 1023.0);; //menggunakan pin A0 untuk membaca output sensor pH // Konversi nilai analog ke tegangan (0-5V)
 pHValue = (adcPH - 2.5) * 3.33;

 adctds = analogRead(A0);       
 voltage = adctds * (5.0 / 1024.0);      
 tds = -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;

 
 pHValuex = pHValue * 100.0;
 
  sensors.requestTemperatures();
  TempC = sensors.getTempCByIndex(0); // Celcius
  TempCx =  TempC * 100.0;

if(pHValue < 6.5){
  amoniak = (pHValue / TempC) * 0.202; 
}

if(pHValue == 7.0){
  amoniak = (pHValue / TempC) * 1.131; 
}

if(pHValue > 7.0){
  amoniak = (pHValue / TempC) * 3.306; 
}


if(amoniak > batasamo){
  digitalWrite(lampu2,HIGH);
  beep (5);
  }

if(amoniak <= batasamo){
  digitalWrite(lampu2,LOW);
  }

  

if((t.hour == jampagi)&&(jampagi > 0)&&(tanda == 0)){
  digitalWrite(relay,HIGH);
  gerakkanservo();
  delay(20000);
  digitalWrite(relay,LOW); 
  tanda = 1;
}

if((t.hour == jamsiang)&&(jamsiang > 0)&&(tanda == 1)){
  digitalWrite(relay,HIGH);
  gerakkanservo();
  delay(20000);
  digitalWrite(relay,LOW);
  tanda = 2;
}

if((t.hour == jamsore)&&(jamsore > 0)&&(tanda == 2)){
  digitalWrite(relay,HIGH);
  gerakkanservo();
  delay(20000);
  digitalWrite(relay,LOW);
  tanda = 0;
}

  
  lcd.setCursor(0,0);
  lcd.print("TDS:");
  lcd.print(tds);
  lcd.print("NTU ");
 
  lcd.setCursor(12,0);
  lcd.print("PH:");
  lcd.print(pHValue);
  lcd.print("  ");

  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(TempC,1);
  lcd.print("c ");
  
  lcd.print(batasamo);
  lcd.print("/");
  lcd.print(batasph);
  lcd.print(" ");

  lcd.setCursor(0,2);
  lcd.print(t.hour, DEC);
  lcd.print(":");
  lcd.print(t.min, DEC);  
  lcd.print(":");
  lcd.print(t.sec, DEC);
  lcd.print("  ");
  lcd.print(jampagi);
  lcd.print("/");
  lcd.print(jamsiang);
  lcd.print("/");
  lcd.print(jamsore);
  lcd.print("  ");

  lcd.setCursor(0,3);
  lcd.print("S=");
  lcd.print(kolam);
  lcd.print("/");
  lcd.print(jumlahgerakan);
  lcd.print("  ");

  lcd.setCursor(10,3);
  lcd.print("Mg/l:");
  lcd.print(amoniak);
  lcd.print(" ");

  Serial.print("*");
  Serial.print(pHValue * 100.0);
  Serial.print(",");
  Serial.print(tds);
  Serial.print(",");
  Serial.print(amoniak * 100.0);
  Serial.print(",");
  Serial.print(TempC * 100.0);
  Serial.println("#");
  
  unsigned long currentMillis = millis(); 

if (currentMillis - previousMillisLCD >= lcdUpdateInterval) {
    previousMillisLCD = currentMillis;  // Simpan waktu sekarang

}
}

void setjampagi(){
  
btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);  

  lcd.setCursor(0,0);
  lcd.print("Waktu Pakan Pagi");
  lcd.setCursor(0,1);
  lcd.print("Jam: ");
  lcd.print(jampagi);
  lcd.print("   ");

if(btupx == 0){
  delay(200);
  jampagi++; 
}

if(btdownx == 0){
  delay(200);
  jampagi--; 
}

if(jampagi > 23){
jampagi = 0;  
}

if(btokx == 0){
  lcd.clear();
  delay(2000);
  EEPROM.write(addr1, jampagi);
  return; 
}

setjampagi();  
}


void setjamsiang(){
  
btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);  

  lcd.setCursor(0,0);
  lcd.print("Waktu Pakan Siang  ");
  lcd.setCursor(0,1);
  lcd.print("Jam: ");
  lcd.print(jamsiang);
  lcd.print("   ");

if(btupx == 0){
  delay(200);
  jamsiang++; 
}

if(btdownx == 0){
  delay(200);
  jamsiang--; 
}

if(jamsiang > 23){
jamsiang = 0;  
}

if(btokx == 0){
  lcd.clear();
  delay(2000);
  EEPROM.write(addr2, jamsiang);
  return; 
}

setjamsiang();  
}


void setjamsore(){
  
btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);  

  lcd.setCursor(0,0);
  lcd.print("Waktu Pakan Sore  ");
  lcd.setCursor(0,1);
  lcd.print("Jam: ");
  lcd.print(jamsore);
  lcd.print("   ");

if(btupx == 0){
  delay(200);
  jamsore++; 
}

if(btdownx == 0){
  delay(200);
  jamsore--; 
}

if(jamsore > 23){
jamsore = 0;  
}

if(btokx == 0){
  lcd.clear();
  delay(2000);
  EEPROM.write(addr3, jamsore);
  return; 
}

setjamsore();  
}


void setbatasamo(){
  
btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);  

  lcd.setCursor(0,0);
  lcd.print("BATAS AMONIAK  ");
  lcd.setCursor(0,1);
  lcd.print("mg/l: ");
  lcd.print(batasamo);
  lcd.print("   ");

if(btupx == 0){
  delay(200);
  batasamo = batasamo + 0.1; 
}

if(btdownx == 0){
  delay(200);
  batasamo = batasamo - 0.1; 
}

if(batasamo > 100.0){
batasamo = 0;  
}

if(btokx == 0){
  lcd.clear();
  delay(2000);
  EEPROM.write(addr4, batasamo);
  return; 
}

setbatasamo();  
}



void setbatasph(){
  
btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);  

  lcd.setCursor(0,0);
  lcd.print("BATAS PH  ");
  lcd.setCursor(0,1);
  lcd.print("PH: ");
  lcd.print(batasph);
  lcd.print("   ");

if(btupx == 0){
  delay(200);
  batasph = batasph + 0.1; 
}

if(btdownx == 0){
  delay(200);
  batasph = batasph - 0.1; 
}

if(batasph > 100.0){
batasph = 0;  
}

if(btokx == 0){
  lcd.clear();
  delay(2000);
  //EEPROM.write(addr5, batasph);
  return; 
}

setbatasph();  
}


void aturjumlahgerakan(){
  
btsetx = digitalRead(btset);
btupx = digitalRead(btup);
btdownx = digitalRead(btdown);
btokx = digitalRead(btok);  

  lcd.setCursor(0,0);
  lcd.print("atur jumlah pakan:  ");
  lcd.setCursor(0,1);
  lcd.print("gram: ");
  lcd.print(jumlahgerakan);
  lcd.print("   ");

if(btupx == 0){
  delay(200);
  jumlahgerakan++; 
}

if(btdownx == 0){
  delay(200);
  jumlahgerakan--; 
}

if(jumlahgerakan > 23){
jumlahgerakan = 0;  
}

if(btokx == 0){
  lcd.clear();
  delay(2000);
  EEPROM.write(addr5, jumlahgerakan);
  return; 
}

aturjumlahgerakan();  
}


void gerakkanservo() {
  for (int i = 0; i < jumlahgerakan; i++) {
    servo1.write(0);  // Menggerakkan servo ke 0 derajat
    delay(800);        // Tunggu 1 detik
    servo1.write(90);   // Menggerakkan servo kembali ke 90 derajat
    delay(1500);        // Tunggu 1 detik
  }
}



void beep(int kedip) {
   static unsigned long previousMillisBeep = 0;
   static int beepCount = 0;
   bool beepState = LOW;

   unsigned long currentMillis = millis();
   
   if (currentMillis - previousMillisBeep >= beepInterval) {
      previousMillisBeep = currentMillis;
      
      if (beepCount < kedip) {
        beepState = !beepState;
        digitalWrite(buzzer, beepState);
        beepCount++;
      } else {
        digitalWrite(buzzer, LOW);
        beepCount = 0;
      }
   }
}

