#define fan_pin1 13
#define fan_pin2 12
#define control_speed 27
//gas-------------------------------------------------
#define SENSOR 34
#define buzzer_gas 15
const int freq = 5000; 
const int ledChannel_1 = 0; 
const int ledChannel_2 = 2; 
const int resolution = 8;
#define light1 25
#define light2 26
#define button1 14
#define button2  33
#define button3  17
//DHT11-----------------------------------------------------
#include <FirebaseESP32.h>
#include  <WiFi.h>
#include "DHT.h" // khai báo thư viện DHT11:
#define FIREBASE_HOST "https://smart-home-a8b72-default-rtdb.firebaseio.com/"
#define FIREBASE_Authorization_key "vvHhSMAChxAxEavaTW3ly7uSyeJ4bOyZrhcv22Gr"
#define WIFI_SSID "Cc" // Change the name of your WIFI
#define WIFI_PASSWORD "12345678888" // Change the password of your WIFI
#define DHTPIN 3 // Chân dữ liệu của DHT 11 
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);
FirebaseData firebaseData;
FirebaseJson json;
//rfid---------------------------------------------------------
#include <LiquidCrystal_I2C.h> //Thu vien LCD_I2C
#include <SPI.h> //Thu vien SPI
#include <MFRC522.h> //Thu vien RFID
#include <ESP32Servo.h> //Thu vien Servo
//Gan cac chan----------------------------------------------------
#define buzzer_rfid 4 //Chan cua Buzzer
#define RST_PIN 36 //Chan RST RFID
#define SDA 5 //Chan du lieu RFID
#define servoPin 16 //Chan cua Servo
#define led_xanh 2 //Chan led bao doc dung the
#define led_do 32 //Chan led bao doc sai the
//Cac bien---------------------------------------------------------
bool state = false;
int i;
int unlock = 0;
byte user1[4] = {0xA0,0x88,0x5D,0x20}; //UID cua the trang0
int scan[4] ;

