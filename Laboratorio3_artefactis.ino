// Lectura simple del LM35 y salida por Serial en formato:
// Temperatura: XX.XX °C
//
// Conexiones:
//  - LM35 VCC -> 5V
//  - LM35 GND -> GND
//  - LM35 Vout -> A0

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

const int pinLM35 = A0;            // pin analógico donde está conectado LM35 Vout
const unsigned long intervalo = 1000; // intervalo de actualización en ms
unsigned long ultimaMillis = 0;

void setup() {
  Serial.begin(9600);            // velocidad del Monitor Serial
  while (!Serial) { /* espera en algunas placas */ }
  delay(50);
  Serial.println("LM35 - Lectura simple");
  display.begin(i2c_Address, true); // Address 0x3C default
  display.setContrast (255); // dim display
  // Clear the buffer.
  display.clearDisplay();
  // Save to update screen changes
  display.display();
}

void loop() {
  unsigned long ahora = millis();
  if (ahora - ultimaMillis >= intervalo) {
    ultimaMillis = ahora;

    int lectura = analogRead(pinLM35);              // 0..1023
    const float Vref = 5;                         // ajustar si tu referencia ADC es distinta
    float voltaje = lectura * (Vref / 1023.0);      // voltaje en V
    float tempC = voltaje * 100.0;                  // LM35: 10 mV/°C -> factor 100

    // Imprime en formato legible
    Serial.print("Temperatura: ");
    Serial.print(tempC, 2);   // 2 decimales
    Serial.println(" °C");

    display.clearDisplay();
    display.setTextSize(1); 
    display.setTextColor(SH110X_WHITE); 
    display.setCursor(0, 0);
    display.println("Temperatura:");
    display.setTextSize(2);
    display.setCursor(20, 20);
    display.print(tempC, 1);
    display.println(" C");
    display.display();
  }

  // Aquí puedes hacer otras tareas no bloqueantes
}