#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 13, 8, 9, 10, 11);
int pinHumedad = A0;
int pinLedAmarillo = 2;
int pinLedVerde = 3;
int pinLedRojo = 4;
int pinBuzzer = 5;

byte gota[8] = {
B00100,
B01010,
B01010,
B11001,
B10001,
B10111,
B10111,
B01110,
};

byte alerta[8] = {
B11111,
B11111,
B11011,
B11011,
B11011,
B11111,
B11011,
B11111,
};

byte error[8] = {
B00000,
B10001,
B01010,
B00100,
B00100,
B01010,
B10001,
B00000,
};

int buzzCount = 3;

void setup()
{
    Serial.begin(9600);
    Serial.println("Sensor de Humedad");
    pinMode(pinHumedad, INPUT);
    pinMode(pinBuzzer, OUTPUT);
    pinMode(pinLedRojo, OUTPUT);
    pinMode(pinLedVerde, OUTPUT);
    pinMode(pinLedAmarillo, OUTPUT);
    lcd.begin(16, 2);
    lcd.createChar(0, gota);
    lcd.createChar(1, alerta);
    lcd.createChar(2, error);
}
void loop()
{
    int valor = map(analogRead(pinHumedad), 0, 1023, 100, 0) * (1/0.7);

    if(valor > 100){
      valor = 100;
    }

    Serial.print("Humedad: ");
    Serial.print(valor);
    Serial.println("%");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humedad: ");
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.print(valor);
    lcd.print("%");

    if(valor < 60){
      digitalWrite(pinLedRojo, HIGH);
      digitalWrite(pinBuzzer, HIGH);
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.print(" Poca Agua");

      delay(500);

      digitalWrite(pinLedRojo, LOW);
      digitalWrite(pinBuzzer, LOW);

      buzzCount = 3;
    } else if(valor > 70){
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(" Mucha Agua");
      digitalWrite(pinLedAmarillo, HIGH);
      
      if(buzzCount > 0){
        analogWrite(pinBuzzer, 255);
      }

      delay(100);
      digitalWrite(pinBuzzer, LOW);

      delay(100);
      if(buzzCount > 0){
        analogWrite(pinBuzzer, 170);
      }
      
      delay(100);
      digitalWrite(pinBuzzer, LOW);

      delay(100);
      if(buzzCount > 0){
        analogWrite(pinBuzzer, 85);
      }

      delay(100);
      digitalWrite(pinLedAmarillo, LOW);
      digitalWrite(pinBuzzer, LOW);

      if(buzzCount > 0){
        buzzCount--;
      }
    } else {
      digitalWrite(pinLedVerde, HIGH);
      delay(500);
      digitalWrite(pinLedVerde, LOW);

      buzzCount = 3;
    }

    delay(1000);
}