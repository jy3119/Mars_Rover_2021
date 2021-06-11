#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"

#define mem_limit 1024
alt_16 hori_dist;			        // horizontal distance of obstacle from rover in mm, left pos right neg
alt_8 diag_dist;              // diagonal distance of obstacle from rover in cm
alt_8 detected;    		        // 1 only if obstacle has been detected, 0 otherwise

void updateTmp(alt_32 base_addr, alt_16 hori_dist, alt_8 diag_dist, alt_8 detected){
	alt_32 tmp = (hori_dist<<16) + (diag_dist<<8) + detected;
	IOWR(base_addr, 0, tmp);
}

int main()
{ 
  alt_putstr("Hello from Nios II!\n");

  alt_putstr("Writing into memory...\n");
  updateTmp(I2C_MEM_BASE,    0x001E, 0x05, 0x01);	 // red
  updateTmp(I2C_MEM_BASE+4,  0xFFE2, 0x05, 0x01);  // green
  updateTmp(I2C_MEM_BASE+8,  0x0028, 0x05, 0x01);	 // blue
  updateTmp(I2C_MEM_BASE+12, 0xFFD8, 0x05, 0x01);	 // pink
  updateTmp(I2C_MEM_BASE+16, 0x0019, 0x05, 0x01);	 // yellow
  alt_putstr("Memory write done!\n");


  while (1);

  return 0;
}
