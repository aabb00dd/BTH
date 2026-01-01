# Lego EV3 Line Follower with Bluetooth Communication

This project demonstrates a line-following robot using the Lego EV3 kit, enhanced with Bluetooth communication for remote control and parking commands. The robot uses multiple sensors, including color and ultrasonic sensors, to navigate, detect obstacles, and perform parking maneuvers. The integration of Bluetooth communication allows for dynamic control and interaction with the robot during operation.

---

## Features

- **Dual Color Sensors**: The robot is equipped with two color sensors to accurately detect and follow lines, ensuring smooth navigation even on complex paths.
- **Ultrasonic Sensor**: Enables the robot to detect obstacles and measure distances, which is crucial for safe parking and avoiding collisions.
- **Bluetooth Communication**: The robot can receive commands remotely via Bluetooth, such as instructions to park or turn around.
- **Parking Functionality**: Implements a parking algorithm that uses sensor data to position the robot safely and accurately.
- **Dynamic Line Following**: Adjusts speed and turning angles dynamically based on sensor readings to maintain alignment with the line.

## How It Works

1. **Initialization**: The robot calibrates its sensors to determine the reflection values for the ground and the line.
2. **Line Following**: Using proportional control, the robot adjusts its speed and turning angle to stay on the line.
3. **Obstacle Detection**: The ultrasonic sensor continuously monitors the distance to obstacles, ensuring the robot stops or adjusts its path when necessary.
4. **Bluetooth Commands**: The robot connects to a remote server via Bluetooth and listens for commands such as "park" or "turn" to execute specific actions.
5. **Parking**: When instructed to park, the robot uses its sensors to find a safe spot and maneuvers into position.

## What I Learned

- **Sensor Integration**: Combining multiple sensors (color and ultrasonic) to achieve complex behaviors like line following and obstacle detection.
- **Bluetooth Communication**: Establishing a reliable Bluetooth connection for real-time command exchange between the robot and a remote server.
- **Proportional Control**: Implementing proportional control algorithms to ensure smooth and precise navigation.
- **Algorithm Design**: Designing and testing advanced algorithms for parking and obstacle avoidance.
- **Debugging and Optimization**: Debugging sensor readings and optimizing the robot's performance for real-world scenarios.