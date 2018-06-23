
const int analogOutPin = 9; // Analog output pin that the LED is attached to
int outputValue = 0;        // value output to the PWM (analog out)
String inString = "";

void setup(){
  
 Serial.begin(115200); 
}

void loop(){
 if(Serial.available()>0){
   char DatoSerial = Serial.read();
   if(DatoSerial == 'a'){
      int LecturaADC = analogRead(A0);
      Serial.write(LecturaADC>>8); 
      Serial.write(LecturaADC);
   }
   else if (isDigit(DatoSerial)){
    inString += (char)DatoSerial;
    analogWrite(analogOutPin, inString.toInt());
    inString = "";
   }
 } 
}
