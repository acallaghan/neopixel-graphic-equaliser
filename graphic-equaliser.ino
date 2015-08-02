int analogPin = A4; // read from multiplexer using analog input 4
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 10; // reset is attached to digital pin 10
int spectrumValue[7]; // to hold a2d values

#define NEO_PIXEL_PIN 4   // to LED out
#define NEO_PIXEL_NUM 40  

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_NUM, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  Serial.println("MSGEQ7 test by J Skoba");
}

void loop() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);

    
    Serial.print("\t");
    Serial.print(spectrumValue[i]);
    setGraphicForChannel(i, spectrumValue[i]);

    digitalWrite(strobePin, HIGH);
  }
  
  Serial.println();
}

void setGraphicForChannel(int channel, int value) {
  int intensity = value;
  int step = 0;

  for (int ch = channel; ch < NEO_PIXEL_NUM; ch = ch +8) {
    int band = intensity - ch * 5;
    
    if (band > 0) {
      pixels.setPixelColor(ch, intensityColour(intensity));
    } else {
      pixels.setPixelColor(ch, pixels.Color(0,0,0));
    }
    
    step++;
  }
  pixels.show();
}

uint32_t intensityColour(int intensity) {
  if (intensity > 0 && intensity > 40) {
    return pixels.Color(0, 155, 46);
  } else if (intensity >= 40 && intensity > 100) {
    return pixels.Color(132, 127, 12);
  } else if (intensity >= 100 && intensity > 150) {
    return pixels.Color(155, 52, 0);
  } else if (intensity >= 150 && intensity > 200) {
    return pixels.Color(132, 24, 12);
  } else {
    return pixels.Color(162, 5, 155);
  }
}
