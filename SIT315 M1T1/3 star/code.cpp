volatile byte lastState = 0;
volatile bool motionDetected = false;

const int trigPin = 4;
const int echoPin = 5;

ISR(PCINT0_vect)
{
  byte currentState = PINB;

  byte changed = currentState ^ lastState;

  if (changed & 0b00000011) {
    motionDetected = true;
  }

  lastState = currentState;
}

void setup() {

  Serial.begin(9600);

  // LED output (Pin 13 = PB5)
  DDRB |= 0b00100000;

  // PIR inputs (Pin 8 & 9)
  DDRB &= 0b11111100;

  // Enable Pin Change Interrupts for PORTB
  PCICR |= 0b00000001;

  // Enable interrupt for PB0 and PB1
  PCMSK0 |= 0b00000011;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

long getDistance(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}

void loop() {

  if (motionDetected){

    long distance = getDistance();

    Serial.print("Distance: ");
    Serial.println(distance);

    if(distance < 20){

      Serial.println("Motion detected and object close LED on");

      PORTB |= 0b00100000;
      delay(300);
      PORTB &= 0b11011111;

    }

    motionDetected = false;

  }

}