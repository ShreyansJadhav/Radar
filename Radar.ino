#include <Servo.h> // include the servo library

Servo myservo; // Creates an objects named "myservo" to refer to the motor later in the code

// sets variables that are later used in the code
int pos = 90; // first sets the initial position of the servo to 90 -> middle of the range for the servo
// sets the variables for the joystick
int button;
int xPin = A0;
int buttonP = 2;
int xVal;
int buttonVal;

// sets the pints for the ultrasonic sensor
const int trigPin = 13;
const int echoPin = 12;

// sets the pins for the LEDs
int redLEDPin = 6;
int greenLEDPin = 5;

void setup() {
  // starts the serial communication
  Serial.begin(9600);

  // sets the INPUT/OUTPUT of the joystick
  pinMode(xPin,INPUT);
  pinMode(buttonP,INPUT_PULLUP);

  // sets the servo to a pin on the arduino
  myservo.attach(9);
  
  // sets the INPUT/OUTPUT of the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // sets the INPUT/OUTPUT of the LEDs
  pinMode(redLEDPin,OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
}

void loop() {

  long duration,cm;  // variables used to determine the duration that the ultrasonic sensor outputs into centimeters and inches

    xVal = analogRead(xPin); // reads the value from the joystick -> this projects focuses only on the x axis so it only reads the x values
    buttonVal = digitalRead(buttonP); // reads the button value -> it can only be 1 or 0
    


  if(buttonVal==0){ // if the button is pressed, then the radar resets back to 90 degrees along with the ultrasonic sensor
    pos=90;
    myservo.write(pos);
    delay(10);
  }
  else{  // if the button isn't pressed, then this constantly checks for the x position from the joystick
    if(xVal>=520){ //if the joystick is pointed to the right, the radar moves to the right
      pos+=5;
      myservo.write(pos);
      delay(10);
    }
    else if(xVal<=480){ //if the joystick is pointed to the left, the radar moves to the left
      pos-=5;
      myservo.write(pos);
      delay(10);
    }
    else{ // if the joystick isn't pointed in any direction, the servo stays where it is
      pos=pos;
      myservo.write(pos);
      delay(10);
    }
  }

  // sends the high frequency sound wave from the ultrasonic sensor
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
 
  // receives the wave after bouncing off from the object
  duration = pulseIn(echoPin,HIGH);

  // calls the function at the bottom to convert the duration of the pulse back into centimeters 
  cm = microsecondsToCentimeters(duration);

    // prints the value of the joystick (between 0 and 1023), whether the button has been pressed (1=not pressed,0=pressed)
    // the distance from the objects in centimeters, and the position of the servo
    Serial.print("X: ");
    Serial.print(xVal);
    Serial.print("  Switch: ");
    Serial.print(buttonVal);
    Serial.print("  cm: ");
    Serial.print(cm);
    Serial.print("  pos: ");
    Serial.println(pos);

    if(cm<20){    // if the objects is less than 20 centimeters then a red LED lights up indicating the objects is too close
      digitalWrite(redLEDPin,HIGH);
      delay(10);
      digitalWrite(greenLEDPin,LOW);
      delay(10);
    }
    else{    // if the object is greater than 20 centimeters then a green LED lights up indicating the object is at a safe distance
      digitalWrite(redLEDPin,LOW);
      delay(10);
      digitalWrite(greenLEDPin,HIGH);
      delay(10);
    }
    
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

