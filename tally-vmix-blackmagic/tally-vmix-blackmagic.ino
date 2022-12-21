#include <BMDSDIControl.h>
#include <SPI.h>
#include <Ethernet.h>

int VMIX_PORT = 8099;

#define TALLY_COUNT 100

//MAC is A8:61:0A:AE:74:D2
//OUI is A8:61:0A
EthernetClient client;
EthernetServer server(80);

BMD_SDITallyControl_I2C sdiTallyControl(0x6E);            // define the Tally object using I2C using the default shield address

// the setup function runs once when you press reset or power the board
void setup()
{
  
  //TODO
  //Load settings from SD Card
  //look flag bite to see if EEPROM is initialized, and if not zero out positions we'll be using
  //check and see if we have a MAC address set
  //randomly generate MAC address if we don't have one starting with A8:61:0A:00:00
  //check if using DHCP
  //if not using DHCP snag ip, subnet and gateway and use that

  char tallymap[TALLY_COUNT];
  
  tallymap[4] = 1;      // map vmix input 1 to ATEM input 1 
  tallymap[5] = 2;      // map vmix input 1 to ATEM input 1 
  tallymap[6] = 3;      // map vmix input 1 to ATEM input 1 
  tallymap[7] = 4;      // map vmix input 1 to ATEM input 1 
  tallymap[8] = 5;      // map vmix input 1 to ATEM input 1 


  pinMode(13, OUTPUT);                                     // initialize digital pin 13 as an output
  Serial.begin(115200);

  // the media access control (ethernet hardware) address for the shield:
  byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x74, 0xD3 };  
  //the IP address for the shield:
  //byte ip[] = { 10, 0, 0, 177 };

  Serial.println(F("Network..."));
  Ethernet.begin(mac);

  byte vmixip[] = { 10, 9, 11, 106 };
  
  delay(2000);

  IPAddress myIPAddress = Ethernet.localIP(); 
  Serial.println(myIPAddress);
    
  /* vMix connections 
  Serial.println(F("vMix..."));
  if(client.connect(vmixip, VMIX_PORT)) {
    Serial.println(F("OK"));
    client.println("SUBSCRIBE TALLY");
  } else {
    Serial.println(F("FAIL"));
  }
  */
  
  server.begin();

  sdiTallyControl.begin();                                 // initialize tally control
  sdiTallyControl.setOverride(true);                       // enable tally override
}
    
// the loop function runs over and over again forever
void loop()
{

  while (client.available()) {
    String rawdata = client.readStringUntil('\r\n');
    Serial.println(rawdata);
 
    //handleData("TALLY OK 1222010000");
    handleData(rawdata);
    
  }


  //WEB SERVER
  webclient();
}

void handleData(String rawdata)
{

  //loop through the data

  Serial.println("data is");
  Serial.println(rawdata);
  Serial.println(rawdata.indexOf("TALLY"));
  
  if (rawdata.indexOf("TALLY") == 0) {
    Serial.println("tally data");

    String tallydata = rawdata.substring(9);
    Serial.println(tallydata);

    int str_len = tallydata.length();

    char tallies[str_len];
         
    // Copy it over 
    tallydata.toCharArray(tallies, str_len);

//    for(int i=0;i<10;i++) {
//
//      //todo convert vmix input (i) to blackmagic camera number
//      
//      if (tallies[i] == '1') {  //program
//        Serial.print(F("vMix Input "));
//        Serial.print(i + 1);
//        Serial.print(F(" -> Atem Camera "));
//        Serial.print(tallymap[i + 1]);
//        Serial.print(F(" = PGM\n"));
//        
//        sdiTallyControl.setCameraTally(                         
//          tallymap[i + 1],                                                     // Camera Number
//          true,                                                  // Program Tally
//          false                                                  // Preview Tally
//        );
//      } else if (tallies[i] == '2') { //preview
//        Serial.print(F("vMix Input "));
//        Serial.print(i + 1);
//        Serial.print(F(" -> Atem Camera "));
//        Serial.print(tallymap[i + 1]);
//        Serial.print(F(" = PVW\n"));
//        
//        sdiTallyControl.setCameraTally(                          
//          tallymap[i + 1],                                                     // Camera Number
//          false,                                                  // Program Tally
//          true                                                  // Preview Tally
//        );
//
//      } else { //tally off
//        Serial.print(F("vMix Input "));
//        Serial.print(i + 1);
//        Serial.print(F(" -> Atem Camera "));
//        Serial.print(tallymap[i + 1]);
//        Serial.print(F(" = OFF\n"));
//
//        sdiTallyControl.setCameraTally(                        
//          tallymap[i + 1],                                                     // Camera Number
//          false,                                                  // Program Tally
//          false                                                  // Preview Tally
//        );
//      }
//      
//    }
    
    //tallystate = tallies;
    
  } else {
    //we didn't get tally data, got something else
  }
  
}


  
