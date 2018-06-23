
const int analogOutPin = 9; // Analog output pin that the LED is attached to
int outputValue = 0;        // value output to the PWM (analog out)


void setup(){
  
 Serial.begin(9600); 
}

void loop(){
 if(Serial.available()>0){
   char DatoSerial = Serial.read();
   if(DatoSerial == '1'){
      int LecturaADC = analogRead(A0);
      Serial.write(LecturaADC>>8); 
      Serial.write(LecturaADC);
   }
      else{  
      analogWrite(analogOutPin, DatoSerial); 
      Serial.write(0x00); 
      Serial.write(DatoSerial);
    
     }
 } 
  
}
