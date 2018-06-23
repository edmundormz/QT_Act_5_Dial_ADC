void setup(){
  
 Serial.begin(9600); 
}

void loop(){
 if(Serial.available()>0){
   char DatoSerial = Serial.read();
   if(DatoSerial == 0x30){
      int LecturaADC = analogRead(A0);
      Serial.write(LecturaADC>>8); 
      Serial.write(LecturaADC);
   }
 } 
  
}
