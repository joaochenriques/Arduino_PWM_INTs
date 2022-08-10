/* Based on:
 *  https://microcontrollerslab.com/arduino-timer-interrupts-tutorial/
 */

#include <avr/io.h>
#include <avr/interrupt.h>

const int LED_pin = 13;
volatile int count;

void flash()
{
  static boolean output = HIGH;
  digitalWrite(LED_pin, output);
  /* toggle output */
  output = !output;
}

ISR(TIMER1_COMPA_vect)
{
  count++;
  flash();
}

void setup()
{
  Serial.begin( 115200 );
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, LOW);

  /* disable interrupts */
  cli(); 

  /* TIMER 1 main registers
   *  - TCCR1A is responsible for PWM 
   *  - TCCR1B is used to set the prescalar value
   */
  TCCR1A = 0;
  TCCR1B = 0;

  /* Clear Timer on Compare Match (CTC) Mode -> WGM12 = 1
   * Prescalar = 256 -> CS12
   * == Speed of Timer1 = 16MHz/256 = 62500 kHz
   */
  TCCR1B = (1 << WGM12) | (1 << CS12);

  /* output compare registers OCR1A -> Value
   * If equal to speed of Timer1 => 1s timed interrupt
   */
  OCR1A = 62500;

  /* TIMSK1 – Timer/Counter1 Interrupt Mask Register
   *   Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
   */
  TIMSK1 = (1 << OCIE1A);

  /* enable interrupts */
  sei();  
  
  Serial.println("TIMER1 Setup Finished.");
}

void loop()
{
  Serial.println(count); // do anything
  delay(200);
}
