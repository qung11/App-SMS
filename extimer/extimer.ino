#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

// Khai báo biến đếm ngược và giá trị ban đầu
int countDown;
int tt=0,tg;
const int led = 8;
const int fan = 13;
const int thap = 0;
const int cao= 1;
// Hàm ngắt timer
void timerInterrupt() 
{
  countDown--;  // Giảm biến đếm ngược mỗi lần ngắt
}

void setup() {
  // Khởi tạo LCD
  Serial.begin(9600);
  pinMode(led, OUTPUT);    
  pinMode(fan, OUTPUT);   
  //pinMode(button, INPUT);  //Cài đặt chân D11 ở trạng thái đọc dữ liệu
  // Khai báo ngắt timer với Timer1 và thời gian 1 giây
  lcd.init();
  lcd.backlight();
  // Hiển thị giá trị ban đầu lên LCD
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("THIET BI TAT");
  delay(7000); // In chuỗi ký tự lên LCD
    TCCR1A = 0;
  TCCR1B = (1 << CS12) | (1 << CS10);
  TIMSK1 = (1 << TOIE1);
  TCNT1 = 49911;
  sei();  // Bật ngắt tổng thể
  countDown = 15 ;
}
void loop() {
  //buttonStatus = digitalRead(button);
  // Serial.println(buttonStatus);
  // Serial.println(countDown);
  // delay(100);
  // if (digitalRead(button) == HIGH) {
  //   countDown = 10;                      // Reset biến đếm ngược về giá trị ban đầu
  //   TCCR1B = (1 << CS12) | (1 << CS10);  // Bật lại chế độ ngắt cho Timer1
  // }
  // Hiển thị giá trị đếm ngược lên LCD
  
if (tt<2)
{
  if (countDown > 0) 
  {
    TCCR1B = (1 << CS12) | (1 << CS10);
    lcd.setCursor(1, 0);
    lcd.print("BAT 2 THIET BI");
    lcd.setCursor(3, 1);
    lcd.print("OFF TIME:");
    lcd.setCursor(13, 1);
    lcd.print(countDown);
     if (countDown < 10) 
     {
      lcd.setCursor(14, 1);
     lcd.print(" ");
     }
    digitalWrite(led, cao);
    digitalWrite(fan, cao);

  }

  // Kiểm tra nếu đếm ngược về 0 thì dừng timer và hiển thị thông báo trên LCD
  else if (countDown == 0) 
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("TAT 2 THIET BI");
    digitalWrite(led, thap);
    digitalWrite(fan, thap);
    
    tt=tt+1;
    delay(6000);
  if(tt==0) {countDown=15;}
  if(tt==1) {countDown=30;}
  if(tt==2) {countDown=20;} 
  }

}
else if(tt==2)
{
  if (countDown == 15) {lcd.clear();}
  
  lcd.setCursor(0, 0);
  lcd.print("CHE DO THU CONG");
  digitalWrite(led, thap);
  digitalWrite(fan, cao);
  lcd.setCursor(2, 1);
  lcd.print("TIME:");
  lcd.setCursor(9, 1);
  tg=20-countDown;
  lcd.print(tg);    
  if (countDown < 3)
  {
  digitalWrite(led, thap);
  digitalWrite(fan, thap);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHE DO THU CONG");
 // TCCR1B = 0;
  tt=tt+1;
  }
}
while(tt==3){};
}

// Hàm ngắt timer
ISR(TIMER1_OVF_vect) {
  TCNT1 = 49911;     // Thiết lập lại giá trị đếm ban đầu
  timerInterrupt();  // Gọi hàm ngắt timer
}
