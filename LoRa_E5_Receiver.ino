
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
/*const char Year = 2024;
//const char Month = 05;
const char Day = 12;
const char Hour = 4;
const char Minute = 30;
const char Second = 15;
*/
//ALL arbitrary values to begin with -> will need to ballpark to begin with
byte RxData; //16 byte character arrays to store data in -> arbitrary depending on the size of the RTC value

SoftwareSerial E5_Serial(RxPin,TxPin); //E5_Serial what we'll be using -> this needs to be a global variable/class

void setup() {
 Serial.begin(115200);
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
  delay(100);
  }
  
void loop()
 {
  char rxData = receive_Data();
  if(sizeof(rxData)>2)
    {
      Serial.print(rxData);
      printf(rxData,"\n");
    }
    delay(200);
  }

char receive_Data()
{
  E5_Serial.write('AT+TEST=RXLRPKT\n'); //continuous mode to receive packets
  while(E5_Serial.available()>0) //while the UART has been freed up
  {
    RxData =E5_Serial.read(); //stores UART data into RxData variable
  }
  char output = 'STAMP:'+RxData;
  return output;
}





