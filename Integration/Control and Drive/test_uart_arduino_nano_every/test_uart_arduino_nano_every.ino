/* Arduino Nano Every UART Test
 * I hope this works - I don't have a Arduino nano Every of my own to test this out... 
 * But since Arduino Nano Every only has one UART port, I'm not sure if its serial monitor will show
 * "Hello from ESP32!" received from ESP32 via UART or 
 * "Hello from Arduino Nano Every!" since we did a Serial.write() ... 
*/


void setup() {
  Serial.begin(38400);
  Serial1.begin(9600);                            
}

void loop() {
  if (Serial1.available()) {
    // read string sent from ESP32's TX to arduino's Serial RX and store into string data_rcvd
    String data_rcvd = Serial1.readString();  

    // print data_rcvd onto Arduino Nano Every's serial monitor
    Serial.println(data_rcvd);
  }

  // send out "Hello from Arduino Nano Every!" to ESP32's RX port via UART/Serial
  Serial1.print("Hello from Arduino Nano Every!");
  delay(500);

}
