volatile byte lastState = 0;
volatile bool motionDetected = false;

const int trigPin = 4;
const int echoPin = 5;


/* ---------- PIN CHANGE INTERRUPT (PIR sensors) ---------- */

ISR(PCINT0_vect)
{
  byte currentState = PINB;

  byte changed = currentState ^ lastState;

  if (changed & 0b00000011) {   // PIR on PB0 and PB1 (Pin 8 & 9)
    motionDetected = true;
  }

  lastState = currentState;
}


/* ---------- TIMER INTERRUPT (heartbeat LED) ---------- */

ISR(TIMER1_COMPA_vect)
{
  PORTB ^= 0b00010000;   // Toggle LED on Pin 12 (PB4)
}


/* ---------- SETUP ---------- */

void setup()
{
  Serial.begin(9600);

  // LED outputs
  // PB5 = Pin 13 (alarm LED)
  // PB4 = Pin 12 (heartbeat LED)
  DDRB |= 0b00110000;

  // PIR inputs (Pin 8 & 9)
  DDRB &= 0b11111100;

  // Enable Pin Change Interrupt for PORTB
  PCICR |= 0b00000001;

  // Enable interrupt for PB0 and PB1
  PCMSK0 |= 0b00000011;

  // Ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  /* ---------- TIMER1 CONFIG ---------- */

  cli();   // disable interrupts

  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = 15624;   // 1 second interval

  TCCR1B |= (1 << WGM12);   // CTC mode

  TCCR1B |= (1 << CS12) | (1 << CS10);   // 1024 prescaler

  TIMSK1 |= (1 << OCIE1A);   // enable timer interrupt

  sei();   // enable interrupts
}


/* ---------- ULTRASONIC DISTANCE FUNCTION ---------- */

long getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}


/* ---------- MAIN LOOP ---------- */

void loop()
{
  if (motionDetected)
  {
    long distance = getDistance();

    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < 20)
    {
      Serial.println("Motion detected and object close");

      // Turn ON alarm LED (Pin 13)
      PORTB |= 0b00100000;

      delay(300);

      // Turn OFF alarm LED
      PORTB &= 0b11011111;
    }

    motionDetected = false;
  }
}