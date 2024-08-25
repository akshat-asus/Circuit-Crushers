#define enA 10    //  right enable grey
#define enB 5     //Left Enable white
#define white 9   //forward right
#define grey 8    //backward right
#define purple 7  //bacward left
#define blue 6    // forward left
#define echo 4    //echo  //Orange
#define trig 3    //trigger  //Yellow
#define rs 11
#define ls 2
#define servo 12
#define fsl 13
#define fsr A0
#define fsc A1
// #define servo2 A2
#define pump A2
bool fire = false;

int distanceF, distanceR, distanceL;

void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;  // Convert angle to microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(50);  // Refresh cycle of servo
}
void fireExtinguisher() {
  stop();

  delay(1000);
  if (digitalRead(fsl) == 0) {
    while (digitalRead(fsc) != 0) {
      left();
      delay(10);
    }
    stop();
  }
  if (digitalRead(fsr) == 0) {
    while (digitalRead(fsc) != 0) {
      right();
      delay(10);
    }
    stop();
  }
  while (digitalRead(fsc) != 0 && Ultrasonic() < 20) {
    forward();
  }
  stop();
  while (digitalRead(fsc) != 1) {
    digitalWrite(pump, HIGH);
    for (int angle = 70; angle > 30; angle -= 5) {
      servoPulse(servo, angle);
    }
    for (int angle = 30; angle <= 110; angle += 5) {
      servoPulse(servo, angle);
    }
  }
  digitalWrite(pump, LOW);
}

void checkSide() {
  stop();
  delay(100);
  for (int angle = 70; angle <= 140; angle += 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distanceL = Ultrasonic();
  Serial.print("D L=");
  Serial.println(distanceL);
  delay(100);
  for (int angle = 140; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distanceR = Ultrasonic();
  Serial.print("D R=");
  Serial.println(distanceR);
  delay(100);
  for (int angle = 0; angle <= 70; angle += 5) {
    servoPulse(servo, angle);
  }
  delay(300);
  compareDistance();
}
void compareDistance() {
  if (distanceL > distanceR) {
    left();
    delay(500);
    forward();
    delay(600);
    right();
    delay(500);
    forward();
    delay(600);
    right();
    delay(600);
    forward();
    delay(600);
  } else {
    right();
    delay(500);
    forward();
    delay(600);
    left();
    delay(500);
    forward();
    delay(600);
    left();
    delay(600);
    forward();
    delay(600);
  }
}
long Ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long time = pulseIn(echo, HIGH);
  return time / 29 / 2;
}


void right() {
  digitalWrite(white, LOW);
  digitalWrite(grey, HIGH);
  digitalWrite(blue, HIGH);
  digitalWrite(purple, LOW);
}

void stop() {
  digitalWrite(white, LOW);
  digitalWrite(grey, LOW);
  digitalWrite(blue, LOW);
  digitalWrite(purple, LOW);
}
void left() {
  digitalWrite(white, HIGH);
  digitalWrite(grey, LOW);
  digitalWrite(blue, LOW);
  digitalWrite(purple, HIGH);
}
void backward() {

  digitalWrite(white, LOW);
  digitalWrite(grey, HIGH);
  digitalWrite(purple, HIGH);
  digitalWrite(blue, LOW);
}
void forward() {
  digitalWrite(white, HIGH);
  digitalWrite(grey, LOW);
  digitalWrite(purple, LOW);
  digitalWrite(blue, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(grey, OUTPUT);
  pinMode(purple, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ls, INPUT);
  pinMode(rs, INPUT);
  pinMode(servo, OUTPUT);
  pinMode(fsl, INPUT);
  pinMode(fsr, INPUT);
  pinMode(fsc, INPUT);
  pinMode(pump, OUTPUT);



  analogWrite(enA, 150);
  analogWrite(enB, 150);
  for (int angle = 70; angle <= 140; angle += 5) {
    servoPulse(servo, angle);
  }
  for (int angle = 140; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  for (int angle = 0; angle <= 70; angle += 5) {
    servoPulse(servo, angle);
  }
}

void loop() {
  // LINE FOLLOWER
  distanceF = Ultrasonic();
  Serial.print("D F=");
  Serial.println(distanceF);

  if((digitalRead(fsl)==0) || (digitalRead(fsc)==0) || (digitalRead(fsr)==0))
  {

    fire = true;
    fireExtinguisher();
    stop();
  }
  if ((digitalRead(ls) == 0) && (digitalRead(rs) == 0))  //BOTH AT WHITE
  {
    if (distanceF > 15) {
      forward();
    } else {
      checkSide();
    }


  } else if ((digitalRead(ls) == 1) && (digitalRead(rs) == 0)) {
    left();
  } else if ((digitalRead(ls) == 0) && (digitalRead(rs) == 1)) {
    right();
  }
  delay(10);
  // digitalWrite(pump,LOW);
  // if(digitalRead(fsc) == 1)
  // {
  //   digitalWrite(pump,HIGH);
  // }
  // else
  // {
  //   digitalWrite(pump,LOW);
  // }
}
