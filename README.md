This project implements a lightweight embedded kernel on ARM Cortex-M microcontrollers in bare-metal C, focusing on real-time task scheduling and low-level system control.

The kernel explores and implements multiple scheduling strategies, including:
Round-robin scheduling
Cooperative scheduling
Periodic (time-triggered) execution model

Core kernel features include manual context switching and execution control at register level, with explicit saving and restoring of CPU state (R0–R12, LR, PC, and xPSR) to enable deterministic task switching.

In addition, the project includes development of essential low-level drivers and system services:
UART driver for serial communication and debugging
SysTick-based timebase generation for scheduling and delays
GPIO control for basic hardware interaction

The implementation emphasizes real-time behavior, interrupt-driven execution, and understanding of MCU internal architecture, including stack usage, exception handling, and register-level execution flow.

Hardware:
Board: STM32F446RE Nucleo
Peripherals Used: User LED (LD2) for visual verification

Software Toolchain:
IDE/Framework: STM32CubeIDE
Terminal Output: RealTerm used for UART-based monitoring

Project Highlights:
Implementation of SysTick-based timebase generation
Verification of timing using RealTerm terminal output
Hardware validation through LED LD2 toggling
Development of RTOS tasks and scheduling from scratch
