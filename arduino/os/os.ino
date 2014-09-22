#include <LiquidCrystal.h>
#include <Keypad.h>


//LCD setup
LiquidCrystal lcd(53, 51, 49, 47, 45, 43, 41);
int backlight = 52;

//Keypad setup using library
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {23, 25, 27, 29};
byte colPins[cols] = {22, 24, 26};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );


//Other variables
char result = 0;

//Menu data

//Menu 0 - Main menu
char menu0_1[18] = "1: Manual Control";
char menu0_2[11] = "2: Scripts";
char menu0_3[12] = "3: Settings";
char menu0_4[15] = "4: Shutdown Pi";
char* menu0[4];


//Menu 1 - Manual control
char menu1_1[11] = "1: Engines";
char menu1_2[14] = "2: Decouplers";
char menu1_3[17] = "3: Action Groups";
char menu1_4[15] = "4: Attitude";
char* menu1[4];

//Menu 1a - Engines
char menu1a_1[15] = "Enter engine #";
char menu1a_2[1] = "";
char menu1a_3[1] = "";
char menu1a_4[1] = "";
char* menu1a[4];

//Menu 1b - Decouplers
char menu1b_1[18] = "Enter decoupler #";
char menu1b_2[1] = "";
char menu1b_3[1] = "";
char menu1b_4[1] = "";
char* menu1b[4];

//Menu 1c - Action Groups
char menu1c_1[21] = "Enter action group #";
char menu1c_2[1] = "";
char menu1c_3[1] = "";
char menu1c_4[1] = "";
char* menu1c[4];

//Menu 1d - Attitude
char menu1d_1[12] = "1: Prograde";
char menu1d_2[14] = "2: Retrograde";
char menu1d_3[10] = "3: Custom";
char menu1d_4[7] = "4: Off";
char* menu1d[4];

//Menu 2 - Scripts
char menu2_1[20] = "";
char menu2_2[20] = "";
char menu2_3[20] = "";
char menu2_4[20] = "";
char* menu2[4];

//Menu 3 - Settings
char menu3_1[17] = "1: Telemachus IP";
char menu3_2[18] = "2: Multiplayer IP";
char menu3_3[21] = "3: Start Multiplayer";
char menu3_4[15] = "4: Wifi Preset";
char* menu3[4];

//General I/O pins
int b1 = 30;
int b2 = 31;
int b3 = 32;
int b4 = 33;
int b5 = 34;

int led1 = 40;
int led2 = 41;
int led3 = 42;
int led4 = 43;
int led5 = 44;

int abort_pin = 35;

int release_pin = 36;

int map_pin = 37;
int map_last = 0;

int buzzer = 2;
int beep1 = 510;
int beep1len = 100;

int throttle_pin = 0;
float throttle = 0;
float last_throttle = 0;



//Write menus
void write_menu(char* input_menu[], int beep) {
  lcd.clear();
  for (int index = 0; index < 4; index++) {
    lcd.setCursor(0,index);
    lcd.print(input_menu[index]);
  }
  if (beep == 1) {
    tone(buzzer,beep1,beep1len);
  }
}

//Double tone acknowledge
void ak_tone() {
  tone(buzzer,600,100);
  delay(200);
  tone(buzzer,600,100);
  delay(200);
}

//Return user inputted number from keypad
char* input(char prompt[], char seperator, int hidden = 0, int beep = 1) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(prompt);
  char output[21];
  int count = 0;
  while (1) {
    result = keypad.getKey();
    if (result != NO_KEY && result != '#') {
      if (result == '*') {
        if (seperator != '0') {
          result = seperator;
        }  
      }
      tone(buzzer,beep1,beep1len);
      lcd.setCursor(count,2);
      if (hidden == 0) {
        lcd.print(result);
      } else {
        lcd.print('*');
      }
      output[count] = result;
      count += 1;
    } else if (result == '#') {
      break;
    }
  }
  if (beep == 1) {
    ak_tone();
  }
  output[count] = '\0';
  return output;
}


