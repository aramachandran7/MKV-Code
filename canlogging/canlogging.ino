#include <SPI.h>
#include "mcp_can.h"
#include <SD.h>


/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

File myFile;

#define newLinePin 7

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_CAN    = 9;
const int SPI_CS_SD     = 4;

MCP_CAN CAN(SPI_CS_CAN);                                    // Set CS pin

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
bool isButtonPressed = true;
bool addLine = false;
unsigned long fileClosedTime = 0;
char filename[30] = "";
int driverNum = 0;
int fileNum = 0;
unsigned int messageCount = 0;


void setup()
{
    SERIAL.begin(500000);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println("Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");

    attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
    
    if (!SD.begin(4)) {
        SERIAL.println("SD initialization failed!");
        while(1);
    }
    SERIAL.println("SD initialization done.");
    sprintf(filename, "Drive%d-%d.csv", driverNum, fileNum);
}

void MCP2515_ISR()
{
    flagRecv = 1;
}

void loop()
{    
    if(flagRecv) 
    {                                   // check if get data

        flagRecv = 0;                   // clear flag
        unsigned long id= 0;

        if (fileClosedTime + 30000 < millis()){
          driverNum++;
          fileNum = 0;
          messageCount = 0;
          SERIAL.println("ND");
        }
//        SERIAL.print(fileClosedTime);
//        SERIAL.print(" millis");
//        SERIAL.println(millis());
//        SERIAL.println(messageCount);
        if (messageCount > 50000) {
          fileNum++;
          messageCount = 0;
          SERIAL.println("NF");
        }
//        SERIAL.println(messageCount);
        sprintf(filename, "Drive%d-%d", driverNum, fileNum);
//        SERIAL.println(filename);
        myFile = SD.open(filename, FILE_WRITE);

        // iterate over all pending messages
        // If either the bus is saturated or the MCU is busy,
        // both RX buffers may be in use and reading a single
        // message does not clear the IRQ conditon.
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            // read data,  len: data length, buf: data buf
            CAN.readMsgBufID(&id, &len, buf);

            SERIAL.print(millis());
            SERIAL.print(",");
            SERIAL.print(id);
            SERIAL.print(",");
            myFile.print(millis());
            myFile.print(",");
            myFile.print(id);
            myFile.print(",");
            messageCount++;
            
            for(int i = 0; i<len; i++)
            {
                SERIAL.print(buf[i]);
                SERIAL.print(",");
                
                myFile.print(buf[i]);
                myFile.print(",");
            }
            SERIAL.println();
            myFile.println();
        }
        
        myFile.close();
        fileClosedTime = millis();
    }
}
