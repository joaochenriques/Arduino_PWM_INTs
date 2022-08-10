/* Based on:
 *  https://microcontrollerslab.com/arduino-timer-interrupts-tutorial/
 */

#include <avr/io.h>
#include <avr/interrupt.h>

const int LED_pin = 13;
volatile int count = 1;

void flash()
{
  static boolean output = HIGH;
  digitalWrite(LED_pin, output);
  /* toggle output */
  output = !output;
}

ISR(TIMER2_COMPA_vect)
{
  count++;
  if( count == 249 ) {
    count = 0;
    flash();
  }
}

void setup()
{
  Serial.begin( 115200 );
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, LOW);

  /* disable interrupts */
  cli(); 

  /* TIMER 2 main registers
   *  - TCCR2A is responsible for PWM 
   *  - TCCR2B is used to set the prescalar value
   */
  TCCR2A = 0;
  TCCR2B = 0;
 
  /* Clear Timer on Compare Match (CTC) Mode -> WGM12 = 1
   * Prescalar = 256 -> CS12
   * == Speed of Timer1 = 16MHz/256 = 62500 kHz
   */
  TCCR2A = (1 << WGM21);
  TCCR2B = (1<<CS22) | (1<<CS21);

  /* output compare registers OCR1A -> Value
   * 250*250=62500 => 1s timed interrupt
   */
  OCR2A = 250; 

  /* TIMSK1 – Timer/Counter1 Interrupt Mask Register
   *   Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
   */
  TIMSK2 = (1 << OCIE2A);

  /* enable interrupts */
  sei();  
  
  Serial.println("TIMER2 Setup Finished.");
}

void loop()
{
  Serial.println(count); // do anything
  delay(200);
}
