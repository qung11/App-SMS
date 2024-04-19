#include<SoftwareSerial.h>
SoftwareSerial SIM900(2, 3);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

const int led = 8;
const int fan = 13;
int tam,tt;
char tempchar;
const unsigned int buff_size = 160 ;
char buff1[buff_size];
int countDown;
int k=0, i=0,index=0,j=0;
char sdt[13];
char number[13]="+84389662562"; 
    // ghi vào số điện thoại của điện thoại
char on[]="1234 ON1";
char off[]="1234 OFF1";
boolean ktsdt = true;
void setup()
{
  Serial.begin (9600);
   //thiết lập tốc độ baud giao tiếp máy tính
  SIM900.begin (9600);
    //thiết lập tốc độ baud giao tiếp SIM900
  pinMode (led,OUTPUT);
  pinMode (fan,OUTPUT);
   //cấu hình chân số 5 là ngõ ra, kêt nối led
  digitalWrite (led,LOW); //cho led tắt
  digitalWrite (fan,LOW); //cho led tắt
  SIM900.println ("AT+CMGD=1,4\r\n");
   //xóa tất cả tin nhắn trong bộ nhớ
  delay (20);
  SIM900.println ("ATE0\r\n"); //bỏ echo của module sim
  delay(1000);
  SIM900.println ("AT+CNMI =2,2,0,0,0\r\n");
   // đặt chế độ hiện nội dung tin nhắn
  delay (1000);
  SIM900.print ("AT+CMGF=1\r\n");
    //cho phép module sim nhận và gửi tin
  delay (500) ;
//timer
  // TCCR1A = 0;
  // TCCR1B = (1 << CS12) | (1 << CS10);
  // TIMSK1 = (1 << TOIE1);
  // TCNT1 = 49911;
  // sei();  // Bật ngắt tổng thể
  //countDown=13;
 // tt=1;tam=1;
}

// void timerInterrupt() 
// {
//   countDown--;  // Giảm biến đếm ngược mỗi lần ngắt
// }
void loop ()
{
  receive_uart ();
  delay (200);
  xu_ly_sms ();
  xoa_buffer () ;

  //  if (countDown > 0) 
  // {
  //   TCCR1B = (1 << CS12) | (1 << CS10);
  //   lcd.setCursor(5, 0);
  //   lcd.print("FAN ON    ");
  //   lcd.setCursor(3, 1);
  //   lcd.print("OFF TIME:");
  //   lcd.setCursor(13, 1);
  //   lcd.print(countDown);
  //    if (countDown < 10) 
  //    {
  //     lcd.setCursor(14, 1);
  //    lcd.print(" ");
  //    }
  //   digitalWrite(led, 1);
  //   digitalWrite(fan, 1);
  // }
  // // Kiểm tra nếu đếm ngược về 0 thì dừng timer và hiển thị thông báo trên LCD
  // else if (countDown == 0) {
  //   lcd.clear();
  //   lcd.setCursor(5, 0);
  //   lcd.print("FAN OFF!!!");
  //   digitalWrite(led, 0);
  //   digitalWrite(fan, 0);
  //   TCCR1B = 0;
  //   delay(3000);   
  //   countDown =12 ; 
  // }
}

// ISR(TIMER1_OVF_vect) 
// {
//   TCNT1 = 49911;     // Thiết lập lại giá trị đếm ban đầu
//   timerInterrupt();  // Gọi hàm ngắt timer
// }
void sendSMS (String message)
{
  SIM900.print ("AT+CSCS =\"GSM\"\r\n");
    //lệnh AT để gửi tin
  SIM900_response (500);
  SIM900.print ("AT+CMGF=1\r\n");
    //cho phép gửi và nhận tin nhắn
  SIM900_response (500);
  SIM900.print ("AT+CMGS=\"+84985769807\"\r");
    // gửi tin nhắn đến số điện thoại
  SIM900_response (500);
  SIM900.print (message);
    //gửi nội dung tin nhắn
  SIM900_response (500);
  SIM900.print ((char)26);
    //kết thúc lệnh gửi
  SIM900_response (5000) ;
}
    // dùng timer delay kết hợp đọc SIM900 và giao tièp máy tính
void SIM900_response (int time)
{
  unsigned long tnow =millis();
  while((millis()-tnow)<time) 
  {
    if (SIM900.available()) 
    {
      tempchar=(char)SIM900.read() ;
      if (tempchar=='r')
        {
          Serial.print ("/R/");
        }
      else if(tempchar== '\n')
       {
         Serial.println("/N/");
        }
      else Serial.print(tempchar);
    }
  }
}
void receive_uart()
    //hàm nhận tin nhắn và lưu vào mảng buffl
{
  k=0;
  while((SIM900.available()==0)){}
  while (SIM900.available()>0)
  {
    buff1[k++]=SIM900.read();
  }
}
void xu_ly_sms() // xử lý chuỗi vừa nhận
{
    //tìm vị trí số điện thoại bắt đầu bằng "+8" 
  for (i=0;i<k; i++)
  {
    if((buff1[i] =='+')&&(buff1[i+1]=='8')) 
    {
      index=i;
      break;
    }
  }
    //tách số điện thoại từ mảng buff1 chứa vào mảng sdt
  j=0;
  for (i=index;i<(index+12);i++) 
  {
    sdt [j]=buff1[i];
    j++;
  }
  ktsdt=true;
  // for (i=0;i<13; i++)
  // {
  //   if (sdt[i]!=number[i])
  //   {
  //     ktsdt=false;
  //     break;
  //   }
  // }
  if(ktsdt==true)
   {
      if (strstr(buff1,"ON")!=NULL)
       { tt=1;
          digitalWrite(led,HIGH);
          //sendSMS("the light is ON");
          digitalWrite (fan,HIGH);
        }
      if(strstr(buff1,"OFF")!=NULL)
       {
         tt=0;
          digitalWrite (led,LOW);
          
          digitalWrite (fan,LOW);
         // sendSMS("the light is OFF");
       }
    }
  Serial.print("buff ");  Serial.println(buff1); 
  Serial.print("ktrasdt "); Serial.println(ktsdt);
  Serial.print("tthai "); Serial.println(tt);
}
void xoa_buffer() //xóa bộ đệm
{
  for (i=0; i<=k;i++)
   {
     buff1[1]=NULL;
    }
  for (i=0;i<13;i++)
   {
      sdt[i]=NULL;
    }
  k=0;j=0;i=0;index=0; 
}