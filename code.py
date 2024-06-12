import RPi.GPIO as GPIO
import time
import requests
import math
from HX711 import SimpleHX711, Rate

# Set your ThingSpeak API key and channel ID
API_KEY = 'RAUHH4DYH3PMEEOH'
CHANNEL_ID = '2524517'

# ThingSpeak API endpoint
TS_API_ENDPOINT = f'https://api.thingspeak.com/update'

GPIO.setmode(GPIO.BCM)
X_axis = 18
Y_axis = 19
Z_axis = 20
vibr = 21

# Set threshold value for vibration sensor (in Hertz)
VIBRATION_THRESHOLD = 10  # Adjust threshold as needed

def X_acc():
    GPIO.setup(X_axis, GPIO.OUT)
    GPIO.output(X_axis, GPIO.LOW)
    time.sleep(0.1)
    GPIO.setup(X_axis, GPIO.IN)
    start_time = time.time()
    while GPIO.input(X_axis) == GPIO.LOW:
        pass
    end_time = time.time()
    total_time = end_time - start_time
    voltage = total_time * 3300 / 1.1
    return voltage

def Y_acc():
    GPIO.setup(Y_axis, GPIO.OUT)
    GPIO.output(Y_axis, GPIO.LOW)
    time.sleep(0.1)
    GPIO.setup(Y_axis, GPIO.IN)
    start_time = time.time()
    while GPIO.input(Y_axis) == GPIO.LOW:
        pass
    end_time = time.time()
    total_time = end_time - start_time
    voltage = total_time * 3300 / 1.1
    return voltage

def Z_acc():
    GPIO.setup(Z_axis, GPIO.OUT)
    GPIO.output(Z_axis, GPIO.LOW)
    time.sleep(0.1)
    GPIO.setup(Z_axis, GPIO.IN)
    start_time = time.time()
    while GPIO.input(Z_axis) == GPIO.LOW:
        pass
    end_time = time.time()
    total_time = end_time - start_time
    voltage = total_time * 3300 / 1.1
    return voltage

def Vibr_value():
    GPIO.setup(vibr, GPIO.OUT)
    GPIO.output(vibr, GPIO.LOW)
    time.sleep(0.1)
    GPIO.setup(vibr, GPIO.IN)
    start_time = time.time()
    while GPIO.input(vibr) == GPIO.LOW:
        pass
    end_time = time.time()
    period = end_time - start_time
    # Calculate frequency (Hz) from period
    if period != 0:
        frequency = 1 / period
    else:
        frequency = 0
    # Check if vibration frequency exceeds threshold
    if frequency > VIBRATION_THRESHOLD:
        print("Risk factor: Vibration threshold exceeded!")
    return frequency

def calculate_angle(x, y, z):
    # Calculate the pitch, roll, and yaw angles in degrees
    pitch = math.atan2(x, math.sqrt(y * 2 + z * 2)) * (180 / math.pi)
    roll = math.atan2(y, math.sqrt(x * 2 + z * 2)) * (180 / math.pi)
    yaw = math.atan2(math.sqrt(x * 2 + y * 2), z) * (180 / math.pi)
    # Adjust angles to the range of 0 to 360 degrees
    pitch = (pitch + 360) % 360
    roll = (roll + 360) % 360
    yaw = (yaw + 360) % 360
    return pitch, roll, yaw

try:
    with SimpleHX711(14, 15, 1, -15744, Rate.HZ_80) as hx:
        while True:
            X = X_acc()
            Y = Y_acc()
            Z = Z_acc()
            V = Vibr_value() 
            weight = hx.weight()
            pitch, roll, yaw = calculate_angle(X, Y, Z)
            print("X Axis (Pitch):", pitch)
            print("Y Axis (Roll):", roll)
            print("Z Axis (Yaw):", yaw)
            print("Vibration sensor frequency (Hz):", V)
            print("Weight:", weight)
            # Send data to ThingSpeak
            params = {'api_key': API_KEY, 'field1': pitch, 'field2': roll, 'field3': yaw, 'field4': V, 'field5': weight}
            response = requests.get(TS_API_ENDPOINT, params=params)
            print("Data sent to ThingSpeak")
            time.sleep(15)  # Adjust delay as needed

except KeyboardInterrupt:
    GPIO.cleanup()
    print("Program terminated by user")