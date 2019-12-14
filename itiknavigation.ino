#include <JC_Button.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <EEPROM.h>
#include <DHT.h>

#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
#define DHT_jeda 2000
unsigned long DHT_millis = 0;
 
char daysOfTheWeek[7][12] = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};
 
RTC_DS1307 rtc; 
LiquidCrystal_I2C lcd(0x27, 16,2);
 
int jam, menit, tanggal, bulan, tahun;
int jam1, menit1, tanggal1, bulan1, tahun1;
unsigned int thnAksi=2000;

int state_MotorON=1;
int tglMotorON=00, blnMotorON=00,thnMotorON=00;
int state_MotorOFF=5;
int tglMotorOFF=00, blnMotorOFF=00,thnMotorOFF=00;
unsigned long Motor_millis = 0;
unsigned long MotorOn =13000;
unsigned long MotorOff =43200000; //18000000; 12 jam
unsigned long intervalMotor = MotorOn;
boolean motorState = true;
 
int state_EmbunON=9;
int tglEmbunON=00, blnEmbunON=00,thnEmbunON=00;
int state_EmbunOFF=13;
int tglEmbunOFF=00, blnEmbunOFF=00, thnEmbunOFF=00;
unsigned long Embun_millis = 0;
unsigned long EmbunOn =10000;
unsigned long EmbunOff =3000; // 5 jam : 18000000
unsigned long intervalEmbun = EmbunOn;
boolean embunState = true;
 
const byte
BUTTON_PIN(2),BUTTON_PIN2(3),BUTTON_PIN3(4),BUTTON_PIN4(5),
relay_alrm(11),rLampu(7), rMotor(8), rEmbun(9);  
             
Button 
myBtn(BUTTON_PIN),
myBtn2(BUTTON_PIN2),
myBtn3(BUTTON_PIN3),
myBtn4(BUTTON_PIN4);    
 
void setup()
{
  lcd.begin(); // 16 baris, 2 kolom
  lcd.setBacklight(255); //menyalakan lampu latar
   
  lcd.setCursor(1,0);
  lcd.print("Itik");
  lcd.setCursor(4,1);
  lcd.print(" Navigation");
  delay(3000);
  lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Starting...");
  lcd.setCursor(6,1);
  lcd.print("By Cangkal");
  delay(3000);
  lcd.clear();
 
  myBtn.begin();  
  myBtn2.begin();
  myBtn3.begin();
  myBtn4.begin();   
  pinMode(rLampu, OUTPUT);
  digitalWrite(rLampu, HIGH);
  pinMode(rMotor, OUTPUT);
  digitalWrite(rMotor,motorState);
  pinMode(rEmbun, OUTPUT);
  digitalWrite(rEmbun,embunState);

  dht.begin();
  delay(2000);
  
}
 
void loop()
{  
  myBtn.read();
  myBtn2.read();
  myBtn3.read();
  myBtn4.read();
 
  DateTime now = rtc.now();
  lcd.setCursor(00,00);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(",");
  lcd.setCursor(4,00);
  if (now.day()<=9){lcd.print("0");}
  lcd.print(now.day(), DEC);
  lcd.print('-');
  if (now.month()<=9){lcd.print("0");}
  lcd.print(now.month(), DEC);
  lcd.print('-');
  lcd.print(now.year(), DEC);

  lcd.setCursor(00,1);
  if (now.hour()<=9){lcd.print("0");}
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute()<=9){lcd.print("0");}
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  if (now.second()<=9){lcd.print("0");}
  lcd.print(now.second(), DEC);   

  dhtRead();
  lampuAksi();

  lcd.setCursor(10,1);lcd.print("L");

  state_MotorON = EEPROM.read(1);
  tglMotorON = EEPROM.read(2);
  blnMotorON = EEPROM.read(3);
  thnMotorON = EEPROM.read(4);
  state_MotorOFF = EEPROM.read(5);
  tglMotorOFF = EEPROM.read(6);
  blnMotorOFF = EEPROM.read(7);
  thnMotorOFF = EEPROM.read(8);

  lcd.setCursor(12,1);lcd.print("M");
  MotorAksi();

  state_EmbunON = EEPROM.read(9);
  tglEmbunON = EEPROM.read(10);
  blnEmbunON = EEPROM.read(11);
  thnEmbunON = EEPROM.read(12);
  state_EmbunOFF = EEPROM.read(13);
  tglEmbunOFF = EEPROM.read(14);
  blnEmbunOFF = EEPROM.read(15);
  thnEmbunOFF = EEPROM.read(16);

  lcd.setCursor(14,1);lcd.print("E");
  EmbunAksi();
  
  if (myBtn2.wasReleased()) {Status();}
  if (myBtn.wasReleased())  {Settings();}
}

