# Vision-Based Multi-Robot Target Following with JetBot

## Overview

This project implements a real-time target-following system for multiple JetBot robots. Instead of using a trained AI model, the system uses OpenCV-based HSV color segmentation and pixel counting to detect a red target from camera input.

Each JetBot streams camera images to a client computer, where image processing and control logic are handled. The client then sends motor commands back to the robots over Wi-Fi.

---

## Key Features

- Real-time camera-based target detection
- OpenCV HSV color segmentation
- Pixel counting and bounding box detection
- HTTP client-server communication
- Differential-drive motor control
- Search behavior when the target is not visible
- Multi-robot cooperation using secondary robot colors
- Physical testing with visible, blocked, and moving target scenarios

---

## System Architecture

```text
JetBot Robot
├── Camera stream
├── Local HTTP server
└── Motor control

Client Computer
├── Image processing
├── Target detection
├── Control logic
├── Shared robot-color tracking
└── Motor command generation
```

---

## How It Works

1. The client requests an image from the JetBot camera.
2. OpenCV detects the red target using HSV color masks.
3. The system calculates the target position in the image.
4. The robot turns left, turns right, moves forward, searches, or stops.
5. If a robot cannot see the red target, it can follow another robot that has already detected it.

---

## Results

The system successfully demonstrated:

- Red target detection
- Real-time target following
- Search behavior when the target was not visible
- Cooperative multi-robot tracking
- Response to a moving target

The project showed that useful multi-robot behavior can be achieved with simple, explainable computer vision and control logic.

---

## Limitations

- Sensitive to lighting changes
- No obstacle avoidance
- No exact distance estimation
- Manual HSV color calibration
- Limited testing environment

---

## Future Improvements

- Add obstacle avoidance
- Add automatic color calibration
- Improve distance estimation
- Test in more environments
- Add machine learning-based object detection
