/*
Flow Rate Sensor 
 */

// byte statusLed    = 13;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 19; // Free pin on Teensy (19, 18, or 39)

// calibrationFactor is a scalar value that calibrates voltage reading to a flow value;
// 8 for Sensirion, 16 for low-flow meter
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
  // digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
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
    
    // Print the flow rate for this second in litres / minute
    // Serial.print("Flow rate: ");
    // Serial.print(int(flowRate));  // Print the integer part of the variable
    // Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate - int(flowRate)) * 10;
    // Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    // Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("Flow Rate: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    // Serial.print("  Output Liquid Quantity: ");             // Output separator
    // Serial.print(totalMilliLitres);
    // Serial.println("mL"); 

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
