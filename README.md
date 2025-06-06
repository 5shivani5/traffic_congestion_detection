 Traffic Congestion Detection & LoRa Alert System
Hey! This is a project I worked on to solve the problem of traffic congestion in narrow internal roads. The idea is simple—monitor the live traffic using a webcam, detect congestion using YOLOv8, and then alert incoming vehicles through a LoRa-based wireless system so they don’t enter and make the jam worse.

Also, I’ve created a web dashboard so that people (or authorities) can see the current traffic status from anywhere on the network.

 What this project does
Captures live video of vehicles inside a narrow/cut road.

Detects congestion using YOLO (car, bike, truck, bus).

Sends the congestion status wirelessly using LoRa (ESP32 + E32) to a receiver kept at the road entrance.

Displays current traffic condition on a simple Flask-based website.

Hardware:
ESP32 (LoRa transmitter)

LoRa E32 module

(Optional) LoRa receiver with OLED/Buzzer for alert

Webcam (USB or laptop camera)

Software:
YOLOv8 (ultralytics package)

Python (OpenCV, Flask)

ESP32 Arduino code using LoRa_E32 library

WORKING:
I run a Python script (main.py) that uses YOLOv8 to detect vehicles from a live camera.

Based on how many vehicles are there, their movement speed, and area, I decide whether it's:

NO Congestion

MEDIUM Congestion (slow movement)

HIGH Congestion (jam or dense)

This status is pushed to:

My Flask web server (localhost)

A public server (optional, for online testing)

Then, an ESP32 board fetches this status from Flask using WiFi and sends it via LoRa to another ESP32 receiver at the entrance of the road.

END RESULT:
Real-time traffic condition printed on the video frame.

Console log of detected congestion type.

ESP32 shows "Sent via LoRa: [status]" every 10 seconds.

Web app shows status in color:

🔴 RED – High congestion

🟠 ORANGE – Medium

🟢 GREEN – No congestion

