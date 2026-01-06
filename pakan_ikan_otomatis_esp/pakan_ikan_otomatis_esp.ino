#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>

int temp;
int x = 5;
int y;

int value1;
int value2;
int value3;
int value4;

float ph;
int tds;
float amoniak;
float suhu;

int datain1;
int datain2;
int datain3;
int datain4;

String dataIn;
String dt[10];
int i;
boolean parsing = false;

String apiKey = "UYGFUY87JKJGJ6HKS";     //  Enter your Write API key from ThingSpeak
const char* resource = "/update?api_key=";

const char* ssid =  "hotspothpku";     // replace with your wifi ssid and wpa2 key
const char* pass =  "123456789";
const char* server = "api.thingspeak.com";

WiFiClient  client;

void setup()
{
 
  dataIn="";
  Serial.begin(9600);
  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

      while (WiFi.status() != WL_CONNECTED)
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

}

void loop()
{

while(Serial.available()>0) {
//   dataIn="";
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '\n') {
    parsing = true;
  }
}

if(parsing){
    parsingData();
       
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(ph);
                             postStr += "\r\n\r\n";

                             client.print(String("GET ") + resource + apiKey + "&field1=" + ph + "&field2=" + tds + "&field3=" + amoniak + "&field4=" + suhu + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");
                                       
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                           
                          
                             Serial.println(". Send to Thingspeak.");
                        }
                       
          client.stop();

          //Serial.println("Waiting...");

  delay(10000);
 
 }
}

void parsingData(){
int j=0;

//kirim data yang telah diterima sebelumnya
//Serial.print("data masuk : ");
//Serial.print(dataIn);
//Serial.print("\n");

//inisialisasi variabel, (reset isi variabel)
dt[j]="";
//proses parsing data
for(i=1;i<dataIn.length();i++){
//pengecekan tiap karakter dengan karakter (#) dan (,)
if ((dataIn[i] == '#') || (dataIn[i] == ','))
{
//increment variabel j, digunakan untuk merubah index array penampung
j++;
dt[j]="";       //inisialisasi variabel array dt[j]
}
else
{
//proses tampung data saat pengecekan karakter selesai.
dt[j] = dt[j] + dataIn[i];
}
}

 datain1 = dt[0].toInt();
 datain2 = dt[1].toInt();
 datain3 = dt[2].toInt();
 datain4 = dt[3].toInt();

//kirim data hasil parsing
Serial.print("data 1 : ");
Serial.print(datain1);
Serial.print("\n");
Serial.print("data 2 : ");
Serial.print(datain2);
Serial.print("\n");
Serial.print("data 3 : ");
Serial.print(datain3);

 ph = datain1 / 100.0;
 tds = datain2 / 1;
 amoniak = datain3 / 100.0;
 suhu = datain4 / 100.0;
 
}
