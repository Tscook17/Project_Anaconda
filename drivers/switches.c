#include "xil_io.h"
#include "xparameters.h"
#include <stdint.h>
#include "switches.h"

#define GPIO_DATA 0x00
#define GPIO_TRI 0x04
#define TRI_INPUT 0x0f

static uint32_t readRegister(uint32_t offset);
static void writeRegister(uint32_t offset, uint32_t value);

// Initializes the SWITCHES driver software and hardware.
void switches_init() { writeRegister(GPIO_TRI, TRI_INPUT); }

// Returns the current value of all 4 switches as the lower 4 bits of the
// returned value. bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
uint8_t switches_read() { return readRegister(GPIO_DATA); }

// returns register value at given address
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + offset);
}

// writes to the given register with the given value
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
}