void dhtRead(){
unsigned long DHT_current = millis();
if (DHT_current - DHT_millis >= DHT_jeda) {
    DHT_millis = DHT_current;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
    }
  }
}

void Status(){
  lcd.clear();
  int e = 1;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
    lcd.setCursor(0, 0);
    lcd.print("Temp");
    lcd.setCursor(0, 1);
    lcd.print("Humidity");
    lcd.setCursor(9, 0);
    lcd.print(t);
    lcd.print(" C");
    lcd.setCursor(9, 1);
    lcd.print(h);
    lcd.print(" %");
      
    if (myBtn4.wasReleased()) { lcd.clear();break;}
    }
  
}
 
void Settings(){
  lcd.clear();
  int a = 1;
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
    lcd.setCursor(0,0);lcd.print("Setting:");
    lcd.setCursor(10,0);lcd.print("Date");
    lcd.setCursor(1,1);lcd.print("Motor");
    lcd.setCursor(10,1);lcd.print("Embun");
      
    if (myBtn2.wasReleased())  {a++; if(a==4) a=1;}
    if (myBtn3.wasReleased())  {a--; if(a==-1) a=3;}
    switch(a){
      case 1:
      lcd.setCursor(9,0);lcd.print(">");
      lcd.setCursor(0,1);lcd.print(" ");
      lcd.setCursor(9,1);lcd.print(" ");
      if (myBtn.wasReleased())  {Set_date();}
      break;
      case 2:
      lcd.setCursor(9,0);lcd.print(" ");
      lcd.setCursor(0,1);lcd.print(">");
      lcd.setCursor(9,1);lcd.print(" ");
      if (myBtn.wasReleased())  {SetMotor();}
      break;
      case 3:
      lcd.setCursor(9,0);lcd.print(" ");
      lcd.setCursor(0,1);lcd.print(" ");
      lcd.setCursor(9,1);lcd.print(">");
      if (myBtn.wasReleased())  {SetEmbun();}
      break;
    }
    if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}

void SetEmbun(){
  lcd.clear();
  int i = 1;
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
    lcd.setCursor(0,0);lcd.print("Setting: EMBUN");
    lcd.setCursor(1,1);lcd.print("ON");
    lcd.setCursor(5,1);lcd.print("OFF");
      
    if (myBtn2.wasReleased())  {i++; if(i==3) i=1;}
    if (myBtn3.wasReleased())  {i--; if(i==0) i=2;}
    switch(i){
      case 1:
      lcd.setCursor(0,1);lcd.print(">");
      lcd.setCursor(4,1);lcd.print(" ");
      if (myBtn.wasReleased())  {Set_EmbunON();}
      break;
      case 2:
      lcd.setCursor(0,1);lcd.print(" ");
      lcd.setCursor(4,1);lcd.print(">");
      if (myBtn.wasReleased())  {Set_EmbunOFF();}
      break;
    }    
    if (myBtn.wasReleased()) {Settings();}
    if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}

void SetMotor(){
  lcd.clear();
  int j = 1;
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
    lcd.setCursor(0,0);lcd.print("Setting: MOTOR");
    lcd.setCursor(1,1);lcd.print("ON");
    lcd.setCursor(5,1);lcd.print("OFF");
      
    if (myBtn2.wasReleased())  {j++; if(j==3) j=1;}
    if (myBtn3.wasReleased())  {j--; if(j==0) j=2;}
    switch(j){
      case 1:
      lcd.setCursor(0,1);lcd.print(">");
      lcd.setCursor(4,1);lcd.print(" ");
      if (myBtn.wasReleased())  {Set_MotorON();}
      break;
      case 2:
      lcd.setCursor(0,1);lcd.print(" ");
      lcd.setCursor(4,1);lcd.print(">");
      if (myBtn.wasReleased())  {Set_MotorOFF();}
      break;
    }    
    if (myBtn.wasReleased()) {Settings();}
    if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}
 
