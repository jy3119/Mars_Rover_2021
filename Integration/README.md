In order to achieve the desired requirements, the following steps need to be done:

For vision:	
1. Platform designer -> load QSYS and generate HDL
2. Afterwards, close it and go to Project -> Add/remove files -> remove the existing QIP file and replace it with the newly synthesised one (inside Qsys/synthesis folder) make sure you DONT include both .qsys and .qip inside the add/remove files
3. Compile
4. Programmer -> blast it (DONT close the pop-up window after it blasts successfully)
5. (if no updates of main.c):
go to NIOS II command shell, cd to path so that you are now in the software/D8M_Camera_Test folder
6. run "nios2-download D8M_Camera_Test.elf -c 1 -g"
5.(if updates):
NIOS II...wtv ECllipse and open eclipse set it to ..../software folder
6. right click on project_name.bsp -> go to Nios II option -> click generate BSP
7. after that, build the project (press Ctrl + B)
8. get the camera and all ready, and you run the Eclipse

For command:
start the command subsystem by following the instructions in the "command" folder 

For drive:
If uploading not success, plug arduino out of the board and upload.
When insert it back, first remove the power cable and it's better to check if pins fit before it's connected.
Switch the physical switch to on and the main loop starts to function.

For control:
Refresh by pressing black button

For connecting the ESP32 to Nano Every, wiring ESP32 and Nano Every.jpg can be refered.
For connecting the ESP32 to FPGA, just insert it on the top so that the digital pins for communication are already set to matched
For the rover to connect to the Command, a list of operation is included in the readme.md under the Command repository. AFter setting the WIFI name and password in the ControlESP32.ino under Control repository, ESP32 should connect to WIFI and to the Command app by itself.

After finish all the settings, the rover should be able to accept instructions from command and start to react.
