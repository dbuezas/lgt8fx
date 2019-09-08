/*
  Experimental extraction of serial software table
  SETUP:
  1. connect rx to D11 and tx to D10.
  2. connect an external ftdi: rx to d6 and tx to d5

  TEST MODE
  1. run `node SoftwareSerial.js` to act as echo
  2. call `test(bauds, rx_center, rx_intra, rx_stop, tx)` in setup
  3. run and see results in hardware console
  SEARCH MODE
  1. run `node SoftwareSerial.js` this will show you results
  2. call `find()` in the setup funciton
  3. run `node SoftwareSerial.js` in the console to see results
  4. upload code, wait and you'll slowly get the table
  5. TEST your results (test mode), for whatever reason, I need to reduce
  rx_center 1 to 5 units for better conectivity with the pc
*/

#include <SoftwareSerial.h>

SoftwareSerial Serial_sw_to_hw(10, 11);  // RX, TX
SoftwareSerial Serial_sw_to_pc(5, 6);    // RX, TX

#define MAX_LENGTH (_SS_MAX_RX_BUFF - 1)
#define BAUDS 115200

void setup() {
  find();
  /*
  { 115200,  8,  18, 18, 17  },
        { 74880,  18, 31, 31, 31  },
        { 57600,  28, 40, 40, 40  },
        { 38400,  43, 65, 65, 65  },
        { 19200,  67, 139,  139,  136 },
        { 9600, 137,  285,  285,  278 },
        { 4800, 283,  579,  579,  565 },
  { 2400, 554,  1167, 1167, 1135  },
  */

  test(115200, 2, 21, 21, 16);
}

#define table_size 12
unsigned long table[table_size][5] = {
    // {250000, 0, 0, 0, 0},
    {230400, 0, 0, 0, 0}, {115200, 0, 0, 0, 0}, {57600, 0, 0, 0, 0},
    {38400, 0, 0, 0, 0},  {19200, 0, 0, 0, 0},  {9600, 0, 0, 0, 0},
    {4800, 0, 0, 0, 0},   {2400, 0, 0, 0, 0},   {1200, 0, 0, 0, 0},
    {600, 0, 0, 0, 0},    {300, 0, 0, 0, 0},
};

#define i_baud 0
#define i_rx_center 1
#define i_rx_intra 2
#define i_rx_stop 3
#define i_tx 4

void printTable(unsigned long aTable[table_size][5]) {
  for (int i = 0; i < table_size; i++) {
    if (aTable[i][i_baud] == BAUDS && table[i][i_tx]) {
      Serial_sw_to_pc.begin(table[i][i_rx_center], table[i][i_rx_intra],
                            table[i][i_rx_stop], table[i][i_tx]);
      Serial_sw_to_pc.println();
      for (int i = 0; i < table_size; i++) {
        Serial_sw_to_pc.print("\t{ ");
        Serial_sw_to_pc.print(aTable[i][i_baud]);
        Serial_sw_to_pc.print(",\t");
        Serial_sw_to_pc.print(aTable[i][i_rx_center]);
        Serial_sw_to_pc.print(",\t");
        Serial_sw_to_pc.print(aTable[i][i_rx_intra]);
        Serial_sw_to_pc.print(",\t");
        Serial_sw_to_pc.print(aTable[i][i_rx_stop]);
        Serial_sw_to_pc.print(",\t");
        Serial_sw_to_pc.print(aTable[i][i_tx]);
        Serial_sw_to_pc.print("\t},");
        Serial_sw_to_pc.println();
      }
      // Serial_sw_to_pc.end();
      // Serial_sw_to_pc.flush();
    }
  }
}
void find() {
  unsigned long last[5] = {
      0,  // bauds
      1,  // rx_center
      1,  // rx_intra
      1,  // rx_stop
      1,  // tx
  };
  for (int i = 0; i < table_size; i++) {
    Serial.begin(table[i][i_baud]);
    Serial.println("Begin");
    Serial.println();
    table[i][i_tx] = findTX(last[i_tx]);
    table[i][i_rx_intra] = find_rx_intra(table[i][i_tx], last[i_rx_intra]);
    table[i][i_rx_stop] = table[i][i_rx_intra];
    table[i][i_rx_center] =
        find_rx_center(table[i][i_rx_intra], table[i][i_rx_intra],
                       table[i][i_tx], last[i_rx_center]);

    last[i_baud] = table[i][i_baud];
    last[i_rx_center] = table[i][i_rx_center];
    last[i_rx_intra] = table[i][i_rx_intra];
    last[i_rx_stop] = table[i][i_rx_stop];
    last[i_tx] = table[i][i_tx];

    printTable(table);
  }
}

String read(Stream& serial) {
  String str = "";
  while (serial.available()) {
    str += (char)serial.read();
  }
  return str;
}

void write(Stream& serial, unsigned long bauds, uint16_t rxcenter,
           uint16_t rxintra, uint16_t rxstop, uint16_t tx) {
  serial.println();
  serial.print("\t{ ");
  serial.print(bauds);
  serial.print(",\t");
  serial.print(rxcenter);
  serial.print(",\t");
  serial.print(rxintra);
  serial.print(",\t");
  serial.print(rxstop);
  serial.print(",\t");
  serial.print(tx);
  serial.print("\t},");
  serial.println();
}