//Check everything on panel and send changes via Serial
void check_panel() {
    int change = 0; 
    if (digitalRead(b1) == 1) {
      while (digitalRead(b1) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial.print("[C1]");
      
    } else if (digitalRead(b2) == 1) {
      while (digitalRead(b2) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial.print("[C2]");
      
    } else if (digitalRead(b3) == 1) {
      while (digitalRead(b3) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial.print("[C3]");
      
    } else if (digitalRead(b4) == 1) {
      while (digitalRead(b4) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial.print("[C4]");
      
    } else if (digitalRead(b5) == 1) {
      while (digitalRead(b5) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial.print("[C5]");
      
    } else if (digitalRead(release_pin) == 1) {
      while (digitalRead(release_pin) == 1) {
        delay(0.1);  
      }
      Serial.print("[C6]");
      
    } else if (digitalRead(abort_pin) == 1) {
      while (digitalRead(abort_pin) == 1) {
        delay(0.1);
      }
      Serial.print("[C7]");
      
    } else if (digitalRead(map_pin) == 1 && map_last == 0) {
      Serial.print("[C8]");
      map_last = 1;
      
    } else if (digitalRead(map_pin) == 0 && map_last == 1) {
      Serial.print("[C9]");
      map_last = 0;  
    }
    
    throttle = map(analogRead(0), 0, 1023, 0, 100);
    throttle = throttle * -1;
    throttle = throttle + 100;
    /*
    if (throttle < 5) {
        throttle = 0.0;
      } else if (throttle > 95) {
        throttle = 100.0;  
      }
    */
    if (throttle != last_throttle && throttle != last_throttle - 1 && throttle != last_throttle + 1) {
      last_throttle = throttle;
      Serial.print("[T");
      //Serial.print(((float)((int)(throttle * 1000))) / 1000);
      Serial.print(throttle/100);
      Serial.print("]");
    }
    if (change == 1) {
      update_leds();  
    }
}

//Update LEDs above control buttons with data from Telemachus
void update_leds() {
  Serial.print("[R2]");
  while (1) {
    //Wait for 5 bytes
    if (Serial.available() >= 5) {
      //process data
      char output[5];
      Serial.readBytes(output,5);
      digitalWrite(led1, output[0]);
      digitalWrite(led2, output[1]);
      digitalWrite(led3, output[2]);
      digitalWrite(led4, output[3]);
      digitalWrite(led5, output[4]);
      break;
    }
    delay(50);
  } 
}






void setup() {
  Serial.begin(9600);
  Serial.begin(9600);
  
  //LCD setup
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  lcd.begin(20,4);
  lcd.clear();
  
  //More menu setup
  menu0[0] = menu0_1;
  menu0[1] = menu0_2;
  menu0[2] = menu0_3;
  menu0[3] = menu0_4;
  
  menu1[0] = menu1_1;
  menu1[1] = menu1_2;
  menu1[2] = menu1_3;
  menu1[3] = menu1_4;
  
  menu1a[0] = menu1a_1;
  menu1a[1] = menu1a_2;
  menu1a[2] = menu1a_3;
  menu1a[3] = menu1a_4;
  
  menu1b[0] = menu1b_1;
  menu1b[1] = menu1b_2;
  menu1b[2] = menu1b_3;
  menu1b[3] = menu1b_4;
  
  menu1c[0] = menu1c_1;
  menu1c[1] = menu1c_2;
  menu1c[2] = menu1c_3;
  menu1c[3] = menu1c_4;
  
  menu1d[0] = menu1d_1;
  menu1d[1] = menu1d_2;
  menu1d[2] = menu1d_3;
  menu1d[3] = menu1d_4;
  
  menu3[0] = menu3_1;
  menu3[1] = menu3_2;
  menu3[2] = menu3_3;
  menu3[3] = menu3_4;
  
  
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to the");
  lcd.setCursor(0,1);
  lcd.print("Kerbal Space Program");
  lcd.setCursor(0,2);
  lcd.print("control panel.");
  
  delay(4000);
  //Enter code to get access to board
  while (1) {
    char *output = input("Please enter code",'0',1,0);
 
    
    if (output[0] == '1' && output[1] == '2' && output[2] == '3' && output[3] == '4') {
      break;
    } else {
      lcd.clear();
      lcd.setCursor(0,2);
      lcd.print("Code incorrect");
      tone(buzzer,400,100);
      delay(200);
      tone(buzzer,400,100);
      delay(2000); 
    }
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Code correct");
  ak_tone();
  delay(1000);
  lcd.setCursor(0,2);
  lcd.print("Welcome");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Waiting for RasPi");
  lcd.setCursor(0,1);
  lcd.print("      to boot      ");
  int count = 0;
  int loadcount = 0;
  while (1) {
    if (Serial.available() > 0 || 1 == 1) {
      break;  
    }
    
    
    if (count == 10) {
      count = 0;
      if (loadcount == 5) {
        loadcount = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Waiting for RasPi");
        lcd.setCursor(0,1);
        lcd.print("      to boot      ");  
      } else {
        lcd.setCursor(5+(loadcount*2),3);
        lcd.print("*");
        loadcount += 1;
      }
      
    }
    count += 1;
    delay(50);  
  }
  
  
}



void loop() {
  //Main menu
  write_menu(menu0, 0);
  while (1) {
    result = keypad.getKey();
    if (result == '1') {
      //Manual control
      write_menu(menu1, 1);
      while (1) {
        check_panel();
        result = keypad.getKey();
        if (result == '*') {
          write_menu(menu0, 1);
          break;
        } else if (result == '1') {
          //Engines
          write_menu(menu1a,1);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1,1);
              break;    
            } else if (result != NO_KEY and result != '#') {
              tone(buzzer,beep1,beep1len);
              lcd.setCursor(10,3);
              lcd.print(result);
              int b = 0;
              while (1) {
                check_panel();
                char result2 = keypad.getKey();
                if (result2 == '*') {
                  b = 1;
                  break;
                } else if (result2 == '#') {
                  ak_tone();
                  break;  
                }
                delay(50);
              }
              if (b == 0) {
                int num = result - '0';
                int ag = num + 10;
                Serial.print("[C");
                Serial.print(ag);
                Serial.print("]");
              }
              write_menu(menu1,0);
              break; 
            }
          }
        } else if (result == '2') {
          //Decouplers
          write_menu(menu1b,1);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1,1);
              break;
            } else if (result != NO_KEY and result != '#') {
              tone(buzzer,beep1,beep1len);
              lcd.setCursor(10,3);
              lcd.print(result);
              int b = 0;
              while (1) {
                check_panel();
                char result2 = keypad.getKey();
                if (result2 == '*') {
                  b = 1;
                  break;  
                } else if (result2 == '#') {
                  ak_tone();
                  break;  
                }
                delay(50);
              }
              if (b == 0) {
                int num = result - '0';
                int ag = num + 20;
                Serial.print("[C");
                Serial.print(ag);
                Serial.print("]");
              }
              write_menu(menu1,0);
              break;
            }
          }
        } else if (result == '3') {
          //Action Groups
          write_menu(menu1c,1);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1,1);
              break;    
            } else if (result != NO_KEY and result != '#') {
              tone(buzzer,beep1,beep1len);
              lcd.setCursor(10,3);
              lcd.print(result);
              int b = 0;
              while (1) {
                check_panel();
                char result2 = keypad.getKey();
                if (result2 == '*') {
                  b = 1;
                  break;  
                } else if (result2 == '#') {
                  ak_tone();
                  break;  
                }
                delay(50);
              }
              if (b == 0) {
                int num = result - '0';
                Serial.print("[C");
                Serial.print(num);
                Serial.print("]");
              }
              write_menu(menu1,0);
              break; 
            }
          }    
        } else if (result == '4') {
          //Attutude
          write_menu(menu1d,1);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1,1);
              break;    
            } else if (result == '1') {
              //Pro
              Serial.print("[A1]");
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Prograde set");
              ak_tone();
              delay(1000);
              write_menu(menu1,0);
              break;
            } else if (result == '2') {
              //Retro
              Serial.print("[A2]");
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Retrograde set");
              ak_tone();
              delay(1000);
              write_menu(menu1,0);
              break;
            } else if (result == '3') {
              tone(buzzer,beep1,beep1len);
              char* output = input("Heading, Pitch, Roll",',');
              int outsize = strlen(output);
              
              Serial.print('[');
              Serial.print('A');
              Serial.print('3');
              for (int x = 0; x < outsize; x++) {
                Serial.write(output[x]);
              }
              Serial.print(']');
              write_menu(menu1,0);
              break;  
            } else if (result == '4') {
              //Off
              Serial.print("[A4]");
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Attitude control off");
              ak_tone();
              delay(1000);
              write_menu(menu1,0);
              break;
            }
          }    
        }
      }
    } else if (result == '2') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Loading scripts...");
      Serial.print("[R2]");
      while (1) {
        if (Serial.available() > 0) {
          //Wait until end character.  
        }
         
      }
      //Split data into list of script names
      
      
      //Have mini menu program with pages if #ofscripts > 4
      
      
      
    } else if (result == '3') {
      write_menu(menu3,1);
      while (1) {
        
        result = keypad.getKey();
        if (result == '*') {
          write_menu(menu0, 1);
          break;  
        } else if (result == '1') {
          //Type in telemachus server
          tone(buzzer,beep1,beep1len);
          char *output = input("Enter server IP:", '.');
          int outsize = strlen(output);
          
          Serial.print('[');
          Serial.print('I');
          Serial.print('1');
          for (int x = 0; x < outsize; x++) {
            Serial.print(output[x]);
          }
          Serial.print(']');
          write_menu(menu3,0);
          
        } else if (result == '2') {
          //Multiplayer server
          tone(buzzer,beep1,beep1len);
          char *output = input("Enter server IP:", '.');
          int outsize = strlen(output);
          
          Serial.print('[');
          Serial.print('I');
          Serial.print('2');
          for (int x = 0; x < outsize; x++) {
            Serial.print(output[x]);
          }
          Serial.print(']');
          write_menu(menu3,0);
          
        } else if (result == '3') {
          //Enable Multiplayer
          
        } else if (result == '4') {
          //Wifi preset
          
        }
        
      } 
    } else if (result == '4') {
      Serial.print("[H]");
      
      for (int x = 0; x < 20; x++) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Please wait ");
        lcd.print(20 - x);
        lcd.setCursor(0,1);
        lcd.print("seconds until Raspi");
        lcd.setCursor(0,2);
        lcd.print("fully shuts down");
        delay(1000);
        
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("It is now safe to");
      lcd.setCursor(0,1);
      lcd.print("turn off power.");
      exit(0);
      
    }
  } 
}
