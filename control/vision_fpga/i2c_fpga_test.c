#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"

#define mem_limit 1024

void updateTmp(alt_32 base_addr, alt_8 diag_dist, alt_8 hori_dist, alt_8 leftctr_or_right, alt_8 obstacle_detected){
	alt_32 tmp = (diag_dist<<24) + (hori_dist<<16) + (leftctr_or_right<<8) + obstacle_detected;
	IOWR(base_addr, 0, tmp);
}

int main()
{ 
  alt_putstr("Hello from Nios II!\n");

  // EDIT VALUES OF PARAMETERS HERE as rover moves around.
  alt_8 diag_dist = 0x05;			// diagonal distance of obstacle to rover, up to 255cm or 0xff
  alt_8 hori_dist = 0x04;		    // horizontal distance of obstacle to rover, up to 255cm or 0xff
  alt_8 leftctr_or_right = 0x00;    // 1 if obstacle is to the right of rover, 0 otherwise (left or ctr of rover)
  alt_8 obstacle_detected = 0x01;   // 1 only if obstacle has been detected, 0 otherwise

  alt_putstr("Writing into memory...\n");

  // writes into the 1st word of I2C MEM
  updateTmp(I2C_MEM_BASE, diag_dist, hori_dist, leftctr_or_right, obstacle_detected);

  alt_putstr("Memory write done!\n");


  while (1);

  return 0;
}
