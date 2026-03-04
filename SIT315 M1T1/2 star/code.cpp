volatile bool motionFlag = false;

void motionISR()
{
	motionFlag = true;	
}


const int trigPin = 4;
const int echoPin = 5;


void setup() {
  Serial.begin(9600);

  DDRB |= 0b00000001;   // PB0 (Pin 8) as OUTPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(2), motionISR, RISING);

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
  
  if (motionFlag){
  	long distance = getDistance();
    
    Serial.print("Distance is: ");
    Serial.println(distance);
    
    if(distance<20){
   		
      Serial.println("Object is close and motion detected, LED ON");
      PORTB |= 0b00000001;
      delay(2000);
      PORTB &= 0b11111110;
      
    
    }
    else{
    Serial.println("Object is far");
    }
 motionFlag = false;
  
  }
  

}