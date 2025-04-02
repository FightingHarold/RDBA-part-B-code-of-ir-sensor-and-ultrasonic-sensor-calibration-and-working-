int leftIR = 8;          //Left IR                                  
int rightIR = 9;         //Right IR 
int trigPin = 10;        //trig pin of ultrasonic sensor
int echoPin = 11;        //echo pin of ultrasonic sensor

int E1 = 5;              // Left Motor speed control
int E2 = 6;              // Right motor speed control
int M1 = 4;              // Left motor directional control
int M2 = 7;              // Right motor directional control

long duration;           // variable for ultrasonic 
int distance;
//define states for AFSM
enum RobotState { LINE_FOLLOWING, OBSTACLE_AVOIDANCE };
RobotState currentState = LINE_FOLLOWING;

void setup(){
//initialize motor control pins
  pinMode(E1, OUTPUT);         
  pinMode(E2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
//initialize sensor pins
  pinMode(leftIR, INPUT);
  pinMode(rightIR, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600); //start serial communication (debugging)
}

void loop(){  //main code
// debugging: print sensor states
  Serial.print("Left IR: ");
  Serial.print(digitalRead(leftIR));
  Serial.print(" | Right IR: ");
  Serial.println(digitalRead(rightIR));

  switch (currentState){
    case LINE_FOLLOWING:
      lineFollowing();
      Serial.println("LINE_FOLLOWING");
      if (isObstacleDetected()) {  //check for obstacles
        currentState = OBSTACLE_AVOIDANCE;
      }
      break;
    case OBSTACLE_AVOIDANCE:
      obstacleAvoidance();
      Serial.println("obstacle");
      //return to line following if no obstacle is detected
      if (!isObstacleDetected()) {     
        currentState = LINE_FOLLOWING;

      }
      break;
  }
}

// Line following sequence
void lineFollowing() {
// both sensors are white (not black), move forward
  if (digitalRead(leftIR)==LOW && digitalRead(rightIR)== LOW) {
    advance(100,100);
    Serial.println("move forward");
  }//if left sensor is black turn left 
  else if (digitalRead(leftIR)== LOW && digitalRead(rightIR) == HIGH) {
    turn_R(100, 0);
    Serial.println("right IR");
  }//if right sensor is black turn right 
  else if (digitalRead(leftIR)== HIGH && digitalRead(rightIR) == LOW) {
    turn_L(0, 130);
    Serial.println("left IR");
  }//if both sensor are black, stop completely
  else{
    stop();
    Serial.println("stop");
  }
}

// obstacle avoidance sequence 
void obstacleAvoidance() {
  stop();
  delay(1000);
  Serial.println("obstacle avoidance running");
}


//obstacle detection sequence
bool isObstacleDetected(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH ,30000);
  distance = duration * 0.034 / 2;
  Serial.println("obstacle detected");
  Serial.print("distance: ");
  Serial.println(distance);
// return true(detected) or false (not detected)
  return( distance <5 && distance >0);


}
//motor control function
void advance(char a, char b){ //forward
  analogWrite(E1, a);         // set left motor speed
  digitalWrite(M1, HIGH);     // set left motor forward
  analogWrite(E2, b);         // set right motor speed
  digitalWrite(M2, HIGH);     // set right motor forward
}

void back_off(char a, char b){ //backward
  analogWrite(E1, a);          // set left motor speed
  digitalWrite(M1, LOW);       // set left motor backward
  analogWrite(E2, b);          // set right motor speed
  digitalWrite(M2, LOW);       // set right motor backward
}
void turn_L(char a, char b){ //turn left
  analogWrite(E1, a);        // set left motor speed
  digitalWrite(M1, LOW);     // set left motor backward
  analogWrite(E2, b);        // set right motor speed
  digitalWrite(M2, HIGH);    // set right motor forward
}
void turn_R(char a, char b){ //turn right
  analogWrite(E1, a);        // set left motor speed
  digitalWrite(M1, HIGH);    // set left motor forward
  analogWrite(E2, b);        // set right motor speed
  digitalWrite(M2, LOW);     // set right motor backward
}
void stop(){ //stop
  analogWrite(E1, 0);        // stop left motor
  analogWrite(E2, 0);        // stop left motor
}
