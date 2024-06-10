#include <MQUnifiedsensor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Definições
#define Board                   ("ESp32 DEV BOARD")
#define MQ9Pin                     34  // Pino analógico
#define DHTPIN                  26  // Pino digital conectado ao DHT11
#define DHTTYPE                 DHT22 // Tipo de DHT
#define Type                    ("MQ-9") // Tipo do sensor MQ-9
#define Voltage_Resolution      5  // Resolução de tensão para ESP32
#define ADC_Bit_Resolution      12  // Resolução ADC para ESP32
#define RatioMQ9CleanAir        9.6  // RS / R0 = 60 ppm
#define PreaheatControlPin5     3   // Pino de preaquecimento 5V
#define PreaheatControlPin14    4   // Pino de preaquecimento 1.4V

#define BLYNK_TEMPLATE_ID "TMPL2A7ii46pP"
#define BLYNK_TEMPLATE_NAME "DHT"
#define BLYNK_AUTH_TOKEN "-tZ6e1DZ9PRqmDP40SjOZw37tYzZF82A"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Declarando Sensores
WiFiServer server(80);
WiFiClient  client; 

MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ9Pin, Type);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);


// Conectividade
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

void setup() {
  Serial.begin(9600);
  
  pinMode(PreaheatControlPin5, OUTPUT);
  pinMode(PreaheatControlPin14, OUTPUT);

  MQ9.setRegressionMethod(1); // _PPM =  a*ratio^b
  MQ9.setA(1000.5); MQ9.setB(-2.186); // Configure the equation to calculate LPG concentration

  MQ9.init();
  Serial.println("Preheating, please wait 2.5 minutes");
  digitalWrite(PreaheatControlPin5, HIGH);
  digitalWrite(PreaheatControlPin14, LOW);
  delay(5000);
  digitalWrite(PreaheatControlPin5, LOW);
  digitalWrite(PreaheatControlPin14, HIGH);
  delay(5000);
  digitalWrite(PreaheatControlPin5, HIGH);
  digitalWrite(PreaheatControlPin14, LOW);
  delay(1000);
  digitalWrite(PreaheatControlPin5, LOW);
  digitalWrite(PreaheatControlPin14, HIGH);
  delay(1000);
  digitalWrite(PreaheatControlPin5, HIGH);
  digitalWrite(PreaheatControlPin14, LOW);

  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ9.update();
    calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
    Serial.print(".");
  }
  MQ9.setR0(calcR0 / 10);
  Serial.println(" done!");

  if (isinf(calcR0)) {
    Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    while (1);
  }
  MQ9.serialDebug(true);
Serial.begin(9600);
   
  dht.begin();
  lcd.init();
  lcd.backlight();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {

  MQ9.update();
  float gasConcentration = MQ9.readSensor();
  if (isnan(gasConcentration)) {
    gasConcentration = 0;
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    h = 0;
    t = 0;
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C");

  Serial.print("Gas inflamavel: ");
  Serial.println(gasConcentration);

  lcd.setCursor(0, 0);
  lcd.print("QTD Gas: ");
  lcd.setCursor(0,8);
  lcd.print(gasConcentration);
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.setCursor(0, 8);
  lcd.print(h);
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temperatura: ");
  lcd.setCursor(0, 8);
  lcd.print(t);
  delay(3000);
  lcd.clear();

  Blynk.virtualWrite(V26, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V1, gasConcentration);
  delay(3000);
}
