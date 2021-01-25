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

<h3>Zdjęcia poglądowe</h3>

![IMG_4253](https://user-images.githubusercontent.com/58706514/105529340-8ba4d100-5ce6-11eb-812f-ef40bff5c9da.jpg)

![IMG_4254](https://user-images.githubusercontent.com/58706514/105529379-9bbcb080-5ce6-11eb-98e7-0df25d68bf90.jpg)

![IMG_4257](https://user-images.githubusercontent.com/58706514/105529411-a70fdc00-5ce6-11eb-83fc-4b1014b6c1b5.jpg)

<h3>Film poglądowy</h3>

https://drive.google.com/file/d/1GX0TifZx9D6Ksmyy8wN6SHwRR8NqQbrZ/view?usp=sharing

<h3>Zasada działania klawiatury</h3>

``` C++
#include "Keypad.h"

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
```
Zasada działania klawatury opiera się na macierzy.
8 wyprowadzeń podzielone są na pół. 4 wyprowadzenia (piny 2, 3, 4, 5) odpowiadają za rząd,
pozostałe 4 (6, 7, 8, 9) odpoiwadają za kolumne.
Zatem nie działa ona jak zwyczajna klawiatura, tylko przy wciśnięciu przycisku odczytywana jest wartość przypisana do kolumny i rzędu zapisanej macierzy. 

``` C++
char keys[ROWS][COLS] = {
  {'1','2','3' , 'A'},
  {'4','5','6' , 'B'},
  {'7','8','9' , 'C'},
  {'*','0','#' , 'D'}
};
```

Przykładowo po wciśnięciu przycisku ```'5'``` program widzi drugi rząd ``` [ROWS] ``` i drugą kolumne ``` [COLS] ```. I odczytuje przypisaną wartość ```'5'```.

<h3> Komunikacja z wyświetlaczem </h3>

``` C++
#include <LiquidCrystal_I2C.h>  

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
  lcd.begin(16,2);   
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(0,0); 
  lcd.print("zgadnij password");
```

Dzięki zastosowaniu komunikacji ``` I2C ``` należy podłączyć jedynie wyprowadzenia VCC i GND z wyściami VCC i GND w mikrokontrolerze Arduino. Tak samo jest z wyprowadznieami SDA i SCL podłączamy je z tak samo oznaczonymi wyprowadzeniami w Arduino.

``` C++
#include <LiquidCrystal_I2C.h>  
```

Biblioteka ``` LiquidCrystal_I2C ``` nie jest standardowo zainstalowana więc należy ją samodzielnie dołączyć do bibliotek.

``` C++
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

W moim przypadku adres ekranu to ``` 0x27 ```. 
Aby ustalić adres ekranu należy sprawdzić adres magistrali i2c, aby to zrobić należy posłużyć się poniższym skryptem i sprawdzić wynik wydrukowany w monitorze szeregowym. 

``` C++
#include <Wire.h>
void setup()
  {
    Wire.begin();
    Serial.begin(9600);
    Serial.println("\nI2C Scanner");
  }
void loop()
  {
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) 
      {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
          {
            Serial.print("I2C device found at address 0x");
            if (address<16) 
            Serial.print("0");
            Serial.print(address,HEX);
            Serial.println(" !");
            nDevices++;
          }
        else if (error==4) 
          {
            Serial.print("Unknow error at address 0x");
            if (address<16) 
            Serial.print("0");
            Serial.println(address,HEX);
          } 
      }
    if (nDevices == 0)
    Serial.println("No I2C devices found\n");
    else
    Serial.println("done\n");
    delay(5000); // wait 5 seconds for next scan
    
  }
  ```
  
Po prawidłowej konfuguracji i podłączeniu można już w pełni korzystać z możliwości biblioteki.

``` C+++
lcd.backlight(); // zalaczenie podwietlenia 
lcd.setCursor(0,0); 
lcd.print("zgadnij password");
```

``` lcd.backlight() ``` odpowiada za podświetlenie ekranu. 
```lcd.setCursor(0,0) ``` odpowiada za ustawienie kursora na ekranie, pierwsza liczba odpowiada za kolumune ```[0;15]```, druga liczba to rząd ```[0;1]```.

<h3> Pełny kod arduino </h3>

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