void Set_date(){
  lcd.clear();
  int b = 1;
  DateTime now = rtc.now();
  jam = now.hour(),DEC;
  menit = now.minute(),DEC;
  tanggal = now.day(),DEC;
  bulan = now.month(),DEC;
  tahun = now.year(),DEC;

  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
    if (myBtn.wasReleased())  {b++; if(b==7) b=1;}
     
    lcd.setCursor(00,00);lcd.print(jam,DEC);
    lcd.setCursor(2,00);lcd.print(":");
    lcd.setCursor(3,00);lcd.print(menit,DEC);
    lcd.setCursor(00,1);lcd.print(tanggal,DEC);
    lcd.setCursor(2,1);lcd.print("-");
    lcd.setCursor(3,1);lcd.print(bulan,DEC);
    lcd.setCursor(5,1);lcd.print("-");
    lcd.setCursor(6,1);lcd.print(tahun,DEC);
    lcd.setCursor(11,1);lcd.print("SAVE");
      
    switch(b){
      case 1:
      lcd.setCursor(00,00);lcd.print(jam,DEC);
      lcd.setCursor(00,00);lcd.print("  ");
      lcd.setCursor(2,00);lcd.print(":");
      if (myBtn2.wasReleased())  {jam++; if(jam==24) jam=0;}
      if (myBtn3.wasReleased())  {jam--; if(jam==-1) jam=23;}
      break;
      case 2:
      lcd.setCursor(3,00);lcd.print(menit,DEC);
      lcd.setCursor(3,00);lcd.print("  ");
      if (myBtn2.wasReleased())  {menit++; if(menit==60) menit=0;}
      if (myBtn3.wasReleased())  {menit--; if(menit==-1) menit=59;}
      break;
      case 3:
      lcd.setCursor(00,1);lcd.print(tanggal,DEC);
      lcd.setCursor(00,1);lcd.print("  ");
      lcd.setCursor(2,1);lcd.print("-");
      if (myBtn2.wasReleased())  {tanggal++; if(tanggal==32) tanggal=1;}
      if (myBtn3.wasReleased())  {tanggal--; if(tanggal==0) tanggal=31;}
      break;
      case 4:
      lcd.setCursor(3,1);lcd.print(bulan,DEC);
      lcd.setCursor(3,1);lcd.print("  ");
      lcd.setCursor(5,1);lcd.print("-");
      if (myBtn2.wasReleased())  {bulan++; if(bulan==13) bulan=1;}
      if (myBtn3.wasReleased())  {bulan--; if(bulan==0) bulan=12;}
      break;
      case 5:
      lcd.setCursor(6,1);lcd.print(tahun,DEC);
      lcd.setCursor(6,1);lcd.print("    ");
      if (myBtn2.wasReleased())  {tahun++; if(tahun==2100) tahun=2000;}
      if (myBtn3.wasReleased())  {tahun--; if(tahun==1999) tahun=2099;}
      break;
      case 6:
      lcd.setCursor(11,1);lcd.print("SAVE");
      lcd.setCursor(11,1);lcd.print("    ");
      if (myBtn2.wasReleased())  { 
        jam1=jam; menit1=menit; tanggal1=tanggal; bulan1=bulan; tahun1=tahun;
        rtc.adjust(DateTime(tahun1,bulan1,tanggal1,jam1,menit1));
        lcd.setCursor(00,00);lcd.print("Data tersimpan..");
        delay(1000);lcd.clear();
      }
      break;
    }
    if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}

