//Other libraries needed for the code
#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include <LiquidCrystal_I2C.h>

// Define pins for the Arduino MAX31855
#define MAXDO   22 // Data Output cable
#define MAXCLK  2 // Clock Cable

// Define Hot Plate pins (thermocouples 1-4)
#define MAXCS1  35 //Hot 1
#define MAXCS2  37 //Hot 2
#define MAXCS3  34 //Hot 3
#define MAXCS4  36 //Hot 4

// Define Guard pins (thermocouples 5-8)
#define MAXCS5  31 //GRD 1
#define MAXCS6  33 //GRD 2
#define MAXCS7  30 //GRD 3
#define MAXCS8  32 //GRD 4

// Define Cold Plate pins (thermocouples 9-14)
#define MAXCS9  39 //CLD1
#define MAXCS10 41 //CLD2
#define MAXCS11 43 //CLD3
#define MAXCS12 38 //CLD4
#define MAXCS13 40 //CLD5
#define MAXCS14 42 //CLD6

// Initialize thermocouples
Adafruit_MAX31855 thermocouples[] = {
  Adafruit_MAX31855(MAXCLK, MAXCS1, MAXDO), // Hot Plate
  Adafruit_MAX31855(MAXCLK, MAXCS2, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS3, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS4, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS5, MAXDO), // Guard
  Adafruit_MAX31855(MAXCLK, MAXCS6, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS7, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS8, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS9, MAXDO), // Cold Plate
  Adafruit_MAX31855(MAXCLK, MAXCS10, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS11, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS12, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS13, MAXDO),
  Adafruit_MAX31855(MAXCLK, MAXCS14, MAXDO)
};

//Screen Initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set LCD address to 0x27 for 16x2 display

void setup() {
  Serial.begin(9600);

  while (!Serial) delay(1); // Wait for Serial on Leonardo/Zero, etc.

  Serial.println("MAX31855 Multi-Sensor Test");

  // Initialize SPI
  SPI.begin();

  // Initialize all thermocouples
  for (int i = 0; i < 14; i++) {
    Serial.print("Initializing sensor ");
    Serial.print(i + 1);
    Serial.print("...");

    if (!thermocouples[i].begin()) {
      Serial.println("ERROR!");
      while (1) delay(10); // Halt if initialization fails
    }
    Serial.println("DONE");
  }

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  lcd.setCursor(2, 0);  // Set cursor to character 2 on line 0
  lcd.print("Ready!");
  delay(1000);
}

void loop() {
  lcd.clear(); // Clear LCD for fresh readings
  
  double groupTemps[3] = {0.0, 0.0, 0.0}; // [Hot Plate, Guard, Cold Plate]
  int groupCounts[3] = {0, 0, 0};         // Count of valid readings per group
  
  for (int i = 0; i < 14; i++) {
    double temp = thermocouples[i].readCelsius();
    
    // Print individual thermocouple readings to Serial Monitor
  

      // Add to respective group averages
      if (i < 4) {
        groupTemps[0] += temp; // Hot Plate
        groupCounts[0]++;
        Serial.print("Hot ");
        Serial.print(i + 1);
        if (isnan(temp)) {
        Serial.println(": Fault!");
        } else {
        Serial.print(": ");
        Serial.print(temp);
        Serial.println(" C");}
    } else if (i < 8) {
        groupTemps[1] += temp; // Guard
        groupCounts[1]++;
        Serial.print("Guard ");
        Serial.print(i - 3);
        if (isnan(temp)) {
        Serial.println(": Fault!");
      } else {
        Serial.print(": ");
        Serial.print(temp);
        Serial.println(" C");}
    } else {
        groupTemps[2] += temp; // Cold Plate
        groupCounts[2]++;
        Serial.print("Cold ");
        Serial.print(i - 7);
        if (isnan(temp)) {
        Serial.println(": Fault!");
        } else {
        Serial.print(": ");
        Serial.print(temp);
        Serial.println(" C");}
      }
    
  }

  // Calculate averages and display on LCD
  //For hot plate
  lcd.setCursor(0, 0);
  if (groupCounts[0] > 0) {
    groupTemps[0] /= groupCounts[0];
    lcd.print("Hot:");
    lcd.print(groupTemps[0], 1);
    Serial.print("Hot Plate Avg: ");
    Serial.println(groupTemps[0], 1);
  } else {
    lcd.print("Hot:N/A");
    Serial.println("Hot Plate Avg: N/A");
  }

   //for guard
  lcd.setCursor(0, 1);
  if (groupCounts[1] > 0) {
    groupTemps[1] /= groupCounts[1];
    lcd.print("Guard:");
    lcd.print(groupTemps[1], 1);
    Serial.print("Guard Avg: ");
    Serial.println(groupTemps[1], 1);
  } else {
    lcd.print("Guard:N/A");
    Serial.println("Guard Avg: N/A");
  }
    //for cold 
  lcd.setCursor(8, 1);
  if (groupCounts[2] > 0) {
    groupTemps[2] /= groupCounts[2];
    lcd.print("Cold:");
    lcd.print(groupTemps[2], 1);
    Serial.print("Cold Plate Avg: ");
    Serial.println(groupTemps[2], 1);
  } else {
    lcd.print("Cold:N/A");
    Serial.println("Cold Plate Avg: N/A");
  }

  Serial.println("--------------------------");
  delay(2000);
}


