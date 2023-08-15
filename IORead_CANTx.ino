#include <SPI.h>
#include <mcp2515.h>

const int ledPin = 8;
const int swPin = 9;
const int intervalTime = 1000;

int swState = LOW; //LOW == 0
int ledState = LOW;
unsigned long previousTime = 0;
int sendFlag = 0; //for send CANmsg, 0:Disable, 1:Enable

struct can_frame canMsg;
MCP2515 mcp2515(10); //chip select CS pin10

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(swPin, INPUT);

  //setup for MCP2515(CAN)
  Serial.begin(9600);
  SPI.begin(); //begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ); //sets CAN at speed 125Kbps and clock 8MHz
  mcp2515.setNormalMode();
}


void loop() {
  swState = digitalRead(swPin);
  digitalWrite(ledPin, swState);
  periodCANMsg();
}

void periodCANMsg()
{
  unsigned long currentTime;

  currentTime = millis();
  if (currentTime - previousTime >= intervalTime) {
    previousTime = currentTime;
    
    /*if (swState == 1) {
      ledState = !ledState;
    }
    else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);*/

    canMsg.can_id = 0x700;
    canMsg.can_dlc = 8;
    canMsg.data[0] = 0x9; //DI(9)
    canMsg.data[1] = swState;
    canMsg.data[2] = 0x0;
    canMsg.data[3] = 0x0;
    canMsg.data[4] = 0x0;
    canMsg.data[5] = 0x0;
    canMsg.data[6] = 0x0;
    canMsg.data[7] = 0x0;
    mcp2515.sendMessage(&canMsg);
  }
}
