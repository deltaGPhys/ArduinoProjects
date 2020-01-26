int red = 9;
int green = 10;
int blue = 11;
int LED1 = 4;
int LED2 = 13;
int LED3 = 8;
int LED4 = 12;
int LED5 = 3; //PWM
int LED6 = 5; //PWM
int LED7 = 6; //PWM
int LEDs[] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7};
int len = sizeof(LEDs) / sizeof(int);

void(* resetFunc) (void) = 0; //declare reset function at address 0

void setup() {

  pinMode(red, OUTPUT); //red
  pinMode(green, OUTPUT); //green
  pinMode(blue, OUTPUT); //blue
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void updown(int period, int duration) {
  int t = 0;
  while (t < duration) {
    for (int thisPin = 0; thisPin <= len - 1; thisPin++) {
      digitalWrite(LEDs[thisPin], true);
      delay(period);
      t = t + period;
    }
    for (int thisPin = len; thisPin >= 0; thisPin--) {
      digitalWrite(LEDs[thisPin], false);
      delay(period);
      t = t + period;
    }

  }
}

void justdown(int period) {
  int t = 0;

  for (int thisPin = len; thisPin >= 0; thisPin--) {
    digitalWrite(LEDs[thisPin], false);
    delay(period);
    t = t + period;
  }

}

void justup(int period) {
  int t = 0;

  for (int thisPin = 0; thisPin <= len; thisPin++) {
    digitalWrite(LEDs[thisPin], true);
    delay(period);
    t = t + period;
  }

}

void updownsingle(int period, int duration) {
  int t = 0;
  while (t < duration) {
    for (int thisPin = 0; thisPin < len; thisPin++) {
      digitalWrite(LEDs[thisPin], true);
      if (thisPin > 0) {
        digitalWrite(LEDs[thisPin - 1], false);
        delay(period);
      }

      t = t + period;
    }
    for (int thisPin = len; thisPin >= 0; thisPin--) {
      digitalWrite(LEDs[thisPin], true);
      if (thisPin < len - 1) {
        digitalWrite(LEDs[thisPin + 1], false);
        delay(period);
      }

      t = t + period;
    }
  }
}

void updownsingledecay(int period, int duration, float constant) {
  int t = 0;
  while (t < duration && period >= 1) {
    period = period * exp(-constant * t);
    for (int thisPin = 0; thisPin < len; thisPin++) {
      digitalWrite(LEDs[thisPin], true);
      if (thisPin > 0) {
        digitalWrite(LEDs[thisPin - 1], false);
        delay(period);
      }

      t = t + period;
    }
    for (int thisPin = len; thisPin >= 0; thisPin--) {
      digitalWrite(LEDs[thisPin], true);
      if (thisPin < len - 1) {
        digitalWrite(LEDs[thisPin + 1], false);
        delay(period);
      }

      t = t + period;
    }
  }
}

void updownpausesdecay(int period, int spacing, int number) {
  int num = 0;
  while (num < number) {
    for (int thisPin = 0; thisPin < len + 1; thisPin++) {
      digitalWrite(LEDs[thisPin], true);
      if (thisPin >= 0) {
        digitalWrite(LEDs[thisPin - 1], false);
        delay(period);
      }
    }
    num = num + 1;

    delay(int(spacing * (number - num) / number));

    for (int thisPin = len; thisPin >= -1; thisPin--) {
      digitalWrite(LEDs[thisPin], true);
      if (thisPin <= len + 1) {
        digitalWrite(LEDs[thisPin + 1], false);
        delay(period);
      }
    }
    num = num + 1;

    delay(int(spacing * (number - num) / number));
  }
}

void blandi(int duration){
  analogWrite(blue, 850); //blue up
  analogWrite(red, 0); //red circle
  analogWrite(green, 0); //green circle

  for (int thisPin = 0; thisPin <= len ; thisPin++) {
      digitalWrite(LEDs[thisPin], true);
   }
   delay(duration);
}

void dothelix(float t, float dt, float t2, float dt2) {
  int var = 750;

  while (t2 < 5) {
    while (var < 1023) {
      analogWrite(blue, var); //blue up
      analogWrite(red, 112 * sin(t) + 911); //red circle
      analogWrite(green, 112 * cos(t) + 911); //green circle
      delay (10);
      var = var + 1;
      //Time Update
      t = t + dt;
    }

    while (var > 800)  {
      analogWrite(blue, var); //blue up
      analogWrite(green, 112 * cos(t) + 911); //green circle
      analogWrite(red, 112 * sin(t) + 911); //red circle
      delay (10);
      var = var - 1;
      //Time Update
      t = t + dt;
    }

    // Time Update
    t2 = t2 + dt2;

  }
  analogWrite(red, 0);
  analogWrite(blue, 0);
  analogWrite(green, 0);

}

void switchtest() {
  while (1 < 2) {
    if (analogRead(A0) > 800) {
      digitalWrite(LED1, HIGH);
    }
    else {
      digitalWrite(LED1, LOW);
    }
    if (analogRead(A1) > 800) {
      digitalWrite(LED2, HIGH);
    }
    else {
      digitalWrite(LED2, LOW);
    }
    if (analogRead(A2) > 800) {
      digitalWrite(LED3, HIGH);
    }
    else {
      digitalWrite(LED3, LOW);
    }
    if (analogRead(A3) > 800) {
      digitalWrite(LED4, HIGH);
    }
    else {
      digitalWrite(LED4, LOW);
    }
    delay(10);
  }
}


void loop()  {
  // values between 0 and 1023 adjust the duty cycle
  //(800 is the lowest value that you'll get light from an LED),
  //and that controls the brightness of the LED, the position of the servo, etc.



  // Time Variables
  float t = 0;
  float t2 = 0;
  float dt = .1;
  float dt2 = 1;

  //switchtest();

  //updown(50,10000);
  justup(50);
  blandi(5000);
  updownpausesdecay(100, 450, 8); //(100, 750, 16);
  updownsingle(100, 3000);  //(100, 3000);
  updownsingledecay(100, 30000, .00001);  //(100, 30000, .00001);

  digitalWrite(LED1, true);
  digitalWrite(LED2, true);
  digitalWrite(LED3, true);
  digitalWrite(LED4, true);
  digitalWrite(LED5, true);
  digitalWrite(LED6, true);
  digitalWrite(LED7, true);

  dothelix(t, dt, t2, dt2);
  justdown(50);
  delay(2000);
  

  resetFunc(); //call reset
}
