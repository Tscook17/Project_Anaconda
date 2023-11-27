#include "intervalTimer.h"
#include "xil_io.h"
#include "xparameters.h"
#include "leds.h"
#include <stdbool.h>
#include <stdint.h>

// timers
#define TMR0 0
#define TMR1 1
#define TMR2 2

// timer registers offsets
#define TCSR0 0x00
#define TLR0 0x04
#define TCR0 0x08
#define TCSR1 0x10
#define TLR1 0x14
#define TCR1 0x18

// TCSR bit locations
#define CASC 11 // TCSR0 specific
#define C_UP_DOWN 1
#define LOAD 5
#define EN_CTR 7
#define AUTO_RLD 4
#define ENIT 6
#define T0INT 8

#define ALL_OFF 0x00000000
#define ALL_ON 0xFFFFFFFF
#define LONG_1 0x00000001
#define SHIFT_32 32

static uint32_t setBit(uint32_t reg, uint8_t bitPlace, bool value);
static uint32_t readRegister(uint8_t timerNumber, uint32_t offset);
static void writeRegister(uint8_t timerNumber, uint32_t offset, uint32_t value);

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts up
// 2. Initialize both LOAD registers with zeros
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountUp(uint32_t timerNumber) {
  uint32_t tempReg = ALL_OFF;

  // set TCSRs to 0
  writeRegister(timerNumber, TCSR0, ALL_OFF);
  writeRegister(timerNumber, TCSR1, ALL_OFF);

  // set cascade mode on
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, CASC, true));

  // set count up
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, C_UP_DOWN, false));

  // set both LOAD regs with 0
  writeRegister(timerNumber, TLR0, ALL_OFF);
  writeRegister(timerNumber, TLR1, ALL_OFF);

  // Load 0 values into counters
  intervalTimer_reload(timerNumber);
}

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts down
//  - The timer automatically reloads when reaching zero
// 2. Initialize LOAD registers with appropriate values, given the `period`.
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountDown(uint32_t timerNumber, double period) {
  uint32_t tempReg = ALL_OFF;
  uint32_t lower_value = (uint32_t)(XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ * period);
  uint32_t upper_value =
      (uint32_t)((uint64_t)(XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ * period) >>
                 SHIFT_32);

  // set TCSRs to 0
  writeRegister(timerNumber, TCSR0, ALL_OFF);
  writeRegister(timerNumber, TCSR1, ALL_OFF);

  // set cascade mode on
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, CASC, true));

  // set count down
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, C_UP_DOWN, true));

  // set automatic reload when reach 0
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, AUTO_RLD, true));

  // set both LOAD regs with start value
  writeRegister(timerNumber, TLR0, lower_value);
  writeRegister(timerNumber, TLR1, upper_value);

  // Load start count values into counters
  intervalTimer_reload(timerNumber);
}

// This function starts the interval timer running.
// If the interval timer is already running, this function does nothing.
// timerNumber indicates which timer should start running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_start(uint32_t timerNumber) {
  uint32_t tempReg = ALL_OFF;
  // set enable timer
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, EN_CTR, true));
}

// This function stops a running interval timer.
// If the interval time is currently stopped, this function does nothing.
// timerNumber indicates which timer should stop running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_stop(uint32_t timerNumber) {
  uint32_t tempReg = ALL_OFF;
  // set enable timer
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, EN_CTR, false));
}

// This function is called whenever you want to reload the Counter values
// from the load registers.  For a count-up timer, this will reset the
// timer to zero.  For a count-down timer, this will reset the timer to
// its initial count-down value.  The load registers should have already
// been set in the appropriate `init` function, so there is no need to set
// them here.  You just need to enable the load (and remember to disable it
// immediately after otherwise you will be loading indefinitely).
void intervalTimer_reload(uint32_t timerNumber) {
  uint32_t tempReg = ALL_OFF;

  // set TCSR0 load bit and then sets back to 0
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, LOAD, true));
  writeRegister(timerNumber, TCSR0, setBit(tempReg, LOAD, false));

  // set TCSR1 load bit and then sets back to 0
  tempReg = readRegister(timerNumber, TCSR1);
  writeRegister(timerNumber, TCSR1, setBit(tempReg, LOAD, true));
  writeRegister(timerNumber, TCSR1, setBit(tempReg, LOAD, false));
}

// Use this function to ascertain how long a given timer has been running.
// Note that it should not be an error to call this function on a running timer
// though it usually makes more sense to call this after intervalTimer_stop()
// has been called. The timerNumber argument determines which timer is read.
// In cascade mode you will need to read the upper and lower 32-bit registers,
// concatenate them into a 64-bit counter value, and then perform the conversion
// to a double seconds value.
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  uint64_t upperReg = ALL_OFF;
  uint64_t lowerReg = ALL_OFF;
  double fullReg = ALL_OFF;

  upperReg = (uint64_t)readRegister(timerNumber, TCR1);
  lowerReg = (uint64_t)readRegister(timerNumber, TCR0);
  fullReg = (double)((upperReg << SHIFT_32) | lowerReg);
  return fullReg / XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
}

// Enable the interrupt output of the given timer.
void intervalTimer_enableInterrupt(uint8_t timerNumber) {
  uint32_t tempReg = ALL_OFF;

  // set TCSR0 enable interupt bit to 1
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, ENIT, true));
}

// Disable the interrupt output of the given timer.
void intervalTimer_disableInterrupt(uint8_t timerNumber) {
  uint32_t tempReg = ALL_OFF;

  // set TCSR0 enable interupt bit to 0
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, ENIT, false));
}

// Acknowledge the rollover to clear the interrupt output.
void intervalTimer_ackInterrupt(uint8_t timerNumber) {
  uint32_t tempReg = ALL_OFF;

  // set TCSR0 enable interupt bit to 0
  tempReg = readRegister(timerNumber, TCSR0);
  writeRegister(timerNumber, TCSR0, setBit(tempReg, T0INT, true));
}

// changes the bit at the indicated placement to either a 1 or 0
static uint32_t setBit(uint32_t reg, uint8_t bitPlace, bool value) {
  // if value = 1 turns it to 1 if value = 0 changes bit to 0
  if (value) {
    // sets bit to 1
    reg = reg | (1 << bitPlace);
    return reg;
  } else {
    // sets bit to 0
    reg = reg & ~(1 << bitPlace);
    return reg;
  }
}

// returns register value at given address
static uint32_t readRegister(uint8_t timerNumber, uint32_t offset) {
  // returns register value based on the timer specified
  switch (timerNumber) {
  case 0:
    return Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + offset);
  case 1:
    return Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + offset);
  case 2:
    return Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + offset);
  }
}

// writes to the given register with the given value
static void writeRegister(uint8_t timerNumber, uint32_t offset,
                          uint32_t value) {
  // writes to the register based on the timer specified
  switch (timerNumber) {
  case 0:
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + offset, value);
  case 1:
    Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + offset, value);
  case 2:
    Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + offset, value);
  }
}
