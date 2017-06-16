#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <TinyGPS++.h>
#include <LCD5110_Basic.h>

SoftwareSerial mySerial(5, 6); // RX, TX
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
LCD5110 LCD(8, 9, 10, 11, 12);
int a;
byte month, day, hour, minute, second, hundredths;
int year;
float flat, flon;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode(7, OUTPUT);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  a = sizeof(TinyGPS);

  digitalWrite(7, HIGH);
  LCD.InitLCD();
  LCD.setContrast(70);
  LCD.clrScr();
  LCD.setFont(SmallFont);
  LCD.print("uBlox Neo ", 0, 1);
  LCD.setFont(SmallFont);
  LCD.print("GPS Testi ", 0, 10);
  LCD.setFont(SmallFont);
  LCD.print("Metin AKKIN", 0, 20);
  LCD.setFont(SmallFont);
  LCD.print("GPS'in Boyutu:", 0, 30);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(a, 0, 35);

  delay(4000);
}

void loop() // run over and over
{
  bool newdata = false;
  unsigned long start = millis();

  // Every 5 seconds we print an update
  while (millis() - start < 5000) {
    if (mySerial.available()) {
      char c = mySerial.read();
      // Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) {
        newdata = true;
        // break;  // uncomment to print new data immediately!
      }
    }
  }

  if (newdata) {
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);

    Serial.println("-------------");
    Serial.println();
  }

}

void gpsdump(TinyGPS &gps)
{
  int uydu = gps.satellites();
  long lat, lon;
  // float flat, flon;
  unsigned long age, date, time, chars;
  float rakim = gps.f_altitude();
  // int year;
  //  byte month, day, hour, minute, second, hundredths;

  unsigned short sentences, failed;
  float  kmph = (gps.f_speed_kmph() * 1.609344);
  LCD.clrScr();
  LCD.setFont(SmallFont);
  LCD.print("Hiz (km/h) ", 0, 1);
  LCD.setFont( MediumNumbers);
  LCD.printNumI(static_cast<int>(kmph), 0, 10);

  LCD.setFont(SmallFont);
  LCD.print("Tarih: ", 0, 31);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<int>(day), 0, 38);
  LCD.setFont(SmallFont);
  LCD.print("/", 12, 45);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<int>(month), 18, 38);
  LCD.setFont(SmallFont);
  LCD.print("/", 30, 45);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<int>(year), 36, 38);
  delay(1000);

  LCD.clrScr();
  LCD.setFont(SmallFont);
  LCD.print("Saat: ", 0, 0);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<int>(hour) + 3, -5, 10);
  LCD.setFont(SmallFont);
  LCD.print(":", 25, 20);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<int>(minute), 30, 10);
  LCD.setFont(SmallFont);
  LCD.print(":", 52, 20);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<int>(second), 57, 10);
  LCD.setFont(SmallFont);
  LCD.print("Rakim: ", 0, 31);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<float>(rakim), 0, 36);
  delay(1000);

  LCD.clrScr();
  LCD.setFont(SmallFont);
  LCD.print("Enlem: ", 0, 0);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<float>(flat), 0, 10);
  LCD.setFont(SmallFont);
  LCD.print("Boylam: ", 0, 30);
  LCD.setFont(MediumNumbers);
  LCD.printNumI(static_cast<float>(flon), 0, 38);
  delay(1000);

  LCD.clrScr();
  LCD.setFont(SmallFont);
  LCD.print("Veri Alinan", 0, 0);
  LCD.setFont(SmallFont);
  LCD.print("Uydu Sayisi:", 0, 10);
  LCD.setFont(BigNumbers);
  LCD.printNumI(static_cast<float>(uydu), 0, 30);
  delay(1000);

  gps.get_position(&lat, &lon, &age);
  Serial.print("Lat/Long(10^-5 deg): "); Serial.print(lat); Serial.print(", "); Serial.print(lon);
  Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

  // On Arduino, GPS characters may be lost during lengthy Serial.print()
  // On Teensy, Serial prints to USB, which has large output buffering and
  //   runs very fast, so it's not necessary to worry about missing 4800
  //   baud GPS characters.

  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);
  Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

  gps.get_datetime(&date, &time, &age);
  Serial.print("Date(ddmmyy): "); Serial.print(date); Serial.print(" Time(hhmmsscc): ");
  Serial.print(time);
  Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  Serial.print("Date: "); Serial.print(static_cast<int>(month)); Serial.print("/");
  Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(static_cast<int>(hour)); Serial.print(":");
  Serial.print(static_cast<int>(minute)); Serial.print(":"); Serial.print(static_cast<int>(second));
  Serial.print("."); Serial.print(static_cast<int>(hundredths));
  Serial.print("  Fix age: ");  Serial.print(age); Serial.println("ms.");


  Serial.print("Alt(cm): "); Serial.print(gps.altitude()); Serial.print(" Course(10^-2 deg): ");
  Serial.print(gps.course()); Serial.print(" Speed(10^-2 knots): "); Serial.println(gps.speed());
  Serial.print("Alt(float): "); printFloat(gps.f_altitude()); Serial.print(" Course(float): ");
  printFloat(gps.f_course()); Serial.println();
  Serial.print("Speed(knots): "); printFloat(gps.f_speed_knots()); Serial.print(" (mph): ");
  printFloat(gps.f_speed_mph());
  Serial.print(" (mps): "); printFloat(gps.f_speed_mps()); Serial.print(" (kmph): ");
  printFloat(gps.f_speed_kmph()); Serial.println();

  gps.stats(&chars, &sentences, &failed);
  Serial.print("Stats: characters: "); Serial.print(chars); Serial.print(" sentences: ");
  Serial.print(sentences); Serial.print(" failed checksum: "); Serial.println(failed);
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) {
    Serial.print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print(".");

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}
