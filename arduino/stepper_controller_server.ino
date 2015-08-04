/*
  Web server

 This sketch makes the Arduino Ethernet shield into a server that 
 listens for a daq computer to connect through ethernet TCP socket 
 and upload a set of commands to control the stepper motor.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 tutorials modified
 by Ronaldo Ortez
 2 Jul 2015

*/
#include <stdlib.h> 
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include <SPI.h>
#include <Ethernet.h>

#include <utility/w5100.h>
#include <utility/socket.h>
//#include <util.h>

// network configuration-------------------------------------------------------

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
uint8_t mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x6D, 0xFA};

byte client_ip[]= {10,95,100,139};  // numeric IP for MIDAS front-end

// Set the static IP address to use if the DHCP fails to assign
uint8_t s_ip[4]={10, 95, 100, 69};
uint8_t s_gateway[4]={10, 95, 100, 19};
uint8_t s_subnet[4] = {255, 255, 255, 0};

// use port 80 to communicate
//EthernetServer server = EthernetServer(80);

//Our socket 0 will be opened in raw mode
SOCKET s;

//define receive buffer parameters
byte rbuf[10];//receive buffer
int rbuflen; //receive buffer length
byte cbuf[1];//confirm buffer 

//Sepper Motor configuration-------------------------------------------------

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #1 (M1 and M2)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);

void setup()
{
  AFMS.begin(110);
  myMotor->setSpeed(1); // 1rpm

  //Start the serial output
  Serial.begin(115200);//this is the highest baud for the Ethernet shield

  Serial.println("Initializing W5100 chip and TCP socket");
  W5100.init();
  W5100.writeSnMR(s, SnMR::TCP); // Set type of socket in mode register

  // Assign network properties to chip.
  W5100.setMACAddress(mac);
  W5100.setIPAddress(s_ip);
  W5100.writeSnPORT(s, 5555); //set port to 80
  W5100.setSubnetMask(s_subnet);
  W5100.setGatewayIp(s_gateway);

 // W5100.execCmdSn(s, Sock_OPEN);
  
  //W5100.execCmdSn(s, Sock_LISTEN);

  
}

void loop()
{
  Serial.println("Opening socet and listen for connections");
  
  W5100.execCmdSn(s, Sock_OPEN);
  W5100.execCmdSn(s,Sock_LISTEN);
  uint8_t status = W5100.readSnSR(0);
  int count = 1;

 // check Status Register to find out if the connection was established
 // while ( (W5100.readSnSR(s) & SnSR:: ESTABLISHED) == SnSR::ESTABLISHED){
 while(W5100.readSnSR(0)==20) {
      delay(1);
      if (count ==2) Serial.println("Listening for a connection...");
      count++;
 }

 count = 1; 
 // wait for client to connect
 while ( W5100.readSnSR(0) ==23) {
     if( count <2) Serial.println("Connection established! What is the daq sending?");

     // wait for client message
     while ((rbuflen = W5100.getRXReceivedSize(s))>0) {

        Serial.println("Incoming packet detected"); 
        W5100.recv_data_processing(s, rbuf, rbuflen);
        W5100.execCmdSn(s, Sock_RECV);
        
        char* msg = (char*)rbuf;

        Serial.print("Raw message transmitted is: ");
        Serial.write(msg);
        Serial.println();

        int steps = atoi(msg);
        
        Serial.print("Steps = ");
        Serial.println(steps);

        if(steps>0) {
          Serial.print("Moving the stepper motor ");
          Serial.print(steps);
          Serial.println(" steps Forward");
          myMotor->step(steps,FORWARD, SINGLE);
          
        } else if (steps<0) {
          Serial.print("Moving stepper motor ");
          Serial.print(-1*steps);
          Serial.println(" steps Backward");
          myMotor->step(-1*steps,BACKWARD, SINGLE);
          
        }else {
          Serial.println("Instrunctions not interpretted");
        }
        // Reset the receive bit and zero receive buffer
        W5100.writeSnIR(s,0x4);
        for (int i = 0; i< sizeof(rbuf);i++) {
            rbuf[i] = (char ) 0;
        }
        
        //now lets send a confirmation message to the daq computer
        Serial.println("Stepper motor finished, ready for next command");
        uint8_t cmsg = 1;
        uint8_t* tmsg = &cmsg;
        cbuf[0]=1;
        
        W5100.send_data_processing(s, cbuf, 1);
        W5100.execCmdSn(s, Sock_SEND);
        
        uint8_t trans = W5100.getTXFreeSize(s);
        if(trans<1) {
          Serial.println("Could not send feedback");
        } else{
          Serial.println("Feedback sent");
        }
        count++;
        
        //Lets flush the serial buffer to make sure nothing is left.
        Serial.flush();
        
      }//end connection
      if (count % 5==0)  Serial.println("5 loops in listening");
  }//end open and listening
  
  //release the motor
  myMotor->release();
  
  //close the socket
  Serial.println("Closing socket now");
  W5100.execCmdSn(s, Sock_CLOSE); 
}//loop

byte socketStat[MAX_SOCK_NUM];

void ShowSockStatus()
{
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    Serial.print(F("Socket#"));
    Serial.print(i);
    uint8_t s = W5100.readSnSR(i);
    socketStat[i] = s;
    Serial.print(F(":0x"));
    Serial.print(s,16);
    Serial.print(F(" "));
    Serial.print(W5100.readSnPORT(i));
    Serial.print(F(" D:"));
    uint8_t dip[4];
    W5100.readSnDIPR(i, dip);
    for (int j=0; j<4; j++) {
      Serial.print(dip[j],10);
      if (j<3) Serial.print(".");
    }
    Serial.print(F("("));
    Serial.print(W5100.readSnDPORT(i));
    Serial.println(F(")"));
  }
}

