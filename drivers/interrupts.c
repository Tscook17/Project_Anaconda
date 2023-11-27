#include "interrupts.h"
#include "armInterrupts.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdint.h>

#define ALL_OFF 0x00000000

// register address offsets
#define IPR 0x04
#define IER 0x08
#define IAR 0x0C
#define SIE 0x10
#define CIE 0x14
#define MER 0x1C

#define NUM_INTERRUPTS 3

static void (*isrFcnPtrs[NUM_INTERRUPTS])() = {NULL};

static void interrupts_isr();
static void setBit(uint32_t reg_offset, uint8_t bitPlace, uint8_t value);
static uint32_t readRegister(uint32_t offset);
static void writeRegister(uint32_t offset, uint32_t value);

// Initialize interrupt hardware
// This function should:
// 1. Configure AXI INTC registers to:
//  - Enable interrupt output (see Master Enable Register)
//  - Disable all interrupt input lines.
// 2. Enable the Interrupt system on the ARM processor, and register an ISR
// handler function. This is done by calling:
//  - armInterrupts_init()
//  - armInterrupts_setupIntc(isr_fcn_ptr)
//  - armInterrupts_enable()
void interrupts_init() {
  // Init axi INTC regs
  setBit(MER, 0, 1);
  setBit(MER, 1, 1);
  writeRegister(IER, ALL_OFF);

  // Enable interrupt on ARM processor
  armInterrupts_init();
  armInterrupts_setupIntc(interrupts_isr);
  armInterrupts_enable();
}

// Register a callback function (fcn is a function pointer to this callback
// function) for a given interrupt input number (irq).  When this interrupt
// input is active, fcn will be called.
void interrupts_register(uint8_t irq, void (*fcn)()) { isrFcnPtrs[irq] = fcn; }

// Enable single input interrupt line, given by irq number.
void interrupts_irq_enable(uint8_t irq) { writeRegister(SIE, 1 << irq); }

// Disable single input interrupt line, given by irq number.
void interrupts_irq_disable(uint8_t irq) { writeRegister(CIE, 1 << irq); }

// Tracks and responds to interrupts
static void interrupts_isr() {
  // loop through each interrupt input
  for (uint8_t i = 0; i < NUM_INTERRUPTS; ++i) {
    // check if it has an interrupt pending
    if (readRegister(IPR) & (1 << i)) {
      // check if there is a callback
      if (isrFcnPtrs[i] != NULL) {
        // call the callback function
        isrFcnPtrs[i]();
      }
      // acknowledge interrupt
      writeRegister(IAR, 1 << i);
    }
  }
}

// changes the bit at the indicated placement to either a 1 or 0
static void setBit(uint32_t reg_offset, uint8_t bitPlace, uint8_t value) {
  uint32_t tempReg = readRegister(reg_offset);
  // if value = 1 turns it to 1, if value = 0 changes bit to 0
  if (value) {
    // sets bit to 1
    tempReg = tempReg | (1 << bitPlace);
    writeRegister(reg_offset, tempReg);
  } else {
    // sets bit to 0
    tempReg = tempReg & ~(1 << bitPlace);
    writeRegister(reg_offset, tempReg);
  }
}

// returns register value at given address
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_AXI_INTC_0_BASEADDR + offset);
}

// writes to the given register with the given value
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + offset, value);
}