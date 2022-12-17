//Creado por Angel David Caicedo Anchico y Arturo Restrepo Ruiz
/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>
#include "AsyncTaskLib.h"

#define LED_GREEN 53
#define LED_RED 51
#define LED_BLUE 49

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns

const int rs = 12, en = 11, d4 = 14, d5 = 15, d6 = 16, d7 = 17;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 4, 3}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char pass[4] = "1111";
char ingresaPass[4];
int indice=0;
int intentos=0;

void ingresarClave();
void validarClave();
void tiempoEspera();

AsyncTask asyncTask1(1,true, ingresarClave);
AsyncTask asyncTask2(1, validarClave);
AsyncTask asyncTask3(1, tiempoEspera);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Ingrese Clave:");
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  Serial.begin(9600);
  asyncTask1.Start(); 
  
}

void loop() {
  asyncTask1.Update();
  asyncTask2.Update();
  asyncTask3.Update();
}

void ingresarClave(){
   char customKey = customKeypad.getKey();
   if(indice == 4){ 
  asyncTask2.Start(); 
  }
  if (customKey){
    if(indice == 0){
      lcd.clear();
    }
    ingresaPass[indice]=customKey;
    indice++;
    Serial.println(customKey);
    lcd.print(customKey);
  }
  
}

void validarClave(){
 // if(indice == 4){   
    if(strncmp(pass,ingresaPass,4)==0){
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.print("Correcto");
      lcd.print("Correcto");
      digitalWrite(LED_BLUE, LOW); 
      digitalWrite(LED_GREEN, HIGH);     
    }else{
      lcd.clear();
      lcd.setCursor(0,0);
      digitalWrite(LED_BLUE, HIGH);
      Serial.print("Incorrecto");
      lcd.print("Incorrecto");
      delay(2000);
      lcd.clear();
      lcd.print("Intente de nuevo:");
      intentos++;   
    }
    indice = 0;
    if(intentos==3){
      lcd.clear();
      lcd.setCursor(0,0);
      digitalWrite(LED_GREEN, LOW); 
      digitalWrite(LED_BLUE, LOW); 
      Serial.print("Sistema Bloqueado");
      lcd.print("Bloqueado");
      digitalWrite(LED_RED, HIGH); 
      asyncTask3.Start();  
      
    }
 // }
}

void tiempoEspera(){
  delay(5000);
  digitalWrite(LED_RED, LOW);
  lcd.clear();
  lcd.print("Ingrese Clave:");
  intentos = 0;  
  indice = 0;
}
