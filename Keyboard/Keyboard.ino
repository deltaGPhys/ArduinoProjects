
int speakerPin = 11;
int current = 0;
int last = 0;

//A5
int lows5[] = {20,145,450};
int highs5[] = {140,240,800};
String notes5[] = {"G#4","F#4","D#4"};
int freqs5[] = {425,370,311};
int N5 = 3;

//A1
int lows1[] = {20,65,135};
int highs1[] = {64,130,240};
String notes1[] = {"D#5","C#5","A#4"};
int freqs1[] = {622,554,466};
int N1 = 3;

//A4
int lows4[] = {30,140,260,460};
int highs4[] = {130,250,400,600};
String notes4[] = {"B4","C5","D5","E5"};
int freqs4[] = {494,523,587,659};
int N4 = 4;

//A3
int lows3[] = {25,66,140,230};
int highs3[] = {65,105,210,310};
String notes3[] = {"A4","G4","F4","E4"};
int freqs3[] = {440,392,349,330};
int N3 = 4;

//A2
int lows2[] = {240,55,20};
int highs2[] = {340,130,54};
String notes2[] = {"C4","D4","C#4"};
int freqs2[] = {262,294,277};
int N2 = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

/* Note pins/voltages
 * 
 * D#5: A5/45
 * C#5: A5/88
 * A#4: A5/188
 * G#4: A5/300
 * F#4: A5/508
 * D#4: A5/680
 * 
 * E5: A4/533
 * D5: A4/310
 * C5: A4/187
 * B4: A4/89
 * 
 * A4: A3/44
 * G4: A3/85
 * F4: A3/187
 * E4: A3/288
 * 
 * C4: A2/308
 * D4: A2/88
 * C#4: A2/45
 */


void loop() {
  // put your main code here, to run repeatedly:
  int input5 = analogRead(A5);
  int input4 = analogRead(A4);
  int input3 = analogRead(A3);
  int input2 = analogRead(A2);
  int input1 = analogRead(A1);
//  Serial.print(current);
//  Serial.print(" ");
//  Serial.print(last);
//  Serial.print(" ");
  Serial.println(input5);
  /*
  Serial.print(input5);
  Serial.print(" ");
  Serial.print(input4);
  Serial.print(" ");
  Serial.print(input3);
  Serial.print(" ");
  Serial.println(input2);
  */

  if (current != 0 && last == current){
    tone(speakerPin,current);
    delay(20);
  } else {
    //Serial.println("off");
    noTone(speakerPin);
  }

  if (input1 == 0 && input2 == 0 && input3 == 0 && input4 == 0 && input5 == 0) {
    current = 0;
  } else {
    last = current;
    //Serial.println(input4);
    for (int i= 0; i < N5; i++){
      if (input5 > lows5[i] && input5 < highs5[i]){
        Serial.print(input5);
        Serial.print(" ");
        Serial.print(lows5[i]);
        Serial.print(" ");
        Serial.print(highs5[i]);
        Serial.print(" ");
        Serial.println(notes5[i]);
        current = freqs5[i];
      } 
    }
    
    for (int i= 0; i < N4; i++){
      if (input4 > lows4[i] && input4 < highs4[i]){
        Serial.print(input4);
        Serial.print(" ");
        Serial.print(lows4[i]);
        Serial.print(" ");
        
        Serial.print(highs4[i]);
        Serial.print(" ");
        Serial.println(notes4[i]);
        current = freqs4[i];
      }
    }
  
    for (int i= 0; i < N3; i++){
      if (input3 > lows3[i] && input3 < highs3[i]){
        Serial.print(input3);
        Serial.print(" ");
        Serial.print(lows3[i]);
        Serial.print(" ");
        Serial.print(highs3[i]);
        Serial.print(" ");
        Serial.println(notes3[i]);
        current = freqs3[i];
      }
    }
  
    for (int i= 0; i < N2; i++){
      if (input2 > lows2[i] && input2 < highs2[i]){
        Serial.print(input2);
        Serial.print(" ");
        Serial.print(lows2[i]);
        Serial.print(" ");
        Serial.print(highs2[i]);
        Serial.print(" ");
        Serial.println(notes2[i]);
        current = freqs2[i];
      }
    }

    for (int i= 0; i < N1; i++){
      if (input1 > lows1[i] && input1 < highs1[i]){
        Serial.print(input1);
        Serial.print(" ");
        Serial.print(lows1[i]);
        Serial.print(" ");
        Serial.print(highs1[i]);
        Serial.print(" ");
        Serial.println(notes1[i]);
        current = freqs1[i];
      }
    }
  }

  

  
}
