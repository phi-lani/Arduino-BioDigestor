#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>  // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // Include the I2C LCD library

#define DHTPIN1 2     // Digital pin connected to the first DHT sensor
#define DHTPIN2 3     // Digital pin connected to the second DHT sensor
#define DHTTYPE DHT22  // DHT 22 (AM2302)

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

const int RELAY_PIN_SENSOR1 = 7;  // Pin connected to relay for Sensor 1
const int RELAY_PIN_SENSOR2 = 8;  // Pin connected to relay for Sensor 2
const int RELAY_PIN_SENSOR3 = 9;  // Pin connected to the additional relay for Sensor 1

bool relay1On = false;  // Flag to track the state of relay for Sensor 1
bool relay2On = false;  // Flag to track the state of relay for Sensor 2
bool relay3On = false;  // Flag to track the state of the additional relay for Sensor 1

float humidity1;
float temperature1;
float humidity2;
float temperature2;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the I2C LCD with the appropriate address and size

void setup() {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  pinMode(RELAY_PIN_SENSOR1, OUTPUT);  // Configure relay pin for Sensor 1
  pinMode(RELAY_PIN_SENSOR2, OUTPUT);  // Configure relay pin for Sensor 2
  pinMode(RELAY_PIN_SENSOR3, OUTPUT);  // Configure relay pin for Sensor 3
  lcd.begin();
}

void loop() {
  temperature1 = dht1.readTemperature();
  humidity1 = dht1.readHumidity();

  temperature2 = dht2.readTemperature();
  humidity2 = dht2.readHumidity();


  //lcd.begin(16,2);
  lcd.clear();  // Clear the LCD display

  // Check if temperature for Sensor 1 is a valid value
  if (!isnan(temperature1)) {
    lcd.setCursor(0, 0);
    lcd.print(F("Sensor T1:"));
    lcd.print(temperature1);
    lcd.print(F("C"));
    if (temperature1 < 37) {
      Serial.println(F("Temperature for Sensor 1 is less than 37°C."));
      digitalWrite(RELAY_PIN_SENSOR1, HIGH);  // Turn on relay for Sensor 1
      digitalWrite(RELAY_PIN_SENSOR3, HIGH);  // Turn on additional relay for Sensor 1
      relay1On = true;
      relay3On = true;
    } else if (relay1On && temperature1 >= 37) {
      digitalWrite(RELAY_PIN_SENSOR1, LOW);  // Turn off relay for Sensor 1
      digitalWrite(RELAY_PIN_SENSOR3, LOW);  // Turn off additional relay for Sensor 1
      relay1On = false;
      relay3On = false;
    }
  } else {
    Serial.println(F("Temperature for Sensor 1 is invalid."));
  }

  // Check if temperature for Sensor 2 is a valid value
  if (!isnan(temperature2)) {
    lcd.setCursor(0, 1);
    lcd.print(F("Sensor T2:"));
    lcd.print(temperature2);
    lcd.print(F("C"));
    if (temperature2 < 70) {
      Serial.println(F("Temperature for Sensor 2 is less than 70°C."));
      digitalWrite(RELAY_PIN_SENSOR2, HIGH);  // Turn on relay for Sensor 2
      relay2On = true;
    } else if (relay2On && temperature2 >= 70) {
      digitalWrite(RELAY_PIN_SENSOR2, LOW);  // Turn off relay for Sensor 2
      relay2On = false;
    }
  } else {
    Serial.println(F("Temperature for Sensor 2 is invalid."));
  }

  delay(5000);
}
