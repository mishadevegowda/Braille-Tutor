const uint32_t refreshDelay = 500; // Change this delay to change time between refreshes

#define OE1 2
#define OE2 3
#define DATA1 4
#define DATA2 5
#define STROBE1 6
#define STROBE2 7
#define CLOCK 8

uint8_t displine[20];

char tempdisprev[20];
char tempdisprevInverted[20];

uint8_t allZeros[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t allOnes[20] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//uint8_t allZeros[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//uint8_t allOnes[20] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//uint8_t allZeros[20] = {0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0};
//uint8_t allOnes[20] = {0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F};

uint32_t count = 0;

void setup() {
  pinMode(OE1, OUTPUT);
  pinMode(OE2, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(STROBE1 , OUTPUT);
  pinMode(STROBE2 , OUTPUT);
  pinMode(CLOCK , OUTPUT);
  digitalWrite(OE1, HIGH);
  digitalWrite(OE2, HIGH);
  digitalWrite(DATA1, LOW);
  digitalWrite(DATA2, LOW);
  digitalWrite(STROBE1 , HIGH);
  digitalWrite(STROBE2 , HIGH);
  digitalWrite(CLOCK , LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(115200);
}

void loop() {
  brailledisplay(allZeros);
  Serial.println("allZeros displayed");
  delay(refreshDelay); 
  brailledisplay(allOnes);
  Serial.println("allOnes displayed");
  delay(refreshDelay);
}

void clearTempBuffer() {
  uint8_t i = 0;
  while (i < 20) {
    tempdisprev[i] = 0x00;
    tempdisprevInverted[i] = 0x00;
    i++;
  }
}

void brailledisplay(uint8_t *disp)
{
  uint8_t bufferSize = sizeof(tempdisprev);
  clearTempBuffer();
  memcpy(tempdisprev, disp, bufferSize);
  uint8_t i = 0, j = 0;
  for (i = 0, j = bufferSize - 1; i < bufferSize / 2; i++, j--)
  {
    uint8_t temp = tempdisprev[i];
    tempdisprev[i] = tempdisprev[j];
    tempdisprev[j] = temp;
  }
  memcpy(tempdisprevInverted, tempdisprev, bufferSize);
  for (i = 0; i < bufferSize; i++) {
    tempdisprevInverted[i] = ~tempdisprev[i];
  }

  digitalWrite(STROBE1, LOW);
  for (i = 0; i < bufferSize; i++) {
    shiftOut(DATA1, CLOCK, LSBFIRST, tempdisprev[i]);
  }
  digitalWrite(STROBE1, HIGH);

  digitalWrite(STROBE2, LOW);
  for (i = 0; i < bufferSize; i++) {
    shiftOut(DATA2, CLOCK, LSBFIRST, tempdisprevInverted[i]);
  }
  digitalWrite(STROBE2, HIGH);
  digitalWrite(OE1, LOW);
  digitalWrite(OE2, LOW);
  delay(35);
  digitalWrite(OE1, HIGH);
  digitalWrite(OE2, HIGH);
  delay(2);
  digitalWrite(OE1, LOW);
  digitalWrite(OE2, LOW);
  delay(10);
  digitalWrite(OE1, HIGH);
  digitalWrite(OE2, HIGH);
  delay(2);
  digitalWrite(OE1, LOW);
  digitalWrite(OE2, LOW);
  delay(7);
  digitalWrite(OE1, HIGH);
  digitalWrite(OE2, HIGH);


  Serial.println("Data sent to braille...");
  Serial.println(++count);
}
