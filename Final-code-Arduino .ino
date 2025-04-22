String inputString = "";
const int pwm1 = 3;
const int pwm2 = 5;
const int pwm3 = 6;
const int pwm4 = 9;

const int dir1 = 10;
const int dir2 = 2;
const int dir3 = 12;
const int dir4 = 13;

int speed = 100;

unsigned long lastCommandTime = 0;
const unsigned long timeout = 200; // milliseconds

void setup() {
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);
  Serial.begin(250000);
}

// Straight motions
void moveForward() {
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, speed);
  digitalWrite(dir4, LOW);
  analogWrite(pwm4, speed);
}

void moveBackward() {
  digitalWrite(dir2, LOW);
  analogWrite(pwm2, speed);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm4, speed);
}

void Left() {
  digitalWrite(dir1, HIGH);  // Flipped
  analogWrite(pwm1, speed);
  digitalWrite(dir3, HIGH);  // Flipped
  analogWrite(pwm3, speed);
}

void Right() {
  digitalWrite(dir1, LOW);   // Flipped
  analogWrite(pwm1, speed);
  digitalWrite(dir3, LOW);   // Flipped
  analogWrite(pwm3, speed);
}


// Diagonal motions
void frontLeft() {
  digitalWrite(dir1, LOW);
  analogWrite(pwm1, speed);
  digitalWrite(dir4, LOW);
  analogWrite(pwm4, speed);
}

void frontRight() {
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, speed);
  digitalWrite(dir4, LOW);
  analogWrite(pwm4, speed);
}

void backLeft() {
  digitalWrite(dir1, LOW);
  analogWrite(pwm1, speed);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm4, speed);
}

void backRight() {
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, speed);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm4, speed);
}

void stopMotors() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('!');
    float velx, vely;
    int xPos = input.indexOf('x');
    int yPos = input.indexOf('y');

    String velxStr = input.substring(0, xPos);
    String velyStr = input.substring(xPos + 1, yPos);

    velx = velxStr.toFloat();
    vely = velyStr.toFloat();

    // Priority: Diagonal > Straight > Stop
    if (abs(velx) > 4 && abs(vely) > 4) {
      if (velx > 0 && vely > 0) {
        frontRight();
      } else if (velx < 0 && vely > 0) {
        frontLeft();
      } else if (velx > 0 && vely < 0) {
        backRight();
      } else if (velx < 0 && vely < 0) {
        backLeft();
      }
    } else if (abs(velx) > 4) {
      if (velx > 0) Right();
      else Left();
    } else if (abs(vely) > 4) {
      if (vely > 0) moveForward();
      else moveBackward();
    } else {
      stopMotors();
    }

    lastCommandTime = millis();  // Update timestamp
  }

  // Stop motors if no input is received within timeout
  if (millis() - lastCommandTime > timeout) {
    stopMotors();
  }
}
