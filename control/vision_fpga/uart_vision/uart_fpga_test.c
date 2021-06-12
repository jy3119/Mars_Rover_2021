#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"
#include "stdio.h"


#define MSG_BUFFER_SIZE 32
char msgVision[MSG_BUFFER_SIZE];
alt_8 color;
alt_8 detected0, detected1, detected2, detected3, detected4;
alt_16 hori_dist0, hori_dist1, hori_dist2, hori_dist3, hori_dist4;
alt_16 diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;


void sendToControl(alt_16 color, alt_16 detected, alt_16 hori_dist, alt_16 diag_dist){
    if (detected = 0x0001){
        snprintf (msgVision, MSG_BUFFER_SIZE, "<%i,%i,%i,%i>", color, detected, hori_dist, diag_dist);
        alt_putstr(msgVision);
    }
}

int main()
{ 
    detected0  = 0x0001;
    hori_dist0 = 0x001E;
    diag_dist0 = 0x0032;

    detected1  = 0x0000;
    hori_dist1 = 0xFFE2;
    diag_dist1 = 0x0032;

    detected2  = 0x0000;
    hori_dist2 = 0x0028;
    diag_dist2 = 0x0032;

    detected3  = 0x0000;
    hori_dist3 = 0xFFD8;
    diag_dist3 = 0x0032;

    detected4  = 0x0001;
    hori_dist4 = 0x0019;
    diag_dist4 = 0x0032;


  while (1) {
      sendToControl(0x0000, detected0, hori_dist0, diag_dist0);
      sendToControl(0x0001, detected1, hori_dist1, diag_dist1);
      sendToControl(0x0002, detected2, hori_dist2, diag_dist2);
      sendToControl(0x0003, detected3, hori_dist3, diag_dist3);
      sendToControl(0x0004, detected4, hori_dist4, diag_dist4);
  }

  return 0;
}


