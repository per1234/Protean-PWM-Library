#include <PWM.h>
#include <Wire.h>

Protean::PWM pwmLogger(0x69);

void help()
{
  Serial.println("e - Toggle PWM echo");
  Serial.println("v - Check firmware compatibility with library");
  Serial.println("w - Write timing, usage: w [channel#] [timing]");
  Serial.println("r - Read timing, usage: r [channel#]");
  Serial.println("d - Start record demo, usage: d [channel#]");
  Serial.println("b - Trigger reboot");
  Serial.println("? - Print this message");
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  pwmLogger.reset();

  Serial.println("  _ \\ \\ \\      / \\  |       |                                  ");
  Serial.println("  __/  \\ \\ \\  / |\\/ | ____| |      _ \\   _` |   _` |   -_)   _|");
  Serial.println(" _|     \\_/\\_/ _|  _|      ____| \\___/ \\__, | \\__, | \\___| _|  ");
  Serial.println("                                       ____/  ____/            ");                                     
  
  help();
}

bool echo = true;

void loop()
{
  int c = Serial.read();
  if(c != -1)
  {
    switch(c)
    {
      // Print help
      case '?':
        help();
        break;
        
      // Toggle echo
      case 'e':
        echo = !echo;
        pwmLogger.setEcho(echo);
        break;

      // Check firmware compatibility
      case 'v': 
        Serial.print("Firmware compatible: ");
        Serial.println(pwmLogger.checkVersion());
        break;
        
      // Write timing on channel
      case 'w':
      {
        int channel = Serial.parseInt();
        unsigned int timing  = Serial.parseInt();
        Serial.print("Writing ");
        Serial.print(timing, DEC);
        Serial.print("us to channel "); Serial.println(channel, DEC);
        pwmLogger.writeTimingRaw(channel, timing);
        break;    
      }

      // Read timing on channel
      case 'r':
      {
        int channel = Serial.parseInt();

        Serial.print("Reading from channel "); Serial.println(channel, DEC);
        
        unsigned int t = pwmLogger.readTimingRaw(channel);
        Serial.print(t, DEC); Serial.println("us");
        break;    
      }

      // Reboot the board
      case 'b':
        pwmLogger.reset();
        break;

      // Run record/playback demo
      case 'd':
      {
        const int samples = 200;
        int recording[samples];
        int c = Serial.parseInt();
                
        pwmLogger.setEcho(true);

        Serial.println("Recording...");
        for(int i = samples; i--;)
        {
          recording[i] = pwmLogger.readTimingRaw(c);
          delay(20); 
        }

        Serial.println("Replaying...");
        pwmLogger.setEcho((echo = false));
        for(int i = samples; i--;)
        {
          pwmLogger.writeTimingRaw(c, recording[i]);
          delay(20);  
        }
        Serial.println("Done");
        pwmLogger.setEcho((echo = true));
        
        break;        
      }
    }
  }
}

