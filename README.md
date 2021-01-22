# password guesser

Projekt polegający na odgadywaniu hasła.
Początkowo wygenerowany kod 4 cyfr nalezy odgadnąć próbojąc wpisać odpowiednią kombinacje.
Następnie pojawia sie podpowiedz czy liczba jest poprawna, czy moze jest za mała bądź za duza.

Po prawidłowym odgadnieciu wygenerowanego kodu pojawi się ilośc prób (pomyłek)



<h2>Wykaz potrzebnych elementów:</h2>
<ul>arduino Uno</ul>
<ul>ekran 1602A</ul>
<ul>konwerter i2c</ul>
<ul>klawiatura 4x4</ul>

``` C++

#include "Keypad.h"
#include <Wire.h>   
#include <LiquidCrystal_I2C.h>  //uzyte biblioteki

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; //definiowanie macierzy klawiatury 4x4
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3' , 'A'},
  {'4','5','6' , 'B'},
  {'7','8','9' , 'C'},
  {'*','0','#' , 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};  //Piny, do których podłączamy wyprowadzenia od rzędów
byte colPins[COLS] = {6, 7, 8 , 9}; //Piny, do których kolumn wyprowadzenia od kolumn

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char wybrane[4];       
String los1;        //zamiana z macierzy char na mozliwość poźniejszego porównania wyniku
String los2;
String los3;
String los4;
String wyb1;
String wyb2;
String wyb3;
String wyb4;

char key;
void setup(){
  lcd.begin(16,2);   
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(0,0); 
  
  delay(500);
 
 Serial.begin(9600);
 
   randomSeed(analogRead(A0));      //genererowanie pseudo losowego hasła
  int wylosowana[4] = {random(0,10), random(0,10), random(0,10), random(0,10)};

 
  
 
  los1 += wylosowana[0];    //przypisywanie elementów macierzy losowania
  los2 += wylosowana[1];
  los3 += wylosowana[2];
  los4 += wylosowana[3];

  
  lcd.setCursor(0,0);
 lcd.print("zgadnij password");
}
  
void loop(){
 lcd.backlight();
 int p=0;   //zmienna p odpowiedzialna za zliczanie ilości powtórek
 int i =0;  //zmienna i odpowiedzialna za czas trwania pętli i=4 -> wszystkie cyfry są poprawne
 
 
 while(i < 4)
 {
  key = keypad.getKey();
  if(key != NO_KEY)
  {
    wybrane[i] = key;
    i++;
    
  }
 }

   for(int i = 0; i <4; i++)
  {
    Serial.print(wybrane[i]);
    Serial.print(", ");
  }

  wyb1 += wybrane[0];
  wyb2 += wybrane[1];
  wyb3 += wybrane[2];
  wyb4 += wybrane[3];
   
   
   lcd.setCursor(0,1);
  lcd.print(wyb1);
  lcd.print(wyb2);
  lcd.print(wyb3);
  lcd.print(wyb4);      //drukowanie liczb wpisanych przez uzytkownika
  lcd.setCursor(15,0);
  lcd.print(" ");
wybor:      //miejsce powrótne goto

p++;
  
    if (wyb1 == los1)       //kazdy głębszy if odpowiada za sprawdzanie kolejnej cyfry 
    {                       //poźniejsze else i if else odpowiada za naprowadzenie uytkownika
                            //czy podana liczba jest za mała lub za duza
    lcd.setCursor(0,1); 
    lcd.print(wyb1);
    if (wyb2 == los2)
    {
      lcd.setCursor(1,1); 
    lcd.print(wyb2);
      if(wyb3 == los3)
      {
        lcd.setCursor(2,1); 
    lcd.print(wyb3);
        if(wyb4 == los4)
        {
          lcd.setCursor(3,1); 
    lcd.print(wyb4);
          Serial.print("\nGG!");
          Serial.print("\nWylosowany ciąg to: ");       //mozliwosc sprawdzenia programu w podglądzie portu szeregowego
          Serial.print(los1);
          Serial.print(los2);
          Serial.print(los3);
          Serial.print(los4);
          lcd.setCursor(0,0);
          lcd.print("gratulacje         ");
          lcd.setCursor(8,1);                   //tekst drukowany na ekranie 16x2
          lcd.print(p);
          lcd.print("-prob");
        }
        else if(wyb4 > los4)
  {
    Serial.print("\n\nPodana czwarta liczba jest za duża!");
    Serial.print("\nPodaj mniejszą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    Serial.print(wyb4);
    lcd.setCursor(0,0);
    lcd.print("4 liczba jest +");   //"liczba jest +" oznacza ze jest za duza
    lcd.setCursor(3,1); 
    lcd.print(wyb4);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[3] = key;
      wyb4 = wybrane[3];
      i++;
      
    }
    
    }
    goto wybor;
  }
  else
  {
    Serial.print("\n\nPodana czwarta liczba jest za mała!");
    Serial.print("\nPodaj większą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    Serial.print(wyb4);
    lcd.setCursor(0,0);
    lcd.print("4 liczba jest -");
  lcd.setCursor(3,1); 
    lcd.print(wyb4);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[3] = key;
      wyb4 = wybrane[3];
      i++;
      
    }
    
    }
    goto wybor;
    
  }
      }
      else if(wyb3 > los3)
  {
    Serial.print("\n\nPodana trzecia liczba jest za duża!");
    Serial.print("\nPodaj mniejszą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    Serial.print(wyb3);
    lcd.setCursor(0,0);
    lcd.print("3 liczba jest +");
    lcd.setCursor(2,1); 
    lcd.print(wyb3);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[2] = key;
      wyb3 = wybrane[2];
      i++;
      
    }
    
    }
    goto wybor;
  }
  else
  {
    Serial.print("\n\nPodana trzecia liczba jest za mała!");
    Serial.print("\nPodaj większą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    Serial.print(wyb3);
    lcd.setCursor(0,0);
    lcd.print("3 liczba jest -");
lcd.setCursor(2,1); 
    lcd.print(wyb3);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[2] = key;
      wyb3 = wybrane[2];
      i++;
      
    }
    
    }
    goto wybor;
    
  }
    }
    else if(wyb2 > los2)
  {
    Serial.print("\n\nPodana druga liczba jest za duża!");
    Serial.print("\nPodaj mniejszą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    Serial.print(wyb2);
    lcd.setCursor(0,0);
    lcd.print("2 liczba jest +");
    lcd.setCursor(1,1); 
    lcd.print(wyb2);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[1] = key;
      wyb2 = wybrane[1];
      i++;
      
    }
    
    }
    goto wybor;
  }
  else
  {
    Serial.print("\n\nPodana druga liczba jest za mała!");
    Serial.print("\nPodaj większą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    lcd.setCursor(0,0);
    lcd.print("2 liczba jest -");
    Serial.print(wyb2);
    lcd.setCursor(1,1); 
    lcd.print(wyb2);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[1] = key;
      wyb2 = wybrane[1];


      
      i++;
      
    }
    
    }
    goto wybor;
    
  }
  }
  else if(wyb1 > los1)
  {
    Serial.print("\n\nPodana pierwsza liczba jest za duża!");
    Serial.print("\nPodaj mniejszą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    lcd.setCursor(0,0);
    lcd.print("1 liczba jest +");
    Serial.print(wyb1);
    lcd.setCursor(0,1); 
    lcd.print(wyb1);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[0] = key;
      wyb1 = wybrane[0];
      i++;

   
      
    }
    
    }
    goto wybor;
  }
  else
  {
    Serial.print("\n\nPodana pierwsza liczba jest za mała!");
    Serial.print("\nPodaj większą liczbę!");
    Serial.print("\nWybrana liczba to: ");
    lcd.setCursor(0,0);
    lcd.print("1 liczba jest -");
    Serial.print(wyb1);
    lcd.setCursor(0,1); 
    lcd.print(wyb1);
    int i= 0;
    while(i<1)
    {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      wybrane[0] = key;
      wyb1 = wybrane[0];
      i++;
    
      
    }
    
    }
    goto wybor;
    
  }
 
  
 
  
}
```

