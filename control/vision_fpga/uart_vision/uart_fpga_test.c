#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"
#include "stdio.h"
#include "math.h"

#define MSG_BUFFER_SIZE 50
char msgVision[MSG_BUFFER_SIZE];
alt_32 detected0, detected1, detected2, detected3, detected4;
alt_32 hori_dist0, hori_dist1, hori_dist2, hori_dist3, hori_dist4;
alt_32 hprev0, hprev1, hprev2, hprev3, hprev4;
alt_32 diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;
alt_32 dprev0, dprev1, dprev2, dprev3, dprev4;

void sendToControl0(){
    if (detected0 == 0x00000001) {
        if( (fabs(hori_dist0-hprev0>0x00000028)) || (fabs(diag_dist0-dprev0)>0x00000028) ){
            hprev0 = hori_dist0;
            dprev0 = diag_dist0;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<0,%li,%li,%li>", detected0, hori_dist0, diag_dist0);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl1(){
    if(detected1 == 0x00000001){
        if( (fabs(hori_dist1-hprev1>0x00000028)) || (fabs(diag_dist1-dprev1)>0x00000028) ){
            hprev1 = hori_dist1;
            dprev1 = diag_dist1;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<1,%li,%li,%li>", detected1, hori_dist1, diag_dist1);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl2(){
    if(detected2 == 0x00000001){
        if( (fabs(hori_dist2-hprev2>0x00000028)) || (fabs(diag_dist2-dprev2)>0x00000028) ){
            hprev2 = hori_dist2;
            dprev2 = diag_dist2;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<2,%li,%li,%li>", detected2, hori_dist2, diag_dist2);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl3(){
    if(detected3 == 0x00000001){
        if( (fabs(hori_dist3-hprev3>0x00000028)) || (fabs(diag_dist3-dprev3)>0x00000028) ){
            hprev3 = hori_dist3;
            dprev3 = diag_dist3;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<3,%li,%li,%li>", detected3, hori_dist3, diag_dist3);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl4(){
    if(detected4 == 0x00000001){
        if( (fabs(hori_dist4-hprev4>0x00000028)) || (fabs(diag_dist4-dprev4)>0x00000028) ){
            hprev4 = hori_dist4;
            dprev4 = diag_dist4;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<4,%li,%li,%li>", detected4, hori_dist4, diag_dist4);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl(){
    sendToControl0();
    sendToControl1();
    sendToControl2();
    sendToControl3();
    sendToControl4();
}

int main()
{ 
    detected0 = 0x00000001;
    hori_dist0 = 0x0000001E;
    diag_dist0 = 0x00000032;

    detected1 = 0x00000000;
    hori_dist1 = 0xFFFFFFE2;
    diag_dist1 = 0x00000032;

    detected2 = 0x00000000;
    hori_dist2 = 0x00000028;
    diag_dist2 = 0x00000032;

    detected3 = 0x00000000;
    hori_dist3 = 0xFFFFFFD8;
    diag_dist3 = 0x00000032;

    detected4 = 0x00000001;
    hori_dist4 = 0x00000019;
    diag_dist4 = 0x00000032;


  while (1) {
      sendToControl();
  }

  return 0;
}


