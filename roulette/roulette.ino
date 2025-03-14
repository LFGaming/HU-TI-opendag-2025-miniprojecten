#include <Adafruit_NeoPixel.h>

#define LED_PIN 7        // Pin where the Neopixel ring is connected
#define LED_COUNT 45     // Number of LEDs in the ring

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Possible outcomes
enum ColorResult { GREEN, RED, BLUE };  
ColorResult finalColor;

void setup() {
  Serial.begin(9600);  // Initialize serial monitor
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  drawRouletteWheel();
  randomSeed(analogRead(0));  // Improve randomness
}

void loop() {
  spinRoulette();
  
  // Print result to serial monitor
  if (finalColor == GREEN) {
    Serial.println("Ball landed on GREEN!");
  } else if (finalColor == RED) {
    Serial.println("Ball landed on RED!");
  } else {
    Serial.println("Ball landed on BLUE!");
  }

  delay(2000);  // Short pause before next spin
}

void drawRouletteWheel() {
  for (int i = 0; i < LED_COUNT; i++) {
    if (i == 0) {
      strip.setPixelColor(i, strip.Color(0, 50, 0));  // Green for zero
    } else if (i % 2 == 0) {
      strip.setPixelColor(i, strip.Color(50, 0, 0));  // Red
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 25));  // Black
    }
  }
  strip.show();
}

void spinRoulette() {
  int position = random(LED_COUNT);  // Random start position
  int speed = random(10, 20);  // Initial fast spin speed (lower = faster)
  int totalSpins = random(80, 120);  // Shorter duration
  int slowdownStart = totalSpins / 3;  // When to start slowing down

  for (int step = 0; step < totalSpins; step++) {
    strip.clear();
    drawRouletteWheel();

    // Ball effect: Set one LED to white (the "ball")
    strip.setPixelColor(position, strip.Color(100, 100, 100));  
    strip.show();

    delay(speed);
    position = (position + 1) % LED_COUNT;  // Move ball to next LED

    // Apply exponential slowdown for a smoother stop
    if (step > slowdownStart) {
      speed += speed / 12;  
    }
  }

  // Determine final color
  if (position == 0) {
    finalColor = GREEN;
  } else if (position % 2 == 0) {
    finalColor = RED;
  } else {
    finalColor = BLUE;
  }
}
