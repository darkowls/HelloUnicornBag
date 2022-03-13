// Altered Carbon Style Bagpack
// Stub code currently

// Defining pins
int RelayPin=2;              // Pin for relay
int RReedPin=3;              // Right Reed Switch Pin
int LReedPin=4;              // Left Reed Switch Pin
int SwitchPin=5;             // Light Mode Pin Switch
byte OnDelay=1;               // Delay before turning on LEDs
byte OffDelay=3;             // Delay before turning off LEDs
byte CurState = 0;            // Current State (0=off, 1=turning on, 2=on, 3= turning off)
uint32_t CurSec =0;           // Runtime seconds
uint32_t EventSec =0;         // Time of event
int SecChanged=0;             // Second Changed Flag
 
void setup() {
  Serial.begin(115200);
  pinMode(RelayPin,OUTPUT);     // Setting Relay Pin
  pinMode(RReedPin,INPUT_PULLUP);      // Setting Right Reed Switch Pin
  pinMode(LReedPin,INPUT_PULLUP);      // Setting Left Reed Switch Pin
  pinMode(SwitchPin,INPUT_PULLUP);     // Setting Light Mode Switch Pin
  Serial.println("CarbonBag Init");                //testing serial print 
  Serial.println("Pins used:"+String(RReedPin)+"|"+String(LReedPin)+"|"+String(SwitchPin));
  digitalWrite(RelayPin,LOW);   // Turning on light
  delay(5000);                 // Waiting 20 seconds
}

void loop() {

  if (millis()/1000 != CurSec){   // Timer and debug part. If timer moved 1 second ahead - we doing EVERYTHING once persecond to conserve power
    CurSec=millis()/1000;
    SecChanged=1;
    Serial.println(String(CurSec)+"s:Right Reed:"+String(digitalRead(RReedPin))+"|"+"Left Reed"+String(digitalRead(LReedPin))+"|"+"Switch:"+String(digitalRead(SwitchPin))+"State:"+String(CurState));
  }
  else{
    SecChanged=0;
  }

  //  Detection part
  if (SecChanged==1){
    if (digitalRead(RReedPin)==HIGH and digitalRead(LReedPin)==HIGH){                     // Detecting open event. If both Reed switches are open (Bagpack open)
      if (CurState==0 or CurState==3){                          // And if we are currently closed or closing
        CurState = 1;                                           // Changing state on "Opening"
        EventSec=CurSec;                                        // Remembering Event time
      }
    }
    if (millis()/1000 >=(EventSec+OnDelay)and CurState == 1){   // Detecting opened complete event. If on delay is passed
      CurState = 2;
    }
    if (digitalRead(RReedPin)==LOW and digitalRead(LReedPin)==LOW){                       // Detecting closed happen event. If both Reed switches are closed (Bagpack closed)
      if (CurState==2 or CurState==1){                          // And if we are currently open or opening
        CurState = 3;                                           // Changing state on "Closing"
        EventSec=CurSec;                                        // Remembering Event time
      }
    }
    if (millis()/1000 >=(EventSec+OffDelay) and CurState == 3){ // Detecting closed complete event. If off delay is passed
      CurState = 0;
    }
  }
  //Setting lights according to state - every second
  if (SecChanged==1){                // every second
    if (CurState == 0 and digitalRead(LReedPin)==LOW){             // If we have to turn off the light and light switch NOT turned to constant on
      digitalWrite(RelayPin,HIGH);              // Turning off light
    }
    if (CurState == 2 or digitalRead(LReedPin)==HIGH){             // If we have to turn on the light or light switch turned to constant on
      digitalWrite(RelayPin,LOW);              // Turning on light
    }
  }
}
