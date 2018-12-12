
// Init the Pins used for PWM
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
 
// Init the Pins used for 10K pots
const int PotPin = A0;
// You had your input button connected to pin #1, but you were using serial messages....
// If you want to use serial messaging, then you cannot connect anything to pins 0 and 1.
const int PotSwitch = A2; // Button input is moved to A2. Also input pin mode is changed to INPUT_PULLUP.
 
// Init our Vars
int currentColorValueRed;
int currentColorValueGreen;
int currentColorValueBlue;
int currentColorValueRed1;
int currentColorValueGreen1;
int currentColorValueBlue1;
 
int button_press_count = 0; // Changed to use 0 = off, 1 = red adjust, 2 = green adjust, 3 = blue adjust, 4 = random
 
int buttonState = 0;
 
// Below are the variables used for debouncing the button input.
int buttonDebounceTime = 500; // This is the time in milliseconds to debounce the button.
bool buttonInputEnabled = true; // This is a variable to block button input while it is debouncing.
unsigned long buttonPress_beginTime = 0;
unsigned long buttonPress_currentTime = 0;
 
// Below are the variables used for running the mode #4 feature.
int randomColor_cycleTime = 300; // This is the time in milliseconds to cycle the random colors
unsigned long randomColor_cycle_beginTime = 0;
unsigned long randomColor_cycle_currentTime = 0;
 
 
 
// function prototypes---------
void buttonPressed(); // -- this is for debouncing the button input.
void writeToColorPins();
void generate_random_colors();
void generate_random_red_color();
void generate_random_green_color();
void generate_random_blue_color();
 
 
void setup()
{
  //Serial.begin(9600);
  //Serial.println("(serial speed test message)");
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(PotSwitch, INPUT_PULLUP);
 
}
 
 
void loop()
{
  if (buttonInputEnabled == true) { // The button is checked only if it is not de-bouncing
    buttonState = digitalRead(PotSwitch);
    if (buttonState == LOW) { // Check for a LOW state here, since that is the "button pressed" state.
      buttonPressed();
      button_press_count++;
      // Changed to use 0 = off, 1 = red adjust, 2 = green adjust, 3 = blue adjust, 4 = random
      if (button_press_count >= 7) {
        button_press_count = 0;
      }
      // The states that only require one change can be performed here:
      // Also this way, the analogRead(PotPin) is only performed once, when
      //Serial.print("button_press_count = ");
      //Serial.println(button_press_count);
      switch (button_press_count) {
        case 0:
          //Serial.println("(turning LEDs off)");
          currentColorValueRed = 0;
          currentColorValueGreen = 0;
          currentColorValueBlue = 0;
          break;
        case 1:
          generate_random_red_color();
          currentColorValueGreen = 0;
          currentColorValueBlue = 0;
          break;
        case 2:
          generate_random_green_color();
          currentColorValueRed = 0;
          currentColorValueBlue = 0;
          break;
        case 3:
          generate_random_blue_color();
          currentColorValueGreen = 0;
          currentColorValueRed = 0;
          break;
        case 4:
          generate_random_colors();
          randomColor_cycle_beginTime = millis();
          break;
        case 5:
          currentColorValueRed = 255;
          currentColorValueGreen = 255;
          currentColorValueBlue = 128;
          break;
         case 6:
          currentColorValueRed = 255;
          currentColorValueGreen = 0;
          currentColorValueBlue = 255;
          break;
        default:
          // nothing here
          break;
      }
      writeToColorPins();
      //buttonPressed();
    }
  }
 
 
  // The section below is for automatically repeating the random color generation for mode #4.
  if (button_press_count == 4) {
    randomColor_cycle_currentTime = millis();
    if (randomColor_cycle_currentTime > randomColor_cycle_beginTime) {
      if (randomColor_cycle_currentTime >= (randomColor_cycle_beginTime + randomColor_cycleTime)) {
        //Serial.println("(auto-changing random colors)");
        generate_random_colors();
        writeToColorPins();
        // reset the start time to the current time so that the cycle will repeat:
        randomColor_cycle_beginTime = millis();
      }
    }
    else {
      randomColor_cycle_beginTime = millis(); // millis() rollover condition check
    }
  }
 
 
  // The section below is for debouncing the button input.
  if (buttonInputEnabled == false) {
    buttonPress_currentTime = millis();
    if (buttonPress_currentTime > buttonPress_beginTime) {
      if (buttonPress_currentTime >= (buttonPress_beginTime + buttonDebounceTime)) {
        buttonInputEnabled = true;
        //Serial.println("(button re-enabled)");
      }
    }
    else {
      buttonPress_beginTime = millis(); // millis() rollover condition check
    }
  }
 
}// end of main loop()
 
 
 
void buttonPressed() {
  //Serial.println("buttonPressed()");
  buttonInputEnabled = false;
  buttonPress_beginTime = millis();
}
 
 
void generate_random_colors() {
  //Serial.println("generateRandomColors()");
  generate_random_red_color();
  generate_random_green_color();
  generate_random_blue_color();
}
 
void generate_random_red_color() {
  currentColorValueRed = random(0, 255);
  //Serial.print("random red = ");
  //Serial.println(currentColorValueRed);
}
 
void generate_random_green_color() {
  currentColorValueGreen = random(0, 255);
  //Serial.print("random green = ");
  //Serial.println(currentColorValueGreen);
}
 
void generate_random_blue_color() {
  currentColorValueBlue = random(0, 255);
  //Serial.print("random blue = ");
  //Serial.println(currentColorValueBlue);
}
 
 
void writeToColorPins() {
  //Serial.println("writeToColorPins()");
  analogWrite(redPin, currentColorValueRed);
  analogWrite(bluePin, currentColorValueBlue);
  analogWrite(greenPin, currentColorValueGreen);
}
