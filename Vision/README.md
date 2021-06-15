# Vision

Files for Vision Subsystem

# How to use:
The following steps are assuming the necessary connections between the PC, FPGA, and VGA port have been made.

1. Download Quartus and Eclipse
2. First, open the project file (DE10_LITE_D8M_VIP_16.qpf)
3. Open Platform Designer or QSYS, load your desired .qsys file, then Generate HDL.
4. After generating the HDL, add the newly produced .qip file to the project
5. Run full compilation of the project.
6. Blast your FPGA that is connected to your computer via the USB port, with ../output_files/DE10_LITE_D8M_VIP_time_limited.sof file.
7. Go to NIOS II terminal, and change to the ../software/D8M_Camera_Test folder, which contains D8M_Camera_Test.elf file. Then, use the following command to nios2-download it: nios2-download D8M_Camera_Test.elf -c 1 -g
8. Open NIOS II Software Build Tools for Eclipse, and open the ../software directory.  
9. Generate the BSP, Build the project, and run it. Observe the camera screen and the output from the NIOS II terminal.


## Folder Directory:
```bash
Vision/DE10_LITE_D8M_VIP_16
├── output_files                        
│   └── DE10_LITE_D8M_VIP_time_limited.sof              //for blasting FPGA
├── ip                        
│   ├── EEE_IMGPROC.v                                   // demonstration image processor
│   └── rgb_to_hsv.v                                    // module for colour space conversion
├── Qsys
│   ├── Qsys.qsys                                       //qsys file loaded in platform designer
│   └── synthesis
│           └──   Qsys.qip                              //for inclusion in the project          
├── software                         
│   ├── D8M_Camera_Test_bsp                             //BSP is generated in this folder
│   └── D8M_Camera_Test             
│            └── D8M_Camera_Test.elf                    
│            └── main.c
└── DE10_LITE_D8M_VIP_16.qpf                            // PROJECT FILE
 
```

