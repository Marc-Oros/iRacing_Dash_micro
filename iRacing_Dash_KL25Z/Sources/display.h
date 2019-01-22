#ifndef DISPLAY_H_
#define DISPLAY_H_

#define STB 1
#define CLK 2
#define DIO 3

#define READ_MODE 0x02
#define WRITE_MODE 0x00
#define INCR_ADDR 0x00
#define FIXED_ADDR 0x04

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
