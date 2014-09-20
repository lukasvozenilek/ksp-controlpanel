#include <LiquidCrystal.h>
#include <Keypad.h>


//Arduino pins to LCD pins
//52-15, 53-4, 51-5, 49-6, 47-7, 45-8, 43-9, 41-*
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

//Menu 1d3 - Custom
char menu1d3_1[21] = "Heading, Pitch, Roll";
char menu1d3_2[1] = "";
char menu1d3_3[1] = "";
char menu1d3_4[1] = "";
char* menu1d3[4];

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
char menu3_4[15] = "4: ";
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
int led6 = 45;

int led1_tog = 0;
int led2_tog = 0;
int led3_tog = 0;
int led4_tog = 0;
int led5_tog = 0;
int led6_tog = 0;

int abort_pin = 35;

int release_pin = 36;

int map_pin = 37;
int map_last = 0;

int buzzer = 2;

int throttle_pin = 0;
float throttle = 0;
float last_throttle = 0;



//Code stuff
char code1 = '1';
char code2 = '2';
char code3 = '3';
char code4 = '4';
char enter[4];
int e_count = 0;


void write_menu(char* input_menu[]) {
  lcd.clear();
  for (int index = 0; index < 4; index++) {
    lcd.setCursor(0,index);
    //Serial.println(input_menu[index]);
    lcd.print(input_menu[index]);  
  }
  Serial.print("\n");
}



boolean float_compare(float f1, float f2){
  return ( (int)(f1 *100)) == ((int)(f2 * 100) );
}

