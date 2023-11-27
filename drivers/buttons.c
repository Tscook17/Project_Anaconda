#include "xil_io.h"
#include "xparameters.h"
#include <stdint.h>
#include "buttons.h"

#define GPIO_DATA 0x00
#define GPIO_TRI 0x04
#define TRI_INPUT 0x0f

static uint32_t readRegister(uint32_t offset);
static void writeRegister(uint32_t offset, uint32_t value);

// Initializes the button driver software and hardware.
void buttons_init() { writeRegister(GPIO_TRI, TRI_INPUT); }

// Returns the current value of all 4 buttons as the lower 4 bits of the
// returned value. bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
uint8_t buttons_read() { return readRegister(GPIO_DATA); }

// returns register value at given address
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_PUSH_BUTTONS_BASEADDR + offset);
}

// writes to the given register with the given value
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_PUSH_BUTTONS_BASEADDR + offset, value);
}