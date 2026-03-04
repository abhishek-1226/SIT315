void setup() {
  Serial.begin(9600);

  DDRB = 0b00000001;   // PB0 (Pin 8) as OUTPUT
  DDRD = 0b00000000;   // All PORTD as INPUT (safe for simple case)
}
void loop() {

  // Check if PD2 (Pin 2) is HIGH
  if (PIND & 0b00000100) {

    Serial.println("Motion detected -> LED ON");

    PORTB = 0b00000001;   // Turn ON LED (Pin 8 HIGH)
    delay(300);
    PORTB = 0b00000000;   // Turn OFF LED
  }

  delay(100);
}