//Check everything on panel and send changes via Serial1
void check_panel() {
    int change = 0; 
    if (digitalRead(b1) == 1) {
      while (digitalRead(b1) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial1.print("[C1]");
    } else if (digitalRead(b2) == 1) {
      while (digitalRead(b2) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial1.print("[C2]");      
    } else if (digitalRead(b3) == 1) {
      while (digitalRead(b3) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial1.print("[C3]");      
    } else if (digitalRead(b4) == 1) {
      while (digitalRead(b4) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial1.print("[C4]");      
    } else if (digitalRead(b5) == 1) {
      while (digitalRead(b5) == 1) {
        delay(0.1);  
      }
      change = 1;
      Serial1.print("[C5]");      
    } else if (digitalRead(release_pin) == 1) {
      while (digitalRead(release_pin) == 1) {
        delay(0.1);  
      }
      Serial1.print("[C6]");      
    } else if (digitalRead(abort_pin) == 1) {
      while (digitalRead(abort_pin) == 1) {
        delay(0.1);
      }
      Serial1.print("[C7]");      
    } else if (digitalRead(map_pin) == 1 && map_last == 0) {
      Serial1.print("[C8]");
      map_last = 1;
    } else if (digitalRead(map_pin) == 0 && map_last == 1) {
      Serial1.print("[C9]");
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
      Serial1.print("[T");
      //Serial1.print(((float)((int)(throttle * 1000))) / 1000);
      Serial1.print(throttle/100);
      Serial1.print("]");
      Serial.println(throttle);
    }
    if (change == 1) {
      update_leds();  
    }
}

//Update LEDs above control buttons with data from Telemachus
void update_leds() {
  Serial1.print("[R2]");
  while (1) {
    if (Serial1.available() >= 5) {
      //process data
      char output[5] = {};
      Serial1.readBytes(output,5);
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


int charToInt(char ch) {
  if (ch == '1') {
    return 1;  
  } else if (ch == '2') {
    return 2; 
  }  else if (ch == '3') {
    return 3; 
  } else if (ch == '4') {
    return 4; 
  } else if (ch == '5') {
    return 5; 
  } else if (ch == '6') {
    return 6; 
  } else if (ch == '7') {
    return 7; 
  } else if (ch == '8') {
    return 8; 
  } else if (ch == '9') {
    return 9; 
  } else if (ch == '*') {
    return 10; 
  } else if (ch == '#') {
    return 11; 
  }
}


void setup() {
  Serial1.begin(9600);
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
  
  menu1d3[0] = menu1d3_1;
  menu1d3[1] = menu1d3_2;
  menu1d3[2] = menu1d3_3;
  menu1d3[3] = menu1d3_4;
  
  menu3[0] = menu3_1;
  menu3[1] = menu3_2;
  menu3[2] = menu3_3;
  menu3[3] = menu3_4;
  
  
  /*
  //Enter code to get access to board
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please enter code");
  Serial.print("Setup complete, enter code\n");
  while (1) {
    result = keypad.getKey();
    if (result != NO_KEY) {
      enter[e_count] = result;
      lcd.setCursor(e_count,3);
      lcd.print("*");
      e_count += 1;
    }
    if (e_count == 4) {
      if (enter[0] == code1 && enter[1] == code2 && enter[2] == code3 && enter[3] == code4) {
        break;
      } else {
        lcd.clear();
        lcd.setCursor(0,3);
        lcd.print("Code incorrect");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Please enter code");
        e_count = 0;
      }
    }
    delay(50);  
  } 
  
  */
  
}



void loop() {
  write_menu(menu0);
  while (1) {
    
    result = keypad.getKey();
    if (result == '1') {
      write_menu(menu1);
      while (1) {
        check_panel();
        result = keypad.getKey();
        if (result == '*') {
          write_menu(menu0);
          break;
        } else if (result == '1') {
          write_menu(menu1a);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1);
              break;    
            } else if (result != NO_KEY and result != '#') {
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
                  break;  
                }
                delay(50);
              }
              if (b == 0) {
                int num = result - '0';
                int ag = num + 10;
                Serial1.print("[C");
                Serial1.print(ag);
                Serial1.print("]");
              }
              write_menu(menu1);
              break; 
            }
          }
        } else if (result == '2') {
          write_menu(menu1b);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1);
              break;
            } else if (result != NO_KEY and result != '#') {
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
                  break;  
                }
                delay(50);
              }
              if (b == 0) {
                int num = result - '0';
                int ag = num + 20;
                Serial1.print("[C");
                Serial1.print(ag);
                Serial1.print("]");
              }
              write_menu(menu1);
              break;
            }
          }
        } else if (result == '3') {
          write_menu(menu1c);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1);
              break;    
            } else if (result != NO_KEY and result != '#') {
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
                  break;  
                }
                delay(50);
              }
              if (b == 0) {
                int num = result - '0';
                Serial1.print("[C");
                Serial1.print(num);
                Serial1.print("]");
              }
              write_menu(menu1);
              break; 
            }
          }    
        } else if (result == '4') {
          write_menu(menu1d);
          while (1) {
            check_panel();
            result = keypad.getKey();
            if (result == '*') {
              write_menu(menu1);
              break;    
            } else if (result == '1') {
              //Pro
              break;
            } else if (result == '2') {
              //Retro
              break;  
            } else if (result == '3') {
              write_menu(menu1d3);
              while (1) {
                check_panel();
                result = keypad.getKey();
                if (result == '*') {
                  break;    
                } else if (result > 0 && result < 10) {
                  //Custom att      
                }
              }
              break;  
            } else if (result == '4') {
              //Off
            }
          }    
        }
      }
      
    } else if (result == '2') {
      write_menu(menu2);
      while (1) {
        
        result = keypad.getKey(); 
      }
       
    } else if (result == '3') {
      write_menu(menu3);
      while (1) {
        
        result = keypad.getKey();
        if (result == '*') {
          write_menu(menu0);
          break;  
        } else if (result == '1') {
          //Type in telemachus server
        } else if (result == '2') {
          //Type in multiplayer server  
        } else if (result == '3') {
          //Enable Multiplayer 
        } else if (result == '4') {
          //Nothing yet  
        }
        
      } 
    } else if (result == '4') {
      Serial.println("Shutting down...");
      Serial1.print("[H]");
    }
  } 
}
