#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DIO 1
#define STB 2
#define CLK 3

#define READ_MODE 0x02
#define WRITE_MODE 0x00
#define INCR_ADDR 0x00
#define FIXED_ADDR 0x04

typedef struct node {
    uint8_t val;
    struct node *next;
    struct node *prev;
} node_t;

void push(node_t*, uint8_t);
uint8_t pop(node_t*);
void setPin(uint8_t, uint8_t);
void pinInit();
void initAll(uint8_t);
void sendByte(uint8_t data);
void setMode(uint8_t wr_mode, uint8_t addr_mode);
void sendCmd(uint8_t cmd);
void sendData(uint8_t addr, uint8_t data);
void turnOn(uint8_t brightness);
void setLED(uint8_t idx, uint8_t value);
void setLEDs(uint8_t num);
void clearLEDs();
void clearDisplay();
void writeChar(uint8_t idx, uint8_t value);
void writeStr(uint8_t idx, char* str);

#endif /* DISPLAY_H_ */
