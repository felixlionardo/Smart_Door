# Home Security System Prototype

## Overview

Our prototype home security system simulates remote door locking and real-time monitoring capabilities through a web application. It consists of a servo motor for door locking, an ultrasonic distance sensor for monitoring in front of the door, and a webcam for live video streaming. The system is accessed by the user through a webpage. Users can lock/unlock the door, view the camera feed and room temperature, and manage their email subscriptions through the web interface.

## Features

- **Servo Motor for Door Locking**: Uses PWM for control, although it was excluded from the final demo due to hardware issues.
- **Ultrasonic Distance Sensor**: Utilizes GPIO to detect presence and trigger email notifications.
- **Webcam for Live Streaming**: Streams live video feed to the web application using FFmpeg.
- **Temperature Sensor**: Uses A2D to monitor room temperature.
- **LED and Button**: Controlled via GPIO for additional user interactions.
- **Node.js Web Server**: Provides the web interface for user interactions.
- **Email Notifications**: Managed through the web interface.
- **UDP Communication**: Facilitates communication between the web server and hardware functions in the HAL layer.

## Project Details

- **Multithreaded Hardware Control**: Developed using C, cross-compiled on a BeagleBone microcontroller (ARM architecture) in a Linux environment.
- **Node.js Web Server**: Leveraged Node.js to build the webserver and deployed on Google Cloud Platform, utilizing Firebase for real-time data synchronization.

### Sensor Integration
- **Ultrasonic Sensor**: Successfully integrated to detect presence and trigger email notifications.
- **Temperature Sensor**: Successfully measured temperature changes, with data displayed on the web interface.