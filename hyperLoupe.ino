
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define LEDPIN 13           // use internal LED for now

float strobeFrequency;      // Hz
int timerCount;             // equiavlent to half the period of the strobe
String statusString;

void setup() {
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);
  clearBuffer();                       // or else print gibberish to console
  Serial.println("Initializing...");
}

void loop() {
  Serial.println("strobeFrequency?");
  while (Serial.available() == 0);
  strobeFrequency = Serial.parseFloat();
  computeTimerCount();
  reportConfiguration();
  initializeTimer();
}

// TODO this is gross
void clearBuffer() {
  delay(2000);                  // magic
  while (Serial.read() > 0);    // burn through buffer
  delay(2000);                  // magic
}

// set Interrupt Service Routine
// TODO implement strobe duration in software(?) currently LED is half-on, half-off
ISR(TIMER1_COMPA_vect) {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}

// updates timerCount
// assumes 1024 prescaler
void computeTimerCount() {
  // 6.4e-5 seconds per ccyle for 1024 prescaler
  // multiply by 2 because we need to toggle on and then toggle off
  // subtract 1 because resetting the register consumes a cycle
  timerCount = round((1 / strobeFrequency) / (0.000064 * 2)) - 1;
}

// prints current configuration to the console
void reportConfiguration() {
  statusString = "\tstrobeFrequency: ";
  statusString += strobeFrequency;
  statusString += "Hz";
  Serial.println(statusString);
  Serial.flush();
} 

void initializeTimer() {
  cli();                       // disable global interrupts
  // set both registers to 0
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = timerCount;          // set compare match register
  TCCR1B |= (1 << WGM12);      // set Clear Timer on Compare Match
  // set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);     // enable timer compare interrupt
  sei();                       // enable global interrupts
}
