# esp-idf-uart2udp
UART to UDP bridge for ESP-IDF.   

![Image](https://github.com/user-attachments/assets/a81ce56e-623d-4b42-88eb-07e5c5cb055a)

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   


# Installation

```
git clone https://github.com/nopnop2002/esp-idf-uart2udp
cd esp-idf-uart2udp/
idf.py menuconfig
idf.py flash
```

# Configuration
![Image](https://github.com/user-attachments/assets/c72c8e65-54ba-4222-a8fa-0bbfd769b6f4)
![Image](https://github.com/user-attachments/assets/6e4fdd49-a535-4304-a2e9-a347c3f70e1f)

## WiFi Setting
Set the information of your access point.   
![Image](https://github.com/user-attachments/assets/6f00ec25-023f-4d48-b864-69a3261bdd0b)

## UART Setting
Set the information of UART Connection.   
![Image](https://github.com/user-attachments/assets/2903cf03-06d3-449f-a032-21d33652ff4d)

## UDP Setting
Set the information of UDP Connection.   
![Image](https://github.com/user-attachments/assets/10be168a-0604-4177-b0c8-4a7967beaccd)

# How to use   

## Write this sketch on Arduino Uno.   
You can use any AtMega microcontroller.   

```
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Serial.println(command);
  }

  if(lastMillis + 1000 <= millis()){
    Serial.print("Hello World ");
    Serial.println(millis());
    lastMillis += 1000;
  }

  delay(1);
}
```

The string input from UART is terminated with CR(0x0d)+LF(0x0a).   
```
I (1189799) UART-RX: 0x3ffc8458   48 65 6c 6c 6f 20 57 6f  72 6c 64 20 31 30 30 31  |Hello World 1001|
I (1189799) UART-RX: 0x3ffc8468   0d 0a
```

The string output to UART must be terminated with LF(0x0a).  
If the string output to the UART is not terminated with LF(0x0a), the Arduino will complete the input with a timeout.   
The string output to the UART is echoed back.   
```
I (1285439) UART-TX: 0x3ffc72f8   61 62 63 64 65 66 67 0a                           |abcdefg.|
I (1285459) UART-RX: 0x3ffc8458   61 62 63 64 65 66 67 0d  0a                       |abcdefg..|
```


## Connect ESP32 and AtMega328 using wire cable   

|AtMega328||ESP32|ESP32S3|ESP32C2/C3/C6|
|:-:|:-:|:-:|:-:|:-:|
|TX|--|GPIO16|GPIO2|GPIO1|
|RX|--|GPIO17|GPIO1|GPIO0|
|GND|--|GND|GND|GND|

__You can change it to any pin using menuconfig.__   


# Windows Application   
I used [this](https://sourceforge.net/projects/sockettest/) app.   
This application runs both a UDP-Listener (UDP-Server) and a UDP-Client simultaneously.   

- Listen from ESP32   
	Specify the port number and press the Start Listening button.   
	This application works as a UDP-Listener (UDP-Server) and communicates with the UDP-Client of ESP32.   
	![Image](https://github.com/user-attachments/assets/30bb42e3-6828-4926-bbe5-7f068ef4a052)   
	![Image](https://github.com/user-attachments/assets/c0ef903a-d0a8-4b68-b653-349fc01ac183)   

- Send to ESP32   
	Specify the IP Address and port number and press the Send button.   
	This application works as a UDP-Client and communicates with the UDP-Server of ESP32.   
	![Image](https://github.com/user-attachments/assets/f1d0002b-4f97-4ef4-8bf9-77eec030551e)   
	Instead of an IP address, you can use an mDNS hostname.   
	![Image](https://github.com/user-attachments/assets/5aafff5a-3bab-481d-b65f-98e3dc6fd25e)   

# Python script   
udp-client.py is UDP-Client.   
udp-server.py is UDP-Listener (UDP-Server).   


## Send to ESP32
Specify the IP Address of ESP32 and port number and press the Send button.   
You can use an mDNS hostname instead of an IP address.   
This application works as a UDP-Client and communicates with the UDP-Listener (UDP-Server) of ESP32.   
![Image](https://github.com/user-attachments/assets/0d0d742f-d1ce-41e9-a405-50258b702bf9)

# Linux Application
I searched for a GUI tool that can be used with ubuntu or debian, but I couldn't find one.   

# References

https://github.com/nopnop2002/esp-idf-web-serial

https://github.com/nopnop2002/esp-idf-uart2bt

https://github.com/nopnop2002/esp-idf-uart2mqtt
