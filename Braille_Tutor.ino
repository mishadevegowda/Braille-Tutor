#include <U8g2lib.h>

#define BUTTON_UP_PIN 13      // Pin for UP button
#define BUTTON_SELECT_PIN 12  // Pin for SELECT button
#define BUTTON_DOWN_PIN 11    // Pin for DOWN button

#define B1 0
#define B2 1
#define B3 2
#define B4 3
#define B5 4
#define B6 5

#define but1 A0
#define but2 A1
#define but3 A2
#define but4 A3
#define but5 6
#define but6 7

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);  // OLED display

const int NUM_ITEMS = 3;  // Number of items in the menu
const int MAX_ITEM_LENGTH = 20;  // Maximum characters for the item name

char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH] = {  // Array with item names
    { "Read" },
    { "Write" },
    { "Learn" }
};

int button_up_clicked = 0;     // Flag to indicate if the up button is clicked
int button_select_clicked = 0; // Flag to indicate if the select button is clicked
int button_down_clicked = 0;   // Flag to indicate if the down button is clicked

int item_selected = 0;  // Currently selected menu item
int current_screen = 0; // 0 = menu

char characters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char braille[][6] = {{1,0,0,0,0,0}, //A
                      {1,0,1,0,0,0}, //B
                      {1,1,0,0,0,0}, //C
                      {1,1,0,1,0,0}, //D
                      {1,0,0,1,0,0}, //E
                      {1,1,1,0,0,0}, //F
                      {1,1,1,1,0,0}, //G
                      {1,0,1,1,0,0}, //H
                      {0,1,1,0,0,0}, //I
                      {0,1,1,1,0,0}, //J
                      {1,0,0,0,1,0}, //K
                      {1,0,1,0,1,0}, //L
                      {1,1,0,0,1,0}, //M
                      {1,1,0,1,1,0}, //N
                      {1,0,0,1,1,0}, //O
                      {1,1,1,0,1,0}, //P
                      {1,1,1,1,1,0}, //Q
                      {1,0,1,1,1,0}, //R
                      {0,1,1,0,1,0}, //S
                      {0,1,1,1,1,0}, //T
                      {1,0,0,0,1,1}, //U
                      {1,0,1,0,1,1}, //V
                      {0,1,1,1,0,1}, //W
                      {1,1,0,0,1,1}, //X
                      {1,1,0,1,1,1}, //Y
                      {1,0,0,1,1,1}, //Z
                      };
int numChars = sizeof(characters) / sizeof(characters[0]);

void setup() {
    u8g2.begin();
     // Set font size
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

    pinMode(B1, OUTPUT);
    pinMode(B2, OUTPUT);
    pinMode(B3, OUTPUT);
    pinMode(B4, OUTPUT);
    pinMode(B5, OUTPUT);
    pinMode(B6, OUTPUT);

    pinMode(but1, INPUT_PULLUP);
    pinMode(but2, INPUT_PULLUP);
    pinMode(but3, INPUT_PULLUP);
    pinMode(but4, INPUT_PULLUP);
    pinMode(but5, INPUT_PULLUP);
    pinMode(but6, INPUT_PULLUP);

}

void loop() {
    if (current_screen == 0) {
        u8g2.setFont(u8g2_font_ncenB10_tr);
        displayMenu();
        handleMenuInput();
    }
}

void displayMenu() {
    u8g2.clearBuffer();
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (i == item_selected) {
            u8g2.setCursor(5, 15 + i * 20); // Set cursor position for selected item
            u8g2.print(">");
        }
        u8g2.setCursor(20, 15 + i * 20); // Set cursor position for menu items
        u8g2.println(menu_items[i]);
    }
    u8g2.sendBuffer();
}

void handleMenuInput() {
    if (digitalRead(BUTTON_UP_PIN) == LOW && !button_up_clicked) {
        item_selected = (item_selected - 1 + NUM_ITEMS) % NUM_ITEMS;
        button_up_clicked = 1;
        delay(200); // Debounce delay
    } else if (digitalRead(BUTTON_DOWN_PIN) == LOW && !button_down_clicked) {
        item_selected = (item_selected + 1) % NUM_ITEMS;
        button_down_clicked = 1;
        delay(200); // Debounce delay
    } else if (digitalRead(BUTTON_SELECT_PIN) == LOW && !button_select_clicked) {
        handleSelection();
        button_select_clicked = 1;
        delay(200); // Debounce delay
    }

    if (digitalRead(BUTTON_UP_PIN) == HIGH) {
        button_up_clicked = 0;
    }
    if (digitalRead(BUTTON_DOWN_PIN) == HIGH) {
        button_down_clicked = 0;
    }
    if (digitalRead(BUTTON_SELECT_PIN) == HIGH) {
        button_select_clicked = 0;
    }
}

