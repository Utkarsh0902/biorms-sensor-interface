/*
  Code to post sensor values every 10 mins
*/

// Global variables for average readings of temperature, pressure, humidity and ph
unsigned int temp_avg = 0, temp_count = 0;
unsigned int pres_avg = 0, pres_count = 0;
unsigned int humi_avg = 0, humi_count = 0;
unsigned int ph_avg = 0, ph_count = 0;

unsigned int pres_threshold = 0; // CHANGE THE VALUE BASED ON SENSOR

void updateValues() {
  
  // DUMMY VALUES FOR THE CURRENT READINGS
  unsigned int sensor  = analogRead(A0);
  unsigned int temp_curr=sensor, pres_curr=sensor, humi_curr=sensor, ph_curr=sensor;
  
  // ACTUAL CODE CONTINUES
  
  //check if alert needs to be called based on pressure value
  while(pres_curr>= pres_threshold){
    // trigger a buzzer if required
    
    Serial.print("{\"alert\":\"high-pressure\",");
    Serial.print("\"value\":"); Serial.print(pres_curr);Serial.println("}");
    //send an update again after 1 minute/ 30 seconds if the alert is still present.
    unsigned int alert_traget_time = millis() + 30000;
      while(pres_curr>= pres_threshold  && millis()< alert_traget_time){
      //read the pressure value again
      pres_curr = analogRead(A0);
    }
    
  }
  temp_avg = ((temp_count*temp_avg) + temp_curr)/(temp_count+1); temp_count++;
  pres_avg = ((pres_count*pres_avg) + pres_curr)/(pres_count+1); pres_count++;
  ph_avg = ((ph_count*ph_avg) + ph_curr)/(ph_count+1); ph_count++;
  humi_avg = ((humi_count*humi_avg) + humi_curr)/(humi_count+1); humi_count++;
}
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  unsigned long start_time = millis();
  //unsigned long duration = 600000; // 10 minutes
  unsigned long duration = 5000;  //  5 seconds
  while(1){
    // update values every 5 seconds
    updateValues();
    delay(200);
    
    // Check if millis() has overflown
    if(millis()< start_time){
      break;
    }
  
    //check if 10 minutes have elapsed
    if (millis() >= start_time + duration) {
      // print the values
      Serial.print("{\"temp\":"); Serial.print(temp_avg/20 + random(9));
      Serial.print(", \"pressure\":"); Serial.print(pres_avg/500 + random(2));
      Serial.print(",\"ph\":"); Serial.print(ph_avg/500 +7 + random(-2,2));
      Serial.print(",\"humidity\":"); Serial.print(humi_avg/25 + random(9)); Serial.println("}");
  
      //reset the values
      start_time = millis();
      temp_avg = 0; temp_count = 0;
      pres_avg = 0; pres_count = 0;
      humi_avg = 0; humi_count = 0;
      ph_avg = 0; ph_count = 0;
    } 
  }
}
