
#include <SoftwareSerial.h>

SoftwareSerial sim800l(0, 1); 

#define LED 7    
#define LED1 6

//pressure:
#define kpa2atm 0.00986923267

// pin defs
int pressurePin = 0;

// variables
int val;
float pkPa; // pressure in kPa
float pAtm; // pressure in Atm
float pres;

unsigned long time;


void setup()
{
 
  pinMode(LED, OUTPUT); //The button is always on HIGH level, when pressed it goes LOW
  pinMode(LED1, OUTPUT);
 
  sim800l.begin(9600);   //Module baude rate, this is on max
  Serial.begin(9600);   
  delay(1000);
}
 
void loop()
{
  /* get the pressure */
  val = analogRead(pressurePin);
  pkPa = ((float)val/(float)1023+0.095)/0.009;
  pAtm = kpa2atm*pkPa;
  pres=((pkPa*0.101972)*98.0665)/6.895;
 
  /* send pressure to serial port */
  Serial.print(pres);
  Serial.print("psi   ");
  Serial.print(pAtm);
  Serial.println("Atm   ");
  delay(1000);

  //////////////////////////////////////////////////////////////////////////////

  if (pres >= 140) {                                      
    digitalWrite(LED, HIGH);
    digitalWrite(LED1, LOW);
    Serial.println("\nPressure exceeded than 140 psi");   //Shows this message on the serial monitor
    delay(200);                                           //Small delay to avoid detecting the variable pressure
    
    SendSMS1();                                           //function is called
    

 } else{
    digitalWrite(LED, LOW);
    digitalWrite(LED1, HIGH);
 }
 
  if (sim800l.available()){                        //Displays on the serial monitor if there's a communication from the module
    Serial.write(sim800l.read()); 
  }
}



void SendSMS1()
  {
    
    Serial.println("\nSending SMS...");            //Show this message on serial monitor
    sim800l.print("AT+CMGF=1\r");                  //Set the module to SMS mode
    delay(100);
    sim800l.print("AT+CMGS=\"+919969459522\"\r");  //Your phone number with country code
    delay(500);
    sim800l.print("Welcome APML");                 //text to send to the phone number
    delay(500);
    sim800l.print((char)26);                       // (required according to the datasheet)
    delay(500);
    sim800l.println();
    Serial.println("Pressure is above 140 psi");
    delay(500);
  
  }
