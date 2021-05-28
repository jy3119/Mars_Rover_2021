# CONTROL-VISION CONNECTION

## I2C communication Protocol

##### Features:
- On-Chip Memory(RAM or ROM) Intel FPGA IP with dual port access 
- I2C Slave to Avalon-MM Master Brdige Intel FPGA IP, byte addressing mode 4
- i2c_adaptor.v

###### How to augment on top of Vision's existing quartus files:
TBC

###### How to use (Test version only): (might do a video walkthrough for clarity)
1. Create new Quartus project, name the project as whatever _projectname_ you want, or esp32_0 if you'd like
2. Open nios2_cpu.qsys in Platform Designer(Tools --> Platform Designer ) and generate HDL (Generate --> Generate HDL)
3. esp32_0.v contains the verilog code for the top-level module. Rename the verilog file to _projectname.v_ and module name as needed for your top-level verilog file. No renaming is required if your project is called esp32_0.
5. Add the top-level verilog file, i2c_adaptor.v, pin_assignment.qsf nios2_cpu.qip file to project. (Project --> Add/Remove Files in Project... --> Add the needed files, then -->  Apply --> OK)
6. Compile (the play button beside the STOP sign) to get .sof file
7. Connect your FPGA to your laptop via the cable and blast the .sof file using Programmer (Tools --> programmer )
8. Open eclipse editor
9. Create new Nios II Application and BSP from template, use Hello World Small and choose the needed .sopcinfo (should be nios2_cpu.sopcinfo)
10. Copy paste the C code found in i2c_fpga_test.c into hello_world_small.c
11. Right click your folder, Run as NIOS II Hardware to program the Nios II CPU