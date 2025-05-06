# Military Sentry Gun Project

## Overview
The **Military Sentry Gun** is an advanced prototype system that utilizes various sensors and actuators to detect and defend against enemies, identify and remove landmines, and avoid obstacles. It consists of multiple components, including:

- **PIR Sensor**: Detects motion (enemy presence).
- **IR Sensors**: Used for obstacle avoidance.
- **Ultrasonic Sensor**: Measures distance for obstacle detection.
- **Gripper**: Detects and removes landmines.
- **Servo Motors**: For controlling the water spraying mechanism for defense.
- **Lead-Acid Battery**: Powers the entire system.

The system is divided into different operating modes:
- **Patrol Mode**: The system moves forward and checks for obstacles or threats.
- **Defense Mode**: Activates the servo motor to spray water when an enemy is detected.
- **Mine Detection Mode**: Detects and removes landmines using the gripper mechanism.

The system's core logic is implemented using **Arduino** for hardware control and **Python** for real-time monitoring and logging.

## Features
- **Real-Time Monitoring**: Tracks the status of sensors, movements, and actions.
- **Sensor Integration**: PIR, IR, ultrasonic, and mine sensors work together.
- **Movement Control**: Moves forward, backward, and turns to avoid obstacles.
- **Water Spraying**: Uses a servo motor to spray water for defense.
- **Mine Detection**: A gripper mechanism is used to detect and remove mines.

## Setup and Installation

### Arduino Code
1. Open the `MilitarySentryGun.ino` file in the **Arduino IDE**.
2. Ensure that your Arduino is connected via USB and select the correct board and port.
3. Upload the code to the Arduino board.

### Python Code
1. Install the required Python libraries:
    ```bash
    pip install random time logging threading
    ```
2. Run the `sentry.py` file in your terminal:
    ```bash
    python sentry.py
    ```

### Folder Structure
```plaintext
Military-Sentry-Gun/
├── main_code/
│   ├── robot_control.py   # Python code for controlling the sentry gun.
│   └── robot.ino          # Arduino code for hardware control.
├── logs/                  # Log files generated during operation.
├── README.md              # Project documentation.
└── .gitignore             # Git ignore file for unnecessary files.
