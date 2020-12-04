/*
Flow Rate Sensor 
 */

// byte statusLed    = 13;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 9; 

// calibrationFactor is a scalar value that converts voltage reading to a flow value
float calibrationFactor = 16; 

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

void setup()
{
  
  // Be sure serial monitor is also set to 38400
  Serial.begin(38400);
   
  // pinMode(statusLed, OUTPUT);
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

/**
 * Main program loop
 */
void loop()
{
   
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 

    detachInterrupt(sensorInterrupt);
        
    // flow rate in L/min
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    

    oldTime = millis();
    
    // flow rate in mL/s
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // total volume of fluid measured
    // totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;

    frac = (flowRate - int(flowRate)) * 10;
    // Print the number of litres flowed in this second
    Serial.print("Flow Rate: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