void handleSelection() {
    switch (item_selected) {
        case 0:
            displayBrailleCharacter();
            break;
        case 1:
            writeMode();
            break;
        case 2:
            // Perform action for Learn mode
            break;
    }
}

void displayBrailleCharacter() {
    int loopC = 0;
    int currentIndex = 0;
    bool exitLoop = false;

    while (!exitLoop) {
        if (loopC == 2) {
            exitLoop = true;
            digitalWrite(B1, 0);
            digitalWrite(B2, 0);
            digitalWrite(B3, 0);
            digitalWrite(B4, 0);
            digitalWrite(B5, 0);
            digitalWrite(B6, 0);
        } else {
            u8g2.clearBuffer(); // Clear the display buffer
            u8g2.setFont(u8g2_font_ncenB24_tr); // Set the font size
            char str[2] = { characters[currentIndex], '\0' }; // Convert char to C-style string
            u8g2.drawStr(10, 40, str);

            // Draw circles for Braille pattern
            int count = 0;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 2; col++) {
                    if (braille[currentIndex][count] == 1) {
                        u8g2.drawDisc(80 + col * 20, 16 + row * 16, 4); // Draw circles at specified positions
                    }
                    count++;
                }
            }
            digitalWrite(B1, braille[currentIndex][0]);
            digitalWrite(B2, braille[currentIndex][1]);
            digitalWrite(B3, braille[currentIndex][2]);
            digitalWrite(B4, braille[currentIndex][3]);
            digitalWrite(B5, braille[currentIndex][4]);
            digitalWrite(B6, braille[currentIndex][5]);

            // Send the buffer to the OLED display
            u8g2.sendBuffer();

            // Print text at coordinates (x, y)
            while (true) {
                if (digitalRead(BUTTON_UP_PIN) == LOW) {
                    currentIndex = (currentIndex + 1) % numChars;
                    delay(500);
                    break;
                } else if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
                    currentIndex = (currentIndex - 1 + numChars) % numChars;
                    delay(500);
                    break;
                } else if (digitalRead(BUTTON_SELECT_PIN) == LOW) {
                    loopC++;
                    delay(100);
                    break;
                }
            }
        }
    }
}

void writeMode() {
    int loopC = 0;
    int currentIndex = 0;
    bool exitLoop = false;

    while (!exitLoop) {
        if (loopC == 2) {
            exitLoop = true;
            digitalWrite(B1, LOW);
            digitalWrite(B2, LOW);
            digitalWrite(B3, LOW);
            digitalWrite(B4, LOW);
            digitalWrite(B5, LOW);
            digitalWrite(B6, LOW);
        } else {
            u8g2.clearBuffer(); // Clear the display buffer
            u8g2.setFont(u8g2_font_ncenB24_tr); // Set the font size
            char str[2] = { characters[currentIndex], '\0' }; // Convert char to C-style string
            u8g2.drawStr(10, 40, str);

            // Draw circles for Braille pattern
            int count = 0;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 2; col++) {
                    if (braille[currentIndex][count] == 1) {
                        u8g2.drawDisc(80 + col * 20, 16 + row * 16, 4); // Draw circles at specified positions
                    }
                    count++;
                }
            }

            u8g2.sendBuffer();

            while (true) {
                if (digitalRead(BUTTON_UP_PIN) == LOW) {
                    currentIndex = (currentIndex + 1) % numChars;
                    delay(500); // Debounce delay
                    break;
                } else if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
                    currentIndex = (currentIndex - 1 + numChars) % numChars;
                    delay(500); // Debounce delay
                    break;
                } else if (digitalRead(BUTTON_SELECT_PIN) == LOW) {
                    loopC++;
                    delay(500); // Debounce delay
                    break;
                }
                digitalWrite(B1, !digitalRead(but3));
                digitalWrite(B2, !digitalRead(but4));
                digitalWrite(B3, !digitalRead(but2));
                digitalWrite(B4, !digitalRead(but5));
                digitalWrite(B5, !digitalRead(but1));
                digitalWrite(B6, !digitalRead(but6));
              }
        }
    }
}
//LMAO