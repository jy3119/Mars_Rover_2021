# Control-Vision: UART communication

## How to use (Test Version):
1. Create new Quartus project, name the project as whatever _projectname_ you want
2. Open nios2_cpu.qsys in Platform Designer(Tools --> Platform Designer ) and generate HDL (Generate --> Generate HDL)
3. esp32_uart.v contains the verilog code for the top-level module. Add this to your project and set it as the top level file.
5. Also add pin_assignment.qsf nios2_cpu.qip file to project. (Project --> Add/Remove Files in Project... --> Add the needed files, then -->  Apply --> OK)
6. Compile (the play button beside the STOP sign) to get .sof file
7. Connect your FPGA with the ESP32 already plugged in ontop of the FPGA to your laptop via the cable and blast the .sof file using Programmer (Tools --> programmer )
8. Open eclipse editor
9. Create new Nios II Application and BSP from template, use Hello World Small and choose the needed .sopcinfo (should be nios2_cpu.sopcinfo)
10. Copy paste the C code found in uart_fpga_test.c into hello_world_small.c
11. Right click your folder, click on Build project, and then click on Run, run as NIOS II Hardware to program the Nios II CPU

