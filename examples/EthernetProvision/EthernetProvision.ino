//*****************************************************************************
//
// EthernetProvision - This example is identicle to the EthernetReadWriteString
//                     example, excpet it uses provisioning instead of using
//                     a hard-coded CIK.
//                     
//
// Copyright (c) 2013 Exosite LLC.  All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:

//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of Exosite LLC nor the names of its contributors may
//    be used to endorse or promote products derived from this software 
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
//*****************************************************************************

#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <Exosite.h>

/*==============================================================================
* Configuration Variables
*
* Change these variables to your own settings.
*=============================================================================*/
byte macData[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };        // <-- Fill in your Ethernet shield's MAC address here.

// Use these variables to customize what datasources are read and written to.
const String readString = "command&uptime";
const String writeString = "uptime=";

// Number of Errors before we try a reprovision.
const unsigned char reprovisionAfter = 3;

/*==============================================================================
* End of Configuration Variables
*=============================================================================*/

String returnString = "";
unsigned char errorCount = reprovisionAfter;  // Force Provision On First Loop
char macString[18];  // Used to store a formatted version of the MAC Address

class EthernetClient client;
Exosite exosite(&client);

/*==============================================================================
* setup
*
* Arduino setup function.
*=============================================================================*/
void setup(){  
  Serial.begin(115200);
  Serial.println(F("Boot"));
  
  // Create MAC Address String in the format FF:FF:FF:FF:FF:FF
  snprintf(macString, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
           macData[0], macData[1], macData[2], macData[3], macData[4], macData[5]);
  
  // Setup Ethernet Connection
  Ethernet.begin(macData);
  
  // Print Some Useful Info
  Serial.print(F("MAC Address: "));
  Serial.println(macString);
  
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
}

/*==============================================================================
* loop 
*
* Arduino loop function.
*=============================================================================*/
void loop(){
  // Check if we should reprovision.
  if(errorCount >= reprovisionAfter){
    if(exosite.provision("exosite", "ard-generic", macString)){
      errorCount = 0;
    }
  }
  
  //Write to "uptime" and read from "uptime" and "command" datasources.
  Serial.println("---- Do Read and Write ----");
  if (exosite.writeRead(writeString+String(millis()), readString, returnString)){
    Serial.println("OK");
    Serial.print("Returned: ");
    Serial.println(returnString);
    
    errorCount = 0;
  }else{
    Serial.println("Error");
    errorCount++;
  }

  delay(5000);
}