Servo myservo; //Khoi tao bien cho Servo
LiquidCrystal_I2C lcd_i2c(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
MFRC522 mfrc522(SDA, RST_PIN);
//fan---------------------------------------------------------------
bool i1=1;
bool i2 =1;
bool i3 ;
int dutycycle =0;
//led1-----------------------------------------------------------------
bool i4=1;
bool i5 =1;
bool i6 ;
//led2----------------------------------------------------------------
bool i7=1;
bool i8 =1;
bool i9 ;
//
void setup() 
{
   //DHT11------------------------------------------------------------------------
  Serial.begin(9600);
  dht.begin(); // bắt đầu đọc dữ liệu
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);

  ledcSetup(ledChannel_2, freq, resolution);
  ledcAttachPin(control_speed, ledChannel_2);

  myservo.setPeriodHertz(50); 
  myservo.attach(servoPin);
  myservo.write(15);
 
  pinMode(buzzer_rfid,OUTPUT);
  pinMode(led_xanh,OUTPUT);
  pinMode(led_do,OUTPUT);
  
 
 

  lcd_i2c.init(); // initialize the lcd
  lcd_i2c.backlight(); //Bat den nen
  lcd_i2c.setCursor(4, 0);      // move cursor to   (4, 0)
  lcd_i2c.print("Quet The"); 

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  //Serial.println(F("Scaning...."));
}
//fan-----------------------------------------------------------------------
void control_fan()
{
  pinMode(fan_pin1, OUTPUT);
  pinMode(fan_pin2, OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  pinMode(control_speed, OUTPUT);
  
    i1= digitalRead(button1);
    if(i1 == 0)
    { 
      delay(250);
      i3 = i2;
      i2 =!i2;
      if( i3 ==1)
      {
        ledcWrite(ledChannel_2, 255);
        Firebase.setString(firebaseData,"/fan",255);
      }else 
      {
        ledcWrite(ledChannel_2, 0);
        Firebase.setString(firebaseData,"/fan",0);
      }
    }else
    { 
      if(Firebase.getString(firebaseData,"/fan")) dutycycle = (firebaseData.stringData()).toInt(); 
      ledcWrite(ledChannel_2,dutycycle);
      if(dutycycle!= 0) i3=1;
      else i3 =0;
    }
    if(i3 ==1)
    {
      digitalWrite(fan_pin1, LOW);
      digitalWrite(fan_pin2, HIGH); 
    }else 
    {
      digitalWrite(fan_pin1, LOW);
      digitalWrite(fan_pin2, LOW);
    } 
 //}
}//*/
//light1----------------------------------------------------------------------------------
void control_light1()
{//isableCore0WDT();
  pinMode(light1, OUTPUT);
  pinMode(button2,INPUT_PULLUP);
 // while(1)
 //{
  i4= digitalRead(button2);
  if(i4 == 0)
  { 
    delay(50);
    i6 = i5;
    i5 =!i5;
    if(i6 ==1) digitalWrite(light1, HIGH);
    else digitalWrite(light1, LOW);
    Firebase.setString(firebaseData,"/light1",String(i6));
  }
  else 
  if(Firebase.getString(firebaseData,"/light1")) 
  {
    i6 = (firebaseData.stringData()).toInt(); 
    if(i6 ==1) digitalWrite(light1, HIGH);
    else digitalWrite(light1, LOW);
  }
//}
}
//light2---------------------------------------------------------------------
void control_light2()
{
  pinMode(light2, OUTPUT);
  pinMode(button3,INPUT_PULLUP);
  //while(1)
  //{
    i7= digitalRead(button3);
    if(i7 == 0)
    { 
      delay(50);
      i9 = i8;
      i8 =!i8;
      if(i9 == 1 ) digitalWrite(light2, HIGH);
      else digitalWrite(light2, LOW);
      Firebase.setString(firebaseData,"/light2",String(i9));
    }
    else 
    if(Firebase.getString(firebaseData,"/light2")) 
    {
      i9 = (firebaseData.stringData()).toInt(); 
      if(i9 ==1) digitalWrite(light2, HIGH);
      else digitalWrite(light2, LOW);
    }
 // }
}//*/
//DHT11---------------------------------------------------------------------------------
void control_DHT11()
{
 // while(1)
  //{
    delay(1000);
    //Đọc dữ liệu trả về vào 3 biến:
    float h = dht.readHumidity();// Đọc độ ẩm
    float t = dht.readTemperature(); // Đọc nhiệt độ theo độ C
    //Kiểm tra dữ liệu trả về, nếu dữ liệu đọc được là nan (not a number) thì in ra màn hình: Failed to read from DHT sensor!
    if (isnan(h) || isnan(t)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    //In dữ liệu nhận được ra terminal:
    //Serial.print("Do am: ");
    //Serial.print(h);
    //Serial.print(" %\t");
    //Serial.print("Nhiet do:  ");
    //Serial.print(t);
    //Serial.println(" *C");
    Firebase.setFloat(firebaseData, "/humidity",h ); //Cập nhật giá trị độ ẩm lên firebase
    Firebase.setFloat(firebaseData,"/temperature",t);//Cập nhật giá trị nhiệt độ lên firebase
  //  }
}
//gas----------------------------------------------------------------------------------------------------------------
void control_gas()
{
  int sensorValue = 0;  // variable to store the value coming from the sensor
  pinMode(buzzer_gas, OUTPUT);
  //while(1)
  //{
    sensorValue = analogRead(SENSOR);
    //Serial.print("Value: "); 
    //Serial.println(sensorValue);
    if(sensorValue > 1000)digitalWrite(buzzer_gas,HIGH);
    else digitalWrite(buzzer_gas,LOW);
    Firebase.setFloat(firebaseData, "/gas",sensorValue);
    delay(500);
 // }
}
//rfid---------------------------------------------------------------------------------------------------------------------
void control_rfid()//void *pvParameters
{

    if ( ! mfrc522.PICC_IsNewCardPresent()) return; //Kiem tra xem co the quet hay khong
    if ( ! mfrc522.PICC_ReadCardSerial())  return; //Kiem tra xem doc the co thanh cong hay khong
    digitalWrite(buzzer_rfid,HIGH);
    delay(300);
    digitalWrite(buzzer_rfid,LOW);
    for (i = 0; i < 4; i++) {
    scan[i] = mfrc522.uid.uidByte[i];
    unlock = 1;
    if(scan[i] != user1[i]) unlock = 0;
  }
  if (unlock == 1)
  {
    if(state == false)
    {
      state = true;
      digitalWrite(led_xanh,HIGH);
      digitalWrite(led_do,LOW);
      myservo.write(60);
      lcd_i2c.clear();
      lcd_i2c.setCursor(4, 0);
      lcd_i2c.print("The Dung");
      lcd_i2c.setCursor(5, 1);
      lcd_i2c.print("Mo Cua");
      delay(1000); 
      digitalWrite(led_xanh,LOW);      
    }
    else 
    {
      state = false;
      digitalWrite(led_xanh,HIGH);
      digitalWrite(led_do,LOW);
      myservo.write(15);
      lcd_i2c.clear();
      lcd_i2c.setCursor(4, 0);
      lcd_i2c.print("The Dung");
      lcd_i2c.setCursor(4, 1);
      lcd_i2c.print("Dong Cua");
      delay(1000); 
      digitalWrite(led_xanh,LOW);
    } 
  }
  else 
  {
    digitalWrite(led_do,HIGH);
    digitalWrite(led_xanh,LOW);
    lcd_i2c.clear();
    lcd_i2c.setCursor(4, 0);
    lcd_i2c.print("The Sai");
    lcd_i2c.setCursor(4, 1);
    lcd_i2c.print("Quet lai");
    delay(1000); 
    digitalWrite(led_do,LOW);
  }
  lcd_i2c.clear();
  lcd_i2c.setCursor(4, 0);      // move cursor to   (4, 0)
  lcd_i2c.print("Quet The"); 
}
void loop()
{
  control_rfid();
control_DHT11();
  control_gas();
  control_light1();
  control_light2();
 control_fan();
}
