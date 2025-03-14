#include <Adafruit_NeoPixel.h>

#define LED_PIN 7        // Pin where the Neopixel ring is connected
#define LED_COUNT 45     // Number of LEDs in the ring
#define Green 4
#define Red 5
#define Blue 6
#define Plus 9
#define Min 8
#define OK 10

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int colorIndex = 0;
int colors[3] = {
  Green, Red, Blue
}; 
int totalColors = sizeof(colors) / sizeof(colors[0]);


// Possible outcomes
enum ColorResult { GREEN, RED, BLUE };  
ColorResult finalColor;

void setup() {
  pinMode(Plus, INPUT);
  pinMode(Min, INPUT);
  pinMode(OK, INPUT);
  Serial.begin(9600);  // Initialize serial monitor
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  drawRouletteWheel();
  randomSeed(analogRead(0));  // Improve randomness
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
    finalColor = BLUE;
  } else {
    finalColor = RED;
  }
}

void ChooseYourCharacter(){
  pinMode(Green, OUTPUT);

  while (!digitalRead(OK))
  {
    if(digitalRead(Plus) == HIGH){
      colorIndex = (colorIndex + 1) % totalColors;
      updateLED();
      delay(1000);
    }

    if(digitalRead(Min) == HIGH){
      colorIndex = (colorIndex - 1 + totalColors) % totalColors;
      updateLED();
      delay(1000);
    }

    if (digitalRead(OK) == HIGH){
      break;
    }
  }
}

void updateLED(){
  if (colors[colorIndex] == Green)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 50, 0));
      strip.show();
    }
  }

  if (colors[colorIndex] == Red)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      strip.setPixelColor(i, strip.Color(50, 0, 0));
      strip.show();
    }
  }

  if (colors[colorIndex] == Blue)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 25));
      strip.show();
    }
  }
}

void loop() {
  ChooseYourCharacter();
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