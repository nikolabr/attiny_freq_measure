#include <SoftwareSerial.h>
#define RX    3   // Pin 2/PB3 
#define TX    4   // Pin 3/PB4

// True frequency: 1.0501 kHz = 1000 freq units

SoftwareSerial serial(RX, TX); // Configure software serial
uint16_t cnt; // Temporary frequency counter
uint16_t val; // Final count after the timer period has elapsed
bool flag = 0; // Interrupt flag
char temp[9]; // Print buffer
void initTimerCounter1(void); // Function to initialize Compare Match on Timer 1

void setup(void)
{
  sei();     // Enable interrupts. Same as interrupts()
  
  GIMSK |= (1<<INT0);     // Enable INT0 (external interrupt) on Pin 7/PB2
  MCUCR |= (1<<ISC01);    // Configure as rising edge
  MCUCR |= (1<<ISC00);	  // Configure as rising edge

  initTimerCounter1();	  // Initialize timer
/* IMPORTANT NOTE: baud rate is tied to CPU frequency! The software serial library assumes 1MHz, so if the CPU is running at 
	8 or 16MHz, the baud rate will be 8 or 16 times smaller! */
  serial.begin(9600);
  serial.println("ready");
}

void loop(void)
{
  while (!flag) { delay(1); } 		// Check if interrupt has happened every millisecond
  cli();				// Disable interrupts. Same as noInterrupts()
  snprintf(temp, 9, "%08uld", val);	// Store the integer as an 8-character string with leading zeros
  serial.println(temp);			// Print string
  flag = 0;				// Reset interrupt flag
  sei();
}

void initTimerCounter1(void)
{
  TCCR1 = 0;                  // Stop the timer
  TCNT1 = 0;                  // Zero the timer
  GTCCR = _BV(PSR1);          // Reset the prescaler
  OCR1A = 108;                // Set the compare value
  TIMSK = _BV(OCIE1A);        // Interrupt on Compare Match A

  /* Start the timer, set it in CTC (Clear Timer on Compare) Mode,
	set prescaler to 4096 */
  TCCR1 = _BV(CTC1) | _BV(CS13) | _BV(CS12) | _BV(CS10);
// Reference: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf#G1.1178304

  sei();
}

ISR(TIMER1_COMPA_vect) // Timer compare interrupt
{
  cli(); 	
  val = cnt; 	// Store counter value
  cnt = 0; 	// Reset temporary counter
  flag = 1;	// Set interrupt flag
  sei();
}

ISR(INT0_vect) // Rising edge interrupt 
{
  cli();
  cnt++; // Increment temporary counter
  sei();
}
