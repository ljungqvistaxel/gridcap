
# Software

Naming stuff is difficult, so here is a quick overview:

## calculations/

Includes the python script used to generate graphs from the collected data. The collected data is also included here, in the form of text files in the format "[plate thickness]\_[pressure point]\_[applied force].txt". The data is stored as two rows, where the first row is the 64 individual pad capacitance readings in one snapshot, and the second row is the tare offset value for the same pads.

## loadcell/

This has an arduino IDE project to read and interpret the force applied by the test rig loadcell. The readings are sent from the microcontroller (ESP32 in our case) to the PC via UART over USB.

## stm32/

Includes the STM32 Cube IDE project for the STM32 microcontroller to measure the capacitances (time) in the sensor module and send it to the PC via UART over USB.

## uart_host/

This includes the program used for recieving and calculating the capacitances sent from the STM32 microcontroller, and shows a live view in the terminal. This program can tare, save, load and show the realtime capacitance values in its terminal interface. It even has color to help interpreting the data. Revolutionary! ;)