void printBits(String str) {
  for (uint16_t i = 0; i < str.length(); i++) {
    printBits(str[i]);
    Serial.print(" ");
  }
}
void printBits(byte myByte) {
  for (byte mask = 0x80; mask; mask >>= 1) {
    if (mask & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
}

boolean sendAndCompare(Stream& sender, Stream& receiver,
                       uint16_t bytesPerPacket, uint16_t iterations,
                       bool wait) {
  receiver.flush();
  for (uint16_t i = 0; i < iterations; i++) {
    String str;
    for (uint16_t j = 0; j < bytesPerPacket; j++) {
      str += (char)(60 + j);
    }
    sender.print(str);
    if (wait) {
      delay(200 + bytesPerPacket * 10 / BAUDS);
    }
    String received = read(receiver);

    if (received != str) {
      if (wait) {
        // Serial.println();
        // Serial.print("-> ");
        // printBits(str);
        // Serial.println();
        // Serial.print("<- ");
        // printBits(received);
        // Serial.println();
      }
      return false;
    }
  }

  return true;
}

void loop() {}
uint16_t findTX(uint16_t tx) {
  int increment = tx / 100;
  if (increment < 1) increment = 1;
  Serial.print("----- findTX ------");
  /* is the easiest one, let's find first the best tx by sending a big chuck
   * of data to HW serial */
  uint16_t min = 0;
  uint16_t max = 0;
  while (!min || tx < max + 10 * increment) {
    Serial.print("-");
    Serial.print(tx);
    Serial_sw_to_hw.begin(0, 0, 0, tx);
    if (sendAndCompare(Serial_sw_to_hw, Serial, 1, 1, 0)) {
      if (sendAndCompare(Serial_sw_to_hw, Serial, MAX_LENGTH, 1, 0)) {
        if (!min) {
          min = tx;
          Serial.println();
          Serial.print("tx >= ");
          Serial.println(min);
        }
        max = tx;
      }
    }
    Serial_sw_to_hw.flush();
    tx += increment;
  }
  Serial.println();
  Serial.print("tx =< ");
  Serial.print(max);
  Serial.print("\n");
  tx = (min + max) / 2.0 + 0.5;
  Serial.print("found mid:");
  Serial.println(tx);
  return tx;
}

uint16_t find_rx_intra(uint16_t tx, uint16_t rx_intra) {
  Serial.print("----- find_rx_intra ------");
  int increment = rx_intra / 100;
  if (increment < 1) increment = 1;
  // center is more tolerant, let's guess it as tx/2
  uint16_t rx_center = tx / 2;
  uint16_t min = 0;
  uint16_t max = 0;

  while (!min || rx_intra < max + 10 * increment) {
    Serial_sw_to_hw.begin(rx_center, rx_intra, rx_intra, tx);
    Serial.print("-");
    Serial.print(rx_intra);

    if (sendAndCompare(Serial, Serial_sw_to_hw, 1, 5, 0)) {
      if (sendAndCompare(Serial, Serial_sw_to_hw, MAX_LENGTH, 1, 0)) {
        if (!min) {
          min = rx_intra;
          Serial.println();
          Serial.print("rx_intra >= ");
          Serial.println(min);
        }
        max = rx_intra;
      }
    }
    Serial_sw_to_hw.flush();
    rx_intra += increment;
  }
  Serial.println();
  Serial.print("rx_intra =< ");
  Serial.println(max);
  rx_intra = (min + max) / 2.0 + 0.5;
  Serial.print("FINAL: rx_intra:");
  Serial.println(rx_intra);

  return rx_intra;
}

uint16_t find_rx_center(uint16_t rx_intra, uint16_t rx_stop, uint16_t tx,
                        uint16_t rx_center) {
  Serial.print("----- find_rx_center ------");
  int increment = rx_center / 100;
  if (increment < 1) increment = 1;
  uint16_t min = 0;
  uint16_t max = 0;

  while (!min || rx_center < max + 10 * increment) {
    Serial_sw_to_hw.begin(rx_center, rx_intra, rx_stop, tx);
    Serial.print("-");
    Serial.println(rx_center);

    if (sendAndCompare(Serial, Serial_sw_to_hw, 1, 2, 0)) {
      if (sendAndCompare(Serial, Serial_sw_to_hw, MAX_LENGTH, 1, 0)) {
        Serial.print("-");
        Serial.print(rx_center);

        if (!min) {
          min = rx_center;
          Serial.println();
          Serial.print("rx_center >= ");
          Serial.println(min);
        }
        max = rx_center;
      }
    }
    Serial_sw_to_hw.flush();
    rx_center += increment;
  }
  Serial.println();
  Serial.print("rx_center =< ");
  Serial.println(max);
  rx_center = (min + max) / 2.0 + 0.5;
  Serial.print("FINAL: rx_center:");
  Serial.println(rx_center);
  return rx_center;
}

uint16_t test(unsigned long bauds, uint16_t rx_center, uint16_t rx_intra,
              uint16_t rx_stop, uint16_t tx) {
  Serial.begin(bauds);
  Serial.println("Begin");
  write(Serial, bauds, rx_center, rx_intra, rx_stop, tx);
  int right = 0;
  Serial_sw_to_pc.begin(rx_center, rx_intra, rx_stop, tx);
  for (uint16_t i = 0; i < 10; i++) {
    delay(200);
    Serial_sw_to_pc.flush();
    bool worked =
        sendAndCompare(Serial_sw_to_pc, Serial_sw_to_pc, MAX_LENGTH, 1, true);
    if (worked) right++;
    Serial.print(right);
    Serial.print("/");
    Serial.print(i + 1);
    Serial.print("\t");
    write(Serial, bauds, rx_center, rx_intra, rx_stop, tx);

    Serial_sw_to_pc.print(right);
    Serial_sw_to_pc.print("/");
    Serial_sw_to_pc.print(i + 1);
    Serial_sw_to_pc.print("\t");
    write(Serial_sw_to_pc, bauds, rx_center, rx_intra, rx_stop, tx);
  }
  return -1;
}
