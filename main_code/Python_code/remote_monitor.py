import random
import time
import logging
from datetime import datetime
import threading
import sys

# Set up logging
logging.basicConfig(filename="sentry_logs.txt", level=logging.INFO, format="%(asctime)s - %(message)s")

# Simulated sensor readings
def read_pir():
    """Simulate PIR (motion) sensor reading."""
    return random.choice([True, False])

def read_mine_sensor():
    """Simulate mine sensor reading with random value."""
    return random.randint(400, 800)

def read_ir_sensors():
    """Simulate infrared (IR) sensors for left or right obstacle detection."""
    return random.choice(["LEFT", "RIGHT", "CLEAR"])

def read_distance():
    """Simulate ultrasonic sensor to measure distance in cm."""
    return random.randint(5, 100)  # distance in cm

def read_temperature():
    """Simulate a temperature sensor reading."""
    return random.uniform(20.0, 35.0)

def read_battery_voltage():
    """Simulate battery voltage sensor reading."""
    return random.uniform(11.0, 14.8)  # Voltage range for lead-acid battery

# Real-time monitoring dashboard
def display_status(enemy_detected, mine_detected, direction, distance, temperature, battery_voltage):
    """Display real-time system status."""
    print("\n================ SYSTEM STATUS ================")
    print(f"Time: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"Enemy Detected: {'YES' if enemy_detected else 'NO'}")
    print(f"Mine Detected: {'YES' if mine_detected else 'NO'}")
    print(f"Obstacle Direction: {direction}")
    print(f"Ultrasonic Distance: {distance} cm")
    print(f"Temperature: {temperature:.2f} C")
    print(f"Battery Voltage: {battery_voltage:.2f} V")
    print("===============================================")

# Logging sensor data
def log_sensor_data(enemy_detected, mine_detected, direction, distance, temperature, battery_voltage):
    """Log sensor data to the file."""
    log_msg = f"Enemy: {'Y' if enemy_detected else 'N'} | Mine: {'Y' if mine_detected else 'N'} | IR: {direction} | Distance: {distance}cm | Temp: {temperature:.2f}C | Battery: {battery_voltage:.2f}V"
    logging.info(log_msg)

# Simulate actions of the sentry gun (Spraying, moving)
def trigger_defense_system():
    """Simulate defense actions like spraying water."""
    print("[DEFENSE] Triggering defense actions!")
    time.sleep(2)  # Simulate time taken for spraying
    print("[DEFENSE] Water sprayed.")

def trigger_mine_detection():
    """Simulate mine detection action."""
    print("[MINE DETECTION] Mine detected, attempting to remove it.")
    time.sleep(3)  # Simulate time taken for mine removal
    print("[MINE DETECTION] Mine removed successfully.")

# Functions for system actions
def move_forward():
    """Simulate the robot moving forward."""
    print("[MOVEMENT] Moving forward...")
    time.sleep(1)

def move_backward():
    """Simulate the robot moving backward."""
    print("[MOVEMENT] Moving backward...")
    time.sleep(1)

def turn_left():
    """Simulate the robot turning left."""
    print("[MOVEMENT] Turning left...")
    time.sleep(1)

def turn_right():
    """Simulate the robot turning right."""
    print("[MOVEMENT] Turning right...")
    time.sleep(1)

def stop_movement():
    """Stop all movement."""
    print("[MOVEMENT] Stopping...")
    time.sleep(1)

# Simulate actions based on detected conditions
def handle_obstacles(direction):
    """Handle detected obstacles by turning or stopping."""
    if direction == "LEFT":
        print("[OBSTACLE] Obstacle detected on left, turning right.")
        turn_right()
    elif direction == "RIGHT":
        print("[OBSTACLE] Obstacle detected on right, turning left.")
        turn_left()
    else:
        print("[OBSTACLE] No obstacle, continuing forward.")
        move_forward()

# System Modes: 0 - Patrol, 1 - Defense, 2 - Mine Detection, 3 - Alert Mode
def system_mode_controller():
    """Control different system modes based on sensor readings."""
    system_mode = 0
    while True:
        # Simulate sensor data
        enemy_detected = read_pir()
        mine_detected = read_mine_sensor() > 500  # Threshold for mine detection
        direction = read_ir_sensors()
        distance = read_distance()
        temperature = read_temperature()
        battery_voltage = read_battery_voltage()

        # Log data
        log_sensor_data(enemy_detected, mine_detected, direction, distance, temperature, battery_voltage)

        # Display status on console
        display_status(enemy_detected, mine_detected, direction, distance, temperature, battery_voltage)

        # Action based on system mode
        if system_mode == 0:
            # Patrol Mode
            if enemy_detected:
                print("[PATROL] Enemy detected, switching to defense mode.")
                system_mode = 1
            elif mine_detected:
                print("[PATROL] Mine detected, switching to mine detection mode.")
                system_mode = 2
            else:
                handle_obstacles(direction)
        elif system_mode == 1:
            # Defense Mode
            trigger_defense_system()
            system_mode = 0  # After defense, return to patrol mode
        elif system_mode == 2:
            # Mine Detection Mode
            trigger_mine_detection()
            system_mode = 0  # After mine detection, return to patrol mode
        elif system_mode == 3:
            # Alert Mode (if something goes wrong)
            print("[ALERT] System in alert mode. Please check!")
            system_mode = 0  # Reset after alert

        # Delay before next cycle
        time.sleep(1)

# Real-time monitoring loop
def monitor_loop():
    """Run the real-time monitoring loop."""
    print("[MONITORING] Military Sentry Gun system started...")
    system_mode_controller()

# Simulate system reset and start
def reset_system():
    """Simulate system reset."""
    print("[RESET] System is being reset...")
    time.sleep(3)
    print("[RESET] System has been reset.")

# Exit function
def exit_system():
    """Simulate system exit."""
    print("[EXIT] Exiting system...")
    sys.exit(0)

# Main entry point for system execution
if __name__ == "__main__":
    try:
        # Start monitoring in a separate thread
        monitoring_thread = threading.Thread(target=monitor_loop)
        monitoring_thread.start()

        # Simulate user interaction
        while True:
            command = input("\nEnter command (reset/exit): ").lower()
            if command == "reset":
                reset_system()
            elif command == "exit":
                exit_system()
            else:
                print("Invalid command.")
    except KeyboardInterrupt:
        print("\n[ERROR] Program interrupted by user.")
        exit_system()
