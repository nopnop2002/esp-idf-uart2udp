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

## Connect ESP32 and AtMega328 using wire cable   

|AtMega328||ESP32|ESP32S3|ESP32C2/C3/C6|
|:-:|:-:|:-:|:-:|:-:|
|TX|--|GPIO16|GPIO2|GPIO1|
|RX|--|GPIO17|GPIO1|GPIO0|
|GND|--|GND|GND|GND|

__You can change it to any pin using menuconfig.__   


# Windows Application   
I used [this](https://sourceforge.net/projects/sockettest/) app.   

## Listen from ESP32
Specify the port number and press the Start Listening button.   
![Image](https://github.com/user-attachments/assets/d789b63d-f59e-463d-b03f-f55461b4ec22)
![Image](https://github.com/user-attachments/assets/5ce1d2d4-230f-4b57-a09f-8426096dd19b)

## Send to ESP32
Specify the IP Address and port number and press the Send button.   
You can use an mDNS hostname instead of an IP address.   
![Image](https://github.com/user-attachments/assets/0d0d742f-d1ce-41e9-a405-50258b702bf9)

# Linux Application
I searched for a GUI tool that can be used with ubuntu or debian, but I couldn't find one.   

