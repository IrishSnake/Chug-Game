#include <LiquidCrystal.h> 
//library to use lcd 

//Declare global variables to be able to use them throughout both methods
const int FSR_PIN = A0; // Pin connected to FSR/resistor divider 
const int LED_PIN1 = 7; 
const int LED_PIN2 = 6; 
const int LED_PIN3 = 4; 
const float VCC = 4.98; // Measured voltage of Ardunio 5V line 
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
long m, m2; 

//connects the lcd screen to the arduino board
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

void setup()  
{ 
  Serial.begin(9600); 
  //initialise LEDs for future use
  pinMode(FSR_PIN, INPUT); 
  pinMode(LED_PIN1, OUTPUT); 
  pinMode(LED_PIN2, OUTPUT); 
  pinMode(LED_PIN3, OUTPUT); 

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2); 

  // Print a message to the LCD. 
  lcd.print("Welcome to"); 
  lcd.setCursor(0, 1); 
  lcd.print("our Chug Game"); 
  delay(4000); 
  lcd.clear(); 

  int fsrADC = analogRead(FSR_PIN); 
  //lcd.setCursor(0, 1); 
  lcd.print("Place full"); 
  lcd.setCursor(0, 1); 
  lcd.print("glass to start"); 

  //empty glass is: 
  //16000 to 20000 
  
  delay(5000); 

} 

void loop()  
{ 
  int fsrADC = analogRead(FSR_PIN); 

  // If the FSR has no pressure, the resistance will be 
  // near infinite. So the voltage should be near 0. 

  
  if (fsrADC != 0) // If the analog reading is non-zero 
  { 

    //Following blocks determine the force thats applied to the pressure sensor
    // Use ADC reading to calculate voltage: 
    float fsrV = fsrADC * VCC / 1023.0; 
    // Use voltage and static resistor value to  
    // calculate FSR resistance: 
    float fsrR = R_DIV * (VCC / fsrV - 1.0); 
    Serial.println("Resistance: " + String(fsrR) + " ohms"); 
    
    // Guesstimate force based on slopes in figure 3 of 
    // FSR datasheet: 
    float force; 
    float fsrG = 1.0 / fsrR; // Calculate conductance 
    
    // Break parabolic curve down into two linear slopes: 
    if (fsrR <= 600)  
    force = (fsrG - 0.00075) / 0.00000032639; 
    
    else 
    force =  fsrG / 0.000000642857; 
    
    //print to serial monitor to determine pressure ranges for glass
    Serial.println("Force: " + String(force) + " g"); 


    //variable initiation
    float full = 150000.00;
    float nothing = 6000.00;
    float empty = 40000.00;
    int endTime = 0;
    int startTime = 0;
    boolean check1 = true;
    boolean check2 = true;

/*******************************************************************
  Ranges and logic for win was not finalised
  Works unde the right circumstances, but finicky
  Did not finish due to unforeseen circumstances


  empty glass~30,000.00
  full glass~150,000.00
  empty sesnor~ 8,000.00
*******************************************************************/


   //if nothing is on sensor, contiue showing welcome

   //if glass is placed, msg lift when ready
   if (force > 130000.00 && force < 180000.00)
   {
      lcd.clear();
      lcd.print("Lift glass when");
      lcd.setCursor(0, 1);
      lcd.print("ready...");
   }

    //start timer when glass is lifted
    else if (force <= 8000.00)
    { 
     //Timer
     //stop.millis();

      lcd.clear(); 
      lcd.setCursor(0,0); 
      m = millis(); 
      m2 = round(m/1000); 
      lcd.print(m2-9); 
      
      Serial.println(m2-9);

      while (check1 == true)
      {
  //save the starting time
  
        startTime = (m2-9);
        check1 = false;      
  //only saves the time the first time this statement runs
  //stops it being overridden  
      }
    }

    //when an empty glass is placed back down, win condition
    else if (force >20000.00 && force <40000.00)
    {
     
      while(check2 == true)
      {
  //save the end time
  
        endTime = ((round(millis()/1000))-9);
        check2 = false;

  //only saves the time the first time this statement runs
  //stops it being overridden 
      }

  int result = endTime-startTime;

       lcd.clear();
      lcd.print("Congratulations!");
      lcd.setCursor(0, 1);
      lcd.print("Your Time: ");
       lcd.setCursor(12, 1);
      lcd.print(result);
      lcd.setCursor(18, 1);
      lcd.print("s");


      
    }

 /*
    //Timer code 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    m = millis(); 
    m2 = round(m/1000); 
    lcd.print(m2-9); 
    Serial.println(m2-9); */

    //if full, contiue showing welcome

    //if glass lifted, start counting
    

    //if glass empty, win condition
    
  } 

  else 
  { 
    // No pressure detected 
  } 
} 

  

 

 