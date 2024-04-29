

#include <SoftwareSerial.h>

const char LORA_Frequency = 915;
const char TX_Power_14  = 14; //up to 1W of EIRP is allowed for HF loRa and LoRaWAN
//*************************************************
const char SF12 = "SF12";
const char SF11 = "SF11";
const char SF10 = "SF10";
const char SF09 = "SF09";
const char SF08 = "SF08";
const char SF07 = "SF07";
const char SF06 = "SF06";
//spreading factor constants**************************
const char BW125 = "125";
const char BW250 = "250";
const char BW500 = "500";
//Supported Bandwidth options^^***********************
byte Command_Flag = 0;
const byte TxPin = 3; //arbitrary value
const byte RxPin = 4; //same thing -> change to reflect appropriate GPIO
//**************RTC Variables ***********************************************************
const char Year = 2024;
const char Month = 05;
const char Day = 12;
const char Hour = 4;
const char Minute = 30;
const char Second = 15;
//ALL arbitrary values to begin with -> will need to ballpark to begin with
char Transmit_data;
unsigned long last_transmit = 0;
const int transmit_period = 250; //this can be changed


SoftwareSerial E5_Serial(RxPin,TxPin); //E5_Serial what we'll be using -> this needs to be a global variable/class


void setup() 
{
  E5_Serial.begin(115200);
  delay(30);
  E5_Serial.write('AT'+'MODE=TEST'); //the device enters test mode
   while(E5_Serial.available()) //while 
  {
    Command_Flag = 1;// variable to indicate UART transfer status (will be stuck here)
    delay(10);
  }
  Command_Flag = 0;
//********************LoRa_E5 Configuration***************************************************************
   E5_Serial.write('AT'+'TEST=RFCNF,'+LORA_Frequency+','+ SF12+','+BW125+','+'10'+','+'10'+','+'14'); 
   //follows the structure: Frequency, Spreading Factor, Bandwidth, PreambleTX, PreambleRX, transmit Power 
   while(!E5_Serial.available()) //while 
  {
    Command_Flag = 1;// variable to indicate UART transfer status (will be stuck here)
    delay(10);
  }
  Command_Flag = 0;
//*********************Setting up the RTC*****************************************************************
char RTC_Initial_Time = Year+'-'+Month+'-'+Day+' '+Hour+':'+Minute+':'+Second;
E5_Serial.write('AT+RTC='+ RTC_Initial_Time);//step 1
delay(50);
E5_Serial.write('AT+RTC=ZONE, "+10:00"'); //setting up timezone for RTC
delay(50);
E5_Serial.write('AT+RTC=LEAPSEC,37'); //setting up leapseconds (has only ever been 37 leap seconds as of 2017)
delay(50);
Transmit_data = E5_Serial.write('AT+RTC=UTCMS');//receives the full RTC (in ms even) from the E5 module into a char buffer
delay(50);
//this apparently will receive the RTC value currently but will need to confirm
//initial timestamp***************************************************
//Using the built in RTC functionality -> since writing utilises a class, user defined functions cannot be used


} 
void loop() 
{
int current = millis(); //the time from which the program has started running in ms
 if(current-last_transmit >transmit_period)
 {
  last_transmit = current;
  Transmit_data = E5_Serial.write('AT+RTC=UTCMS');//gets the new RTC value into the transmit buffer ->confirm size of data with sizet()
  E5_Serial.write('AT+TEST=TXLRPKT,"{}"\n'+Transmit_data);
  //something here to show whether transmission has been successful
    while(E5_Serial.available()) //while 
  {//might want to include some additional trouble shooting
    Command_Flag = 1;// variable to indicate UART transfer status (will be stuck here)
    delay(10);
  }
  Command_Flag = 0;
  delay(100);
 }
}

