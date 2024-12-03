//inisialisai koneksi ke BLYNK app
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL63UXa7gH_"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "_XH6KAFhL5Eb_-E2Qg_Q70wGHUkJyswq"

//library
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <GP2YDustSensor.h> // Library for dust sensor

//delay untuk blynk
BlynkTimer timer;

//auth blynk serta pass dan ssid wifi user
char auth[] = "_XH6KAFhL5Eb_-E2Qg_Q70wGHUkJyswq"; //Paste auth token you copied
char ssid[] = "Projectkita_id"; //wifi name
char pass[] = "caritaudong"; //wifi password

//sensor co2
int sensorIn = 5; // Analog Pin Out for CO2 sensor infrared
unsigned long duration;
long ppm;

//lcd tipe i2c dan ukuran layar
LiquidCrystal_I2C lcd(0x27, 20, 4);

//dust sensor
const uint8_t SHARP_LED_PIN = 12; // Dust sensor LED pin
const uint8_t SHARP_VO_PIN = 34; // Dust sensor analog pinout
GP2YDustSensor dustSensor(GP2YDustSensorType :: GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

//dht22 sensor
#define DHTPIN 14   // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//pin digital relay
int RELAY = 18;

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  //memulai lcd
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Filter Udara");
  lcd.setCursor(1, 3);
  lcd.print("connecting to wifi");

  //memulai sensor serta inisialisasi pin sensor co2 sebagai input dan relay sebagai output
  dht.begin();
  dustSensor.begin();
  pinMode(sensorIn, INPUT);
  pinMode(RELAY, OUTPUT);
  Serial.println("connected to wifi");
  //mengkoneksikan ke wifi dan blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  //memulai fungsi blynk
  Blynk.run();

  //main program
  delay(2000);
  //mendeteksi kelembapan dan temperature
  unsigned int h = dht.readHumidity(); // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  unsigned int t = dht.readTemperature();  // Read temperature as Celsius (the default)

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  while (digitalRead(sensorIn) == HIGH) {
    ;
  }
  //wait for the pin to go HIGH and measure HIGH time

  //kalibrasi sensor dust
  duration = pulseIn(sensorIn, HIGH, 2000000);
  ppm = 5000 * ((duration / 1000) - 2) / 1000;
  //Serial.print(ppm);
  Serial.print(ppm);
  Serial.println();

  //menampilkan lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity : " + String(h) + "%");
  lcd.setCursor(0, 1);
  lcd.print("Temperature :" + String(t) + " C");
  lcd.setCursor(0, 2);
  lcd.print("Dust :" + String(dustSensor.getDustDensity()) + " ug/m3");
  lcd.setCursor(0, 3);
  lcd.print("CO2 :" + String(ppm));
  Serial.println(dustSensor.getDustDensity());
  //mengirim data sensor ke blnynk
  Blynk.virtualWrite(V4, h); // send humidity value from the dht22 sensor to blynk virtual pin v4
  Blynk.virtualWrite(V5, t); // send temperature sensor from the dht22 sensor to blynk virtual pin v5
  Blynk.virtualWrite(V6, ppm); // send co2 concentration from co2 sensor to blynk virtual pin v6
  Blynk.virtualWrite(V7, dustSensor.getDustDensity()); // send dust density value from dust sensor to blynk virtual pin v7
}

//mengaktifkan kipas melalui tombol pada blynk
BLYNK_WRITE(V3) {
  digitalWrite(RELAY, param.asInt()); // Turn LED on.
  Serial.println("kipas berputar");
}
