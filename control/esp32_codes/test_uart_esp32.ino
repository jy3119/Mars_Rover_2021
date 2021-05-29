/* ESP32 UART Test 
 *  ESP32's serial monitor should show "Hello from Arduino Nano Every!"
 *  There could be a possibility that it might show "Hello from ESP32!" though lol
*/

#define RXD2 16 // look for port 9 on the arduino adaptor of the ESP32, connect this to TX on the arduino
#define TXD2 17 // look for port 8 on the arduino adaptor of the ESP32, connect this to RX on the arduino

void setup() {
  // initialize UART with baud rate of 9600 bps
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);    // Serial2 is used for communication with arduino   
  Serial.begin(9600);                             // Serial is used for communication with serial monitor on the laptop
}

void loop() {
  if (Serial2.available()) {
    // read string sent from arduino's TX to ESP32's Serial2 RX and store into string data_rcvd
    String data_rcvd = Serial2.readString();  

    // print data_rcvd onto ESP32's serial monitor
    Serial.println(data_rcvd);
  }

  // send out "Hello from ESP32!" to Arduino's RX port via UART/Serial
  Serial2.write("Hello from ESP32!");
}
