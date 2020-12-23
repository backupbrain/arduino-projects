float startValue = 0;
float endValue = 1000;
long timeout_ms = 5000;
long startTime;
long currentTime;

float tween(float prev, float next, float percent) {
  float result = (next-prev)*percent + prev;
  return result;
}
void setup() {
  Serial.begin(9600);
  startTime = millis();

}

void loop() { 
  currentTime = millis();
  long timeDifference = currentTime - startTime;
  float scale = 0;
  float value = 0;
  
  /*
  Serial.print(currentTime);
  Serial.print(" - ");
  Serial.print(startTime);
  Serial.print(" = ");
  Serial.println(timeDifference);
  /* */
  if (timeDifference < timeout_ms) {
    scale = (float)timeDifference/timeout_ms;
    value = tween(startValue, endValue, scale);
    
    Serial.print(timeDifference);
    Serial.print("/");
    Serial.print(timeout_ms);
    Serial.print(" = ");
    Serial.print(scale);
    Serial.print(" : ");
    Serial.println(value);
    /* */
  } else {
    startTime = currentTime;
    
  }
  
  delay(200);
}
