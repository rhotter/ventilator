
/*
  Checks if Serial is available and initializes Raspberry Pi Communication
  
  @params:
  int maxWaitTime - Amount of time to wait for pi to start (milliseconds).

  TODO: Arduino is the first to talk, and the Pi responds

*/
#include "piCommunication.h"
#include "controls.h" // for the param update
#include "utilities.h"


/*
  Handshake/elbowbump with Pi.
  Arduino -> Pi -> Arduino.

  Returns 1 if connected, 0 if not.
*/
int initializePiCommunication(int maxWaitTime, int pingInterval) {
  int BAUD_RATE = 9600 // move
  Serial.begin(BAUD_RATE);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Sends message to pi until it gets a response
  int count = 0;
  while (Serial.available() <= 0) { // Serial.available says how many bytes available to read
    Serial.print('elbowbump\n');   // send a capital A
    count += pingInterval;
    delay(pingInterval);
    if (count > maxWaitTime) {
      return 0;
    }
  }

  if (Serial.available() > 0) {
    String response = Serial.readStringUntil('\n'); // doesn't include '/n'
    if (response.equals('elbowbump')){
      return 1;
    }
  }

  return 0;
}


/*
  Gets initial setup parameters from pi.
*/
int setupInitialParametersFromPi() {
    String initialParameterString = getPiString();
    parsePiString(initialParameterString);
    return 1;
}


/*
  Package and send data to Pi, and reset the storage stuff. Error checking is done elsewhere.
*/
void sendData(pressureAvg, flowAvg){

  int data[2] = {pressureAvg, flowAvg};
  int checksum = createCheckSum(data, 2); // TODO: this function must be implemented

  Serial.write('D');
  Serial.write(checksum);
  Serial.write(pressureAvg);
  Serial.write(flowAvg);
  Serial.write(status); // TODO; status must be implemented (either 'G', or an error code)
  Serial.write('\n');

  return; 
}

/*
  Helper function to verify checksum from first character of string.
*/
int isChecksumValid(String piString) {
  int checkSumVal = piString.charAt(0); // ^ XOR
  
  int testVal = byte(piString.charAt(1));
  for (int i = 2; i < piString.length(); i++){
    testVal = testVal ^ byte(piString.charAt(i));
  }

  if (checkSumVal == testVal){
    return 1;
  }
  return 0;
}

/*
  Rasp Pi sends N-length string, this function returns N-1 character long String object.
  Drops the '\n' at the end of the message.
*/
String getPiString(){
  String msg = Serial.readStringUntil('\n');
  return msg;
}


void parsePiString(String str){
  char ch;

    return -1;
}