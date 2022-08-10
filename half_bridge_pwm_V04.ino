/*
 * Based on:
 *  https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm
 * 
 */ 

void setupPWM16(int resolution) 
{
  /* disable interrupts */
  cli();   

  /*  Output Compare Registers OCRnA and OCRnB 
   *   set the levels at which outputs A and B will be affected 
   *   When the timer value matches the register value, the 
   *   corresponding output will be modified as specified by the mode
   */
  OCR1A = 0;
  OCR1B = 0;

  /* Set Port B pins 1 and 2 as outputs (page 72 and 280) 
   * Arduino UNO Pins 9 and 10
   */
  DDRB  |= _BV(DDB1) | _BV(DDB2);       
   
  /* WGM   -> Wave form generation bits:
   *          -> control the overall mode of the timer
   *          -> These bits are split between TCCRnA and TCCRnB
   *          
   * CS    -> Clock Select bits
   *          -> control the clock prescaler
   *          
   * COMnA -> Compare Match Output A Mode bits
   *          -> enable/disable/invert output A 
   *          
   * COMnB -> Compare Match Output B Mode bits
   *          -> enable/disable/invert output B 
   */

  /* Mode 14: FAST PWM => TOP -> OCR1A  (Pag 109: Table 15-5)
   *  WGM13 = 1 & WGM12 = 1 & WGM11 = 1 
   */ 
  TCCR1A = _BV(COM1A1)                  /* Ch-A non-inverting (Pag 109: Table 15-4) */
         | _BV(COM1B1) | _BV(COM1B0)    /* Ch-B inverting (Pag 109: Table 15-4) */
         | _BV(WGM11);                  
  
  TCCR1B = _BV(WGM13) | _BV(WGM12)      
         | _BV(CS10);                   /* no prescaler (Pag 110: Table 15-5) */

  /* TOP counter value (freeing OCR1A */
  ICR1 = ( 0xffff >> (16-resolution) ) | 0xFF;                           

  /* enable interrupts */
  sei();  
}

/* 16-bit version of analogWrite(). Works only on pins 9 and 10. */
void analogWrite16(uint8_t pin, uint16_t val)
{
  switch (pin) {
    case  9: OCR1A = val; break;
    case 10: OCR1B = val; break;
  }
}

void setup() 
{ 
  setupPWM16( 9 );
}

void loop() 
{
  analogWrite16(9,128);
  analogWrite16(10,128);
}
