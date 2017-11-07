
#include <Servo.h>

#include <LCD_1602_RUS.h>

#define LEDpin 11

#define IRpin 17         // питание фототранзистора

#define IRsens1 14        // сигнал фототранзистора
#define IRsens2 22        // сигнал фототранзистора

Servo myservo;
bool isSigned;
LCD_1602_RUS lcd(0x3f, 20, 4); 

const int MoneyCount = 7;
int moneySignals[MoneyCount] = {
                                  52,
                                  95,
                                  872,
                                  58,
                                  265,
                                  520,
                                  773
                               };
                               
char* moneyNames[MoneyCount] = {
                                  "1 kopeyka",
                                  "2 kepeyki",
                                  "5 kopeek",
                                  "10 kopeek",
                                  "15 kopeek",
                                  "20 kopeek",
                                  "50 kopeek"
                                };



int GetCurrencyIdBySignal(int sign)
{
  int signalsSub[MoneyCount];
  int minimalSub = -500;
  int minimalSubId = -1;
  for(int i = 0; i < MoneyCount; i++)
  {
    signalsSub[i] = moneySignals[i] - sign;
    
    if(minimalSubId == -1 || minimalSub > abs(signalsSub[i]))
    {
      minimalSub = abs(signalsSub[i]);
      minimalSubId = i;
    }
  }

  return minimalSubId;
  
  return -1;
}

void setup() {
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, OUTPUT);

  
  digitalWrite(IRpin, 1);
  isSigned = false;

  lcd.init();
  lcd.backlight();

  lcd.clear();

  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(L"HelloWorld123");
    lcd.setCursor(0, 1);
    lcd.print(L"HelloWorld123");
    lcd.setCursor(0, 2);
    lcd.print(L"HelloWorld123");
    lcd.setCursor(0, 3);
    lcd.print(L"HelloWorld123");


    //delay();
}




void loop() {

  delay(500);

  
    lcd.setCursor(3, 0);
    lcd.print(L"Сервис");
    
  Serial.begin(9600);
  int min1 = -10; 
  int min2 = -10; 
  delay(500);
  int initialSignal1 = analogRead(IRsens1);
  int initialSignal2 = analogRead(IRsens2);
  const int SignalsCoun = 5;
  int signals[SignalsCoun] = {initialSignal2,initialSignal2,initialSignal2,initialSignal2,initialSignal2};
  int max2;
  do{
    
    int sens_signal1 = analogRead(IRsens1);
    int sens_signal2 = analogRead(IRsens2);


    for(int i = 0; i < SignalsCoun-1; i++)
    {
      signals[i] = signals[i+1];
    }
    signals[SignalsCoun-1] = sens_signal2;

    if(sens_signal2 - initialSignal2 > 2 )
    {
      isSigned = false;
      max2 = max(sens_signal2,max2);
    } 
    else
    if(abs(sens_signal2 - initialSignal2) < 2 )
    {
      if(!isSigned)
      {
        
        lcd.clear();

        //Serial.println(max2);
        if(GetCurrencyIdBySignal(max2) != -1)
        {
          Serial.println(String(moneyNames[GetCurrencyIdBySignal(max2)]));
          
          lcd.setCursor(7, 2);
          lcd.print(moneyNames[GetCurrencyIdBySignal(max2)]);
          if(max2 >= 900)
          {
            lcd.setCursor(7, 3);
            lcd.print("Vladik");
          }
        }
        else
        {
          Serial.println("Ошибка");
          Serial.println(max2);
        }
        

        
        myservo.attach(7);
        float asdasd = 180.0/1023.0;
        myservo.write((int)((float)max2 * asdasd));
        delay(500);
        myservo.detach();
        
        delay(1000);

        lcd.clear();
    
      }
      isSigned = true;
      max2 = 0;
    } 
  }while(true);
}
