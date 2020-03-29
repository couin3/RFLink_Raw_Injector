#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "settings.h"
#include "common_msg.h"
#include "raw_msg.h"

void setup() {
  // Low Power Setup
  power_Setup();
  // Starting transmitter
  radio_tx_Setup();
  pinMode(LED_BUILTIN, OUTPUT);

  // Sending messages
  digitalWrite(LED_BUILTIN, HIGH);
  playMsg(msgPreamble, NELEMS(msgPreamble));
  for (byte j = 0; j < msgRepeat; j++)
  {
    playMsg(msgTemp, NELEMS(msgTemp));
    playMsg(msgInterlude, NELEMS(msgInterlude));
  }
  playMsg(msgEnd, NELEMS(msgEnd));

  // Shutting down transmitter
  digitalWrite(LED_BUILTIN, LOW);
  radio_tx_Setdown();

  //Sleep
  sleep_mode();
}

// the loop function runs over and over again forever
void loop()
{
  // do nothing
}

void power_Setup()
{
  ADCSRA = 0;             // disable ADC
  power_all_disable();    // turn off all modules
  power_timer0_enable();  // turn on Timer/Counter0
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void radio_tx_Setup()
{
#ifdef PIN_GND
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);
#endif
#ifdef PIN_VCC
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_VCC, HIGH);
#endif
#if defined(PIN_GND) || defined(PIN_VCC)
  delay(250);
#endif
  pinMode(PIN_DATA, OUTPUT);
}

void radio_tx_Setdown()
{
#ifdef PIN_VCC
  digitalWrite(PIN_VCC, LOW);
  pinMode(PIN_VCC, INPUT);
#endif
#ifdef PIN_GND
  digitalWrite(PIN_GND, LOW);
  pinMode(PIN_GND, INPUT);
#endif
  pinMode(PIN_DATA, INPUT);
}

void playMsg(const unsigned int * msg, const byte msgSize) {

  for (byte i = 0; i < msgSize; i += 2)
  {
    digitalWrite(PIN_DATA, HIGH);
    delayMicroseconds(msg[i]);

    digitalWrite(PIN_DATA, LOW);
    delayMicroseconds(msg[i + 1]);
  }
}
