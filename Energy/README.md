Battery_Charge_Cycle_Logged_V1.1.ino                              // Battery initial test

Battery_Charge_Cycle_Logged_V1.1_CV-CC.ino                        // CC-CV charging test

Battery_Charge_Cycle_Logged_SOC.ino                               // SOC test (coulumb counting + lookup table)

Battery_Charge_Cycle_Logged_V1.1_SoH.ino                          // SOH test (coulumb counting)

PV_characterize.ino                                               // Sweeping duty cycle to characterize PV

MPPT.ino                                                          // perturb and observe (P&O)

-------------------------------------------------------------------------------------------------------------------


The energy subsystem cannot be physically implemented.

A computer was used as an intermediary device to communicate with Command.

The information (SoC, SoH) can be passed using the UART port on the Arduino Nano Every.
