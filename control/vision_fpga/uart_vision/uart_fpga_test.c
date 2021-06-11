#include "sys/alt_stdio.h"
#include "system.h"
#include "io.h"
#include "stdio.h"
#include "math.h"

#define MSG_BUFFER_SIZE 50
char msgVision[MSG_BUFFER_SIZE];
alt_16 detected0, detected1, detected2, detected3, detected4;
alt_16 hori_dist0, hori_dist1, hori_dist2, hori_dist3, hori_dist4;
alt_16 hprev0, hprev1, hprev2, hprev3, hprev4;
alt_16 diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;
alt_16 dprev0, dprev1, dprev2, dprev3, dprev4;

void sendToControl0(){
    if (detected0 == 0x0001) {
        if( (fabs(hori_dist0-hprev0>40)) || (fabs(diag_dist0-dprev0)>40) ){
            hprev0 = hori_dist0;
            dprev0 = diag_dist0;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<0,%i,%i,%i>", detected0, hori_dist0, diag_dist0);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl1(){
    if(detected1 == 0x0001){
        if( (fabs(hori_dist1-hprev1>40)) || (fabs(diag_dist1-dprev1)>40) ){
            hprev1 = hori_dist1;
            dprev1 = diag_dist1;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<1,%i,%i,%i>", detected1, hori_dist1, diag_dist1);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl2(){
    if(detected2 == 0x0001){
        if( (fabs(hori_dist2-hprev2>40)) || (fabs(diag_dist2-dprev2)>40) ){
            hprev2 = hori_dist2;
            dprev2 = diag_dist2;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<2,%i,%i,%i>", detected2, hori_dist2, diag_dist2);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl3(){
    if(detected3 == 0x0001){
        if( (fabs(hori_dist3-hprev3>40)) || (fabs(diag_dist3-dprev3)>40) ){
            hprev3 = hori_dist3;
            dprev3 = diag_dist3;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<3,%i,%i,%i>", detected3, hori_dist3, diag_dist3);
            alt_putstr(msgVision);
        }
    }
}

void sendToControl4(){
    if(detected4 == 0x0001){
        if( (fabs(hori_dist4-hprev4>40)) || (fabs(diag_dist4-dprev4)>40) ){
            hprev4 = hori_dist4;
            dprev4 = diag_dist4;
            snprintf (msgVision, MSG_BUFFER_SIZE, "<4,%i,%i,%i>", detected4, hori_dist4, diag_dist4);
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
    detected0 = 0x0001;
    hori_dist0 = 0x001E;
    diag_dist0 = 0x0032;

    detected1 = 0x0001;
    hori_dist1 = 0xFFE2;
    diag_dist1 = 0x0032;

    detected2 = 0x0001;
    hori_dist2 = 0x0028;
    diag_dist2 = 0x0032;

    detected3 = 0x0001;
    hori_dist3 = 0xFFD8;
    diag_dist3 = 0x0032;

    detected4 = 0x0001;
    hori_dist4 = 0x0019;
    diag_dist4 = 0x0032;


  while (1) {
      sendToControl();
  }

  return 0;
}


