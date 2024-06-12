# Robust Positional Force Sensing Using Metal Over Capacitive Touch

This repository contains software, PCB design files, and 3D models used in the research with the title as above, as well as the collected data. The research investigates the use of metal over cap in a 2D matrix to meassure applied force at different parts of the sensor module. The data collected ad used in the research is also included in this repository.

## Used hardware and components

Gridcap module: 
- stm32f401re, charges/discharges pads and reads charging time with a 11.9 ns resolution, and sends the pads charging time over UART
- 20x20 aluminium extrution (any standard length works, as long as its long enough)
- 220 kOhm resistor for pad charging
- 74HC4067PW-Q100J MUX (4x), found at: https://www.mouser.se/ProductDetail/Nexperia/74HC4067PW-Q100J?qs=sPdA%252BpIfi%2F%252BXvXN1KGf9sg%3D%3D
- 0.5mm, 0.7mm or 1.0mm thich aluminium metal sheet (110mm x 110mm)
- SN74HC14N schmitt trigger, found at: https://www.digikey.se/sv/products/detail/texas-instruments/SN74HC14N/277223

Test rig:
- HX711 loadcell amplifier, found at: https://www.electrokit.com/forstarkare-for-lastceller-hx711
- 10kg rated loadcell, found at: https://www.electrokit.com/lastcell-10kg
