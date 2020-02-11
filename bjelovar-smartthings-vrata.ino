#define garagePin D7 // full opening
#define garagePinPartial D6 // partial opening
bool toggle = false;
//int A0 = 0;
//int A1 = 0;
//int A2 = 0;


//pin initializations
const int pinReedSensorClosed = A0; // A0 =  HIGH - door closed - Orange/WhiteOrange
const int pinReedSensorOpen = A1; // A1 = HIGH - door fully Open - Green/WhteGreen
const int pinReedSensorVent = A2; // A2 = HIGH - ventilation - Blue/WhiteBlue
const char* STATUS_MESSAGE_UNAVAILABLE = "Status unavailable.";
const char* CLOSED = "Closed"; // Orange/WhiteOrange
const char* OPEN = "Open"; //Green/WhteGreen
const char* MID = "In motion";
const char* VENT = "Ventilation"; //Blue/WhiteBlue
const char* message = STATUS_MESSAGE_UNAVAILABLE;

int pinState = 0;
int pinStatePrevious = 0;

int gatePartial(String command);

int doorStatus(String command);
int GarageRelay(String command);

void setup() {
    pinMode(pinReedSensorClosed, INPUT_PULLDOWN);
    pinMode(pinReedSensorOpen, INPUT_PULLDOWN);
    pinMode(pinReedSensorVent, INPUT_PULLDOWN);
    pinMode(garagePin, OUTPUT);
    pinMode(garagePinPartial, OUTPUT);
    digitalWrite(garagePin, HIGH);
    digitalWrite(garagePinPartial, LOW);
    Particle.variable("doorStatus", STATUS_MESSAGE_UNAVAILABLE, STRING);  //Particle.variable("doorStatus", STATUS_MESSAGE_UNAVAILABLE, STRING); 
    Particle.function("GarageRelay", GarageRelay);
    Particle.function("GarageRelayButton", GarageRelayButton);
    Particle.function("gatePartial", gatePartial); // This is what exposes the funtion to the internet!
}


void loop() {
    
   // loop over Reed switchs status to see if doors are open / closed / mid and send when changed to IFTTT
   
    const char* message = STATUS_MESSAGE_UNAVAILABLE;

    if(digitalRead(pinReedSensorClosed) ==1 and digitalRead(pinReedSensorOpen) ==0){
    pinState = 1; 
    }

    if(digitalRead(pinReedSensorClosed) ==0 and digitalRead(pinReedSensorOpen) ==0){
    pinState = 2; 
    }
    
    if(digitalRead(pinReedSensorClosed) ==0 and digitalRead(pinReedSensorOpen) ==1){
    pinState = 3; 
    }
  
    if(digitalRead(pinReedSensorClosed) ==0 and digitalRead(pinReedSensorOpen) ==0 and digitalRead(pinReedSensorVent) ==1){
    pinState = 4; 
    }
    
    if(pinState == 1 ) message = CLOSED;
    if(pinState == 2 ) message = MID;
    if(pinState == 3 ) message = OPEN;
    if(pinState == 4 ) message = VENT;

    if(pinState != pinStatePrevious) {
    Particle.variable("doorStatus", message, STRING);
    Particle.publish("doorStatus", message);
     delay(100); 
    pinStatePrevious = pinState;
    }
    else{
    pinStatePrevious = pinStatePrevious;
    }


    

}



    int GarageRelay(String command){
        
        if (command == "1") {  // nakon 20 sekundi pocinje otvaranje vrata /google ili alexa potpuno otvaranje ili zatvaranje
            delay(20000);
            digitalWrite(garagePin, LOW);
            delay(1000);
            digitalWrite(garagePin, HIGH);
            Particle.publish("GarageRelay", "Vrata aktivirana!", PRIVATE);
	        return 1;
        } 
        
        if (command == "2") {  // nakon 10 sekundi pocinje otvaranje vrata /google ili alexa potpuno otvaranje ili zatvaranje
            delay(10000);
            digitalWrite(garagePin, LOW);
            delay(1000);
            digitalWrite(garagePin, HIGH);
            Particle.publish("GarageRelay", "Vrata aktivirana!", PRIVATE);
	        return 1;
        }
        
         if (command == "open" and  "1") {  // nakon 10 sekundi pocinje otvaranje vrata /google ili alexa potpuno otvaranje ili zatvaranje - SmartThings button
            delay(10000);
            digitalWrite(garagePin, LOW);
            delay(1000);
            digitalWrite(garagePin, HIGH);
            Particle.publish("GarageRelay", "Vrata aktivirana!", PRIVATE);
	        return 1;
        }
        
            if (command == "3") {  // pocinje otvaranje vrata ODMAH
            digitalWrite(garagePin, LOW);
            delay(1000);
            digitalWrite(garagePin, HIGH);
            Particle.publish("GarageRelay", "Vrata aktivirana!", PRIVATE);
	        return 1;
        }
        

  } 
  
    int gatePartial(String command){ // djelomicno otvaranje vrata za ventilaciju za IFTTT
    char inputStr[64];
    command.toCharArray(inputStr,64);
    int i = atoi(inputStr);
            digitalWrite(garagePinPartial, LOW);
            delay(1000);
            digitalWrite(garagePinPartial, HIGH);
    toggle = false;
    // Respond
    return 1;
}


    int GarageRelayButton(String command){ // potpuno otvaranje vrata za IFTTT gumbic
    char inputStr[64];
    command.toCharArray(inputStr,64);
    int i = atoi(inputStr);
            delay(10000);
            digitalWrite(garagePin, LOW);
            delay(1000);
            digitalWrite(garagePin, HIGH);
    toggle = false;
    // Respond
    return 1;
}