void Set_MotorON(){
  lcd.clear();
  int d = 1;
  DateTime now = rtc.now();
  
  state_MotorON = EEPROM.read(1);
  tglMotorON = EEPROM.read(2);
  blnMotorON = EEPROM.read(3);
  thnMotorON = EEPROM.read(4);
 
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
       
    lcd.setCursor(00,00);lcd.print("Motor:");
    if(state_MotorON==1) {lcd.setCursor(7,00);lcd.print("ON");}
    lcd.setCursor(0,1);lcd.print(tglMotorON);
    lcd.setCursor(2,1);lcd.print("-");
    lcd.setCursor(3,1);lcd.print(blnMotorON);
    lcd.setCursor(5,1);lcd.print("-");
    lcd.setCursor(6,1);lcd.print(thnAksi+thnMotorON);
    lcd.setCursor(12,1);lcd.print("SAVE");
    if (myBtn.wasReleased())  {d++; if(d==6) d=1;}
    switch(d){
      case 1:
      if(state_MotorON==1) {lcd.setCursor(7,00);lcd.print("ON");}
      lcd.setCursor(7,00);lcd.print("   ");
      if (myBtn2.wasReleased())  {state_MotorON++; if(state_MotorON>=0) state_MotorON=1;}
      if (myBtn3.wasReleased())  {state_MotorON--; if(state_MotorON<=2) state_MotorON=1;}
      break;
      case 2:
      lcd.setCursor(00,1);lcd.print(tglMotorON);
      lcd.setCursor(00,1);lcd.print("  ");
      lcd.setCursor(2,1);lcd.print("-");
      if (myBtn2.wasReleased())  {tglMotorON++; if(tglMotorON>=32) tglMotorON=1;}
      if (myBtn3.wasReleased())  {tglMotorON--; if(tglMotorON==0) tglMotorON=31;}
      break;
      case 3:
      lcd.setCursor(3,1);lcd.print(blnMotorON);
      lcd.setCursor(3,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {blnMotorON++; if(blnMotorON>=13) blnMotorON=1;}
      if (myBtn3.wasReleased())  {blnMotorON--; if(blnMotorON==0) blnMotorON=12;}
      break;
      case 4:
      lcd.setCursor(6,1);lcd.print(thnAksi+thnMotorON);
      lcd.setCursor(8,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {thnMotorON++; if(thnMotorON>=100) thnMotorON=00;}
      if (myBtn3.wasReleased())  {thnMotorON--; if(thnMotorON<=-1) thnMotorON=99;}
      break;      
      case 5:
      lcd.setCursor(12,1);lcd.print("SAVE");
      lcd.setCursor(12,1);lcd.print("    ");
      if (myBtn2.wasReleased())  { 
        EEPROM.write(1,state_MotorON);
        EEPROM.write(2,tglMotorON);
        EEPROM.write(3,blnMotorON);
        EEPROM.write(4,thnMotorON);
        lcd.setCursor(00,00);lcd.print("Data tersimpan..");
        delay(1000);lcd.clear();
      }
      break;
    
    }
  if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}

void Set_MotorOFF(){
  lcd.clear();
  int f = 1;
  DateTime now = rtc.now();
  
  state_MotorOFF = EEPROM.read(5);
  tglMotorOFF = EEPROM.read(6);
  blnMotorOFF = EEPROM.read(7);
  thnMotorOFF = EEPROM.read(8);
 
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
       
    lcd.setCursor(00,00);lcd.print("Motor:");
    if(state_MotorOFF==5) {lcd.setCursor(7,00);lcd.print("OFF");}
    lcd.setCursor(00,1);lcd.print(tglMotorOFF);
    lcd.setCursor(2,1);lcd.print("-");
    lcd.setCursor(3,1);lcd.print(blnMotorOFF);
    lcd.setCursor(5,1);lcd.print("-");
    lcd.setCursor(6,1);lcd.print(thnAksi+thnMotorOFF);
    lcd.setCursor(12,1);lcd.print("SAVE");
    if (myBtn.wasReleased())  {f++; if(f==6) f=1;}
    switch(f){
      case 1:
      if(state_MotorOFF==5) {lcd.setCursor(7,00);lcd.print("OFF ");}
      lcd.setCursor(7,00);lcd.print("   ");
      if (myBtn2.wasReleased())  {state_MotorOFF++; if(state_MotorOFF>=6) state_MotorOFF=5;}
      if (myBtn3.wasReleased())  {state_MotorOFF--; if(state_MotorOFF<=4) state_MotorOFF=5;}
      break;
      case 2:
      lcd.setCursor(00,1);lcd.print(tglMotorOFF);
      lcd.setCursor(00,1);lcd.print("  ");
      lcd.setCursor(2,1);lcd.print("-");
      if (myBtn2.wasReleased())  {tglMotorOFF++; if(tglMotorOFF>=32) tglMotorOFF=1;}
      if (myBtn3.wasReleased())  {tglMotorOFF--; if(tglMotorOFF==0) tglMotorOFF=31;}
      break;
      case 3:
      lcd.setCursor(3,1);lcd.print(blnMotorOFF);
      lcd.setCursor(3,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {blnMotorOFF++; if(blnMotorOFF>=13) blnMotorOFF=1;}
      if (myBtn3.wasReleased())  {blnMotorOFF--; if(blnMotorOFF==0) blnMotorOFF=12;}
      break;
      case 4:
      lcd.setCursor(6,1);lcd.print(thnAksi+thnMotorOFF);
      lcd.setCursor(8,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {thnMotorOFF++; if(thnMotorOFF>=100) thnMotorOFF=00;}
      if (myBtn3.wasReleased())  {thnMotorOFF--; if(thnMotorOFF<=-1) thnMotorOFF=99;}
      break;   
      case 5:
      lcd.setCursor(12,1);lcd.print("SAVE");
      lcd.setCursor(12,1);lcd.print("    ");
      if (myBtn2.wasReleased())  { 
        EEPROM.write(5,state_MotorOFF);
        EEPROM.write(6,tglMotorOFF);
        EEPROM.write(7,blnMotorOFF);
        EEPROM.write(8,thnMotorOFF);
        lcd.setCursor(00,00);lcd.print("Data tersimpan..");
        delay(1000);lcd.clear();
      }
      break;
    
    }
  if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}

void Set_EmbunON(){
  lcd.clear();
  int g = 1;
  DateTime now = rtc.now();
  
  state_EmbunON = EEPROM.read(9);
  tglEmbunON = EEPROM.read(10);
  blnEmbunON = EEPROM.read(11);
  thnEmbunON = EEPROM.read(12);
 
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
       
    lcd.setCursor(00,00);lcd.print("Embun:");
    if(state_EmbunON==9) {lcd.setCursor(7,00);lcd.print("ON");}
    lcd.setCursor(00,1);lcd.print(tglEmbunON);
    lcd.setCursor(2,1);lcd.print("-");
    lcd.setCursor(3,1);lcd.print(blnEmbunON);
    lcd.setCursor(5,1);lcd.print("-");
    lcd.setCursor(6,1);lcd.print(thnAksi+thnEmbunON);
    lcd.setCursor(12,1);lcd.print("SAVE");
    if (myBtn.wasReleased())  {g++; if(g==6) g=1;}
    switch(g){
      case 1:
      if(state_EmbunON==9) {lcd.setCursor(7,00);lcd.print("ON");}
      lcd.setCursor(7,00);lcd.print("   ");
      if (myBtn2.wasReleased())  {state_EmbunON++; if(state_EmbunON>=10) state_EmbunON=9;}
      if (myBtn3.wasReleased())  {state_EmbunON--; if(state_EmbunON<=8) state_EmbunON=9;}
      break;
      case 2:
      lcd.setCursor(00,1);lcd.print(tglEmbunON);
      lcd.setCursor(00,1);lcd.print("  ");
      lcd.setCursor(2,1);lcd.print("-");
      if (myBtn2.wasReleased())  {tglEmbunON++; if(tglEmbunON>=32) tglEmbunON=1;}
      if (myBtn3.wasReleased())  {tglEmbunON--; if(tglEmbunON==0) tglEmbunON=31;}
      break;
      case 3:
      lcd.setCursor(3,1);lcd.print(blnEmbunON);
      lcd.setCursor(3,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {blnEmbunON++; if(blnEmbunON>=13) blnEmbunON=1;}
      if (myBtn3.wasReleased())  {blnEmbunON--; if(blnEmbunON==0) blnEmbunON=12;}
      break;
      case 4:
      lcd.setCursor(6,1);lcd.print(thnAksi+thnEmbunON);
      lcd.setCursor(8,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {thnEmbunON++; if(thnEmbunON>=100) thnEmbunON=00;}
      if (myBtn3.wasReleased())  {thnEmbunON--; if(thnEmbunON<=-1) thnEmbunON=99;}
      break;  
      case 5:
      lcd.setCursor(12,1);lcd.print("SAVE");
      lcd.setCursor(12,1);lcd.print("    ");
      if (myBtn2.wasReleased())  { 
        EEPROM.write(9,state_EmbunON);
        EEPROM.write(10,tglEmbunON);
        EEPROM.write(11,blnEmbunON);
        EEPROM.write(12,thnEmbunON);
        lcd.setCursor(00,00);lcd.print("Data tersimpan..");
        delay(1000);lcd.clear();
      }
      break;
    
    }
  if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}

void Set_EmbunOFF(){
  lcd.clear();
  int h = 1;
  DateTime now = rtc.now();
  
  state_EmbunOFF = EEPROM.read(13);
  tglEmbunOFF = EEPROM.read(14);
  blnEmbunOFF = EEPROM.read(15);
  thnEmbunOFF = EEPROM.read(16);
 
  while(1){
    myBtn.read();
    myBtn2.read();
    myBtn3.read();
    myBtn4.read();
       
    lcd.setCursor(00,00);lcd.print("Embun:");
    if(state_EmbunOFF==13) {lcd.setCursor(7,00);lcd.print("OFF");}
    lcd.setCursor(00,1);lcd.print(tglEmbunOFF);
    lcd.setCursor(2,1);lcd.print("-");
    lcd.setCursor(3,1);lcd.print(blnEmbunOFF);
    lcd.setCursor(5,1);lcd.print("-");
    lcd.setCursor(6,1);lcd.print(thnAksi+thnEmbunOFF);
    lcd.setCursor(12,1);lcd.print("SAVE");
    if (myBtn.wasReleased())  {h++; if(h==6) h=1;}
    switch(h){
      case 1:
      if(state_EmbunOFF==13) {lcd.setCursor(7,00);lcd.print(state_EmbunOFF);}
      lcd.setCursor(7,00);lcd.print("   ");
      if (myBtn2.wasReleased())  {state_EmbunOFF++; if(state_EmbunOFF>=14) state_EmbunOFF=13;}
      if (myBtn3.wasReleased())  {state_EmbunOFF--; if(state_EmbunOFF<=12) state_EmbunOFF=13;}
      break;
      case 2:
      lcd.setCursor(00,1);lcd.print(tglEmbunOFF);
      lcd.setCursor(00,1);lcd.print("  ");
      lcd.setCursor(2,1);lcd.print("-");
      if (myBtn2.wasReleased())  {tglEmbunOFF++; if(tglEmbunOFF>=32) tglEmbunOFF=1;}
      if (myBtn3.wasReleased())  {tglEmbunOFF--; if(tglEmbunOFF==0) tglEmbunOFF=31;}
      break;
      case 3:
      lcd.setCursor(3,1);lcd.print(blnEmbunOFF);
      lcd.setCursor(3,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {blnEmbunOFF++; if(blnEmbunOFF>=13) blnEmbunOFF=1;}
      if (myBtn3.wasReleased())  {blnEmbunOFF--; if(blnEmbunOFF==0) blnEmbunOFF=12;}
      break;
      case 4:
      lcd.setCursor(6,1);lcd.print(thnAksi+thnEmbunOFF);
      lcd.setCursor(8,1);lcd.print("  ");
      if (myBtn2.wasReleased())  {thnEmbunOFF++; if(thnEmbunOFF>=100) thnEmbunOFF=00;}
      if (myBtn3.wasReleased())  {thnEmbunOFF--; if(thnEmbunOFF<=-1) thnEmbunOFF=99;}
      break; 
      case 5:
      lcd.setCursor(12,1);lcd.print("SAVE");
      lcd.setCursor(12,1);lcd.print("    ");
      if (myBtn2.wasReleased())  { 
        EEPROM.write(13,state_EmbunOFF);
        EEPROM.write(14,tglEmbunOFF);
        EEPROM.write(15,blnEmbunOFF);
        EEPROM.write(16,thnEmbunOFF);
        lcd.setCursor(00,00);lcd.print("Data tersimpan..");
        delay(1000);lcd.clear();
      }
      break;
    
    }
  if (myBtn4.wasReleased()) { lcd.clear();break;}
  }
}


void MotorAksi(){
  DateTime now = rtc.now();
  tanggal = now.day(),DEC;
  bulan = now.month(),DEC;
  tahun = now.year(),DEC;

  float totDatenow = tanggal + (bulan * 30.417) + (tahun * 365);
  
  int tahunMotorON = thnAksi + thnMotorON;
  float totMotorON = tglMotorON + (blnMotorON * 30.417) + (tahunMotorON * 365);

  int tahunMotorOFF = thnAksi + thnMotorOFF;
  float totMotorOFF = tglMotorOFF + (blnMotorOFF * 30.417) + (tahunMotorOFF * 365);

  if (totMotorON > totDatenow) {
    digitalWrite(rMotor, HIGH);
    lcd.setCursor(13, 1);
    lcd.print("*");
  }
  
  else if (totMotorON == totDatenow || totMotorON < totMotorOFF) { 
  unsigned long Motor_current = millis();
  if (Motor_current - Motor_millis >= intervalMotor) {
    if (motorState){
      digitalWrite(rMotor, motorState); //Motor non-aktif
      //Serial.println("Motor Non-Aktif");
      lcd.setCursor(13, 1);
      lcd.print("-");
      intervalMotor = MotorOff;
      }
      else {  
        digitalWrite(rMotor, motorState); // Motor Aktif     
        //Serial.println("Motor Aktif");
        lcd.setCursor(13, 1);
        lcd.print("+");
        intervalMotor = MotorOn;       
       }
       motorState = !(motorState);
       Motor_millis = Motor_current;
      }
  }
  if (totMotorOFF <= totDatenow) {
    digitalWrite(rMotor, HIGH);
    lcd.setCursor(13, 1);
    lcd.print(".");
  }
}

void EmbunAksi(){
  DateTime now = rtc.now();
  tanggal = now.day(),DEC;
  bulan = now.month(),DEC;
  tahun = now.year(),DEC;
  jam = now.hour(),DEC;
  menit = now.minute(),DEC;

  float totDatenow = tanggal + (bulan * 30.417) + (tahun * 365);

  int tahunEmbunON = thnAksi + thnEmbunON;
  float totEmbunON = tglEmbunON + (blnEmbunON * 30.417) + (tahunEmbunON * 365);

  int tahunEmbunOFF = thnAksi + thnEmbunOFF;
  float totEmbunOFF = tglEmbunOFF + (blnEmbunOFF * 30.417) + (tahunEmbunOFF * 365);
  
  if (totEmbunON > totDatenow) {
    digitalWrite(rEmbun, HIGH);
    lcd.setCursor(15, 1);
    lcd.print("*");
  }
  else if (totEmbunON == totDatenow || totEmbunON < totEmbunOFF) {
  unsigned long EMBUN_current = millis();
  if (EMBUN_current - Embun_millis >= intervalEmbun) {
    if (embunState){
      digitalWrite(rEmbun, embunState); //Semprotan non-aktif
      //Serial.println("Embun Non-Aktif");
      lcd.setCursor(15, 1);
      lcd.print("-");
      intervalEmbun = EmbunOff;
      }
      else {  
        digitalWrite(rEmbun, embunState); // semprotan aktif      
        //Serial.println("Semprotan Aktif");
        lcd.setCursor(15, 1);
        lcd.print("+");
        intervalEmbun = EmbunOn;       
       }
       embunState = !(embunState);
       Embun_millis = EMBUN_current;
      }
  }
  if (totEmbunOFF <= totDatenow) {
    digitalWrite(rEmbun, HIGH);
    lcd.setCursor(15, 1);
    lcd.print(".");
  }
}

void lampuAksi(){
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  //jika suhu lebih dari 38, lampu mati 
  if (t > 38.00){
    digitalWrite(rLampu, HIGH);
    lcd.setCursor(11, 1);
    lcd.print("-");
  }
  //jika suhu kurang dari sama dengan 37, lampu nyala
  else if (t <= 37.00){ 
    digitalWrite(rLampu, LOW); //lampu menyala
    lcd.setCursor(11, 1);
    lcd.print("+");
  }  
}

