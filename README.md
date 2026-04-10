The project works on development of a lightweight kernel and explores the different algorithm for a scheduler : round robin, periodic and co-operative scheduling. Other tasks include : development of low level drivers for UART, timebase generation and GPIO.

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
