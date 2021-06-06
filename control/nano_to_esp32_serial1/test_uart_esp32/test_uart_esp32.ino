#define TXD2 17 // look for port 8 on the arduino adaptor of the ESP32, connect this to RX on the arduino
#define RXD2 16 // look for port 9 on the arduino adaptor of the ESP32, connect this to TX on the arduino
#define TXD1 1
#define RXD1 3

void setup(){
  Serial.begin(115200);
  // Serial2 ports connected to Arduino Nano UART
  Serial2.begin(9600,SERIAL_8N1,RXD2,TXD2);
}

void loop(){
  if(Serial2.available()){
    String data_rcvd = Serial2.readString();
    Serial.println(data_rcvd);
  }
  Serial2.print("hello from esp32");
  delay(500);
}
