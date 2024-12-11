
// Current and previous state of the button.
volatile bool buttonPressed = false;

// Counter to keep track of the LED sequence.
int counter = 0;

// Variables to implement button debouncing.
unsigned long lastDebounceTime  = 0;
unsigned long debounceDelay     = 150;

unsigned long waitDelay = 5000;
unsigned long lastCounterTime = 0;

// Array to store LED pins.
int LEDS[] = {LED_D0, LED_D1, LED_D2, LED_D3};
int NUM_LEDS = 4;

// Arrays of relays and user LEDs
int relayOutputs[] = {D0, D1, D2, D3};
// Compute the number of relays/LEDs based on the size of the relayPins array
int numRelays = 4;


void setup() {
  // Initialize Opta user LEDs.
  for(int i = 0; i < NUM_LEDS; i++) {
    pinMode(LEDS[i], OUTPUT);
    pinMode(relayOutputs[i], OUTPUT); 
  }
  
  // Set up the interrupt on USER_BTN to trigger on a rising edge (when the button is pressed)
  attachInterrupt(digitalPinToInterrupt(A0), buttonISR, FALLING);
}

/**
  Interrupt service routine (ISR)
  Set the variable buttonPressed to true

  @param None.
*/
void buttonISR() {
  buttonPressed = true;
}

void loop() {
  // If the button was pressed
  if (buttonPressed) {
    // Debouncing routine.
     lastCounterTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();

      // Only increment the counter if the new button state is HIGH.
      if(counter < NUM_LEDS){
        counter++;
      }
      else{
        counter = 0;
      }
      
      // Reset the button pressed flag
      buttonPressed = false;
    }
  }

   if ((millis() - lastCounterTime) > waitDelay) {
      

      // Scan from the first LED to the last.
  for (int i = 0; i < NUM_LEDS; i++) {
    // Turn on the LED.
    // Wait for 50 milliseconds.
    digitalWrite(LEDS[i], HIGH); 
    digitalWrite(relayOutputs[i], HIGH);
    delay(200); 
    
  }
  
  // Scan back from the last LED to the first.
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    // Turn on the LED.
    // Wait for 50 milliseconds.
    
    digitalWrite(LEDS[i], LOW); 
    digitalWrite(relayOutputs[i], LOW); 
    delay(200); 
  }
  
  }


  // Control LED states.
  changeLights();
}

/**
  Control the status LEDs based on a counter value.
  Turns off all LEDs first, then turns on the LED 
  corresponding to the current counter value.

  @param None.
*/
void changeLights() {
  // Turn off all user LEDs.
  for(int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LEDS[i], LOW);
    digitalWrite(relayOutputs[i], LOW);
  }

  // Turn on the selected user LED.
  if(counter > 0) {
    digitalWrite(relayOutputs[counter-1], HIGH);
    digitalWrite(LEDS[counter - 1], HIGH);
    
  }
}
