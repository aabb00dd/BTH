#!/usr/bin/env pybricks-micropython
# Pybricks imports
from pybricks import robotics
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, ColorSensor, UltrasonicSensor
from pybricks.parameters import Port, Stop, Direction, Color
from pybricks.tools import wait, StopWatch
from pybricks.robotics import DriveBase
from pybricks import messaging

# Robot definitions
ev3 = EV3Brick()

# Motor definitions
left_motor = Motor(Port.B)
right_motor = Motor(Port.C)
robot = DriveBase(left_motor, right_motor, wheel_diameter=55.5, axle_track=104)

# Sensor definitions
left_light = ColorSensor(Port.S3)
right_light = ColorSensor(Port.S2)
distance_sensor = UltrasonicSensor(Port.S4)


def connect_to_server():
    SERVER = 'robot3'
    client = messaging.BluetoothMailboxClient()
    mbox = messaging.TextMailbox('greeting', client)
    client.connect(SERVER)
    print("Connected")
    mbox.send("hello")
    mbox.wait()
    print(mbox.read())
    print("Established connection", mbox.read())
    return mbox


def color_initialization():
    ev3.speaker.beep()
    wait(3000)
    g_color = left_light.reflection()
    ev3.speaker.beep()
    wait(3000)
    l_color = left_light.reflection()
    ev3.speaker.beep()
    wait(2000)
    return g_color, l_color


def search_line(const_speed, const_average):
    while const_average < left_light.reflection():
        robot.drive(const_speed, 0)


def parking(spd, bk_spd, safe_dis, mail_b):
    rotation_speed = 50
    backwards_rotation_speed = rotation_speed / (-4)
    robot.drive(0, rotation_speed)
    wait(400)
    robot.drive(bk_spd, 0)
    wait(400)
    robot.drive(bk_spd, rotation_speed)
    wait(300)
    robot.drive(0, rotation_speed)
    wait(1200)
    if distance_sensor.distance() > safe_dis*3:
        robot.drive(spd, 0)
        wait(5500)
        robot.drive(0, 0)
        ev3.light.on(Color.RED)
        mail_b.send("parked")
        mail_b.wait()
        ev3.light.on(Color.BLUE)
        mail_b.wait()
    ev3.light.on(Color.GREEN)
    robot.drive(bk_spd, 0)
    wait(1400)
    robot.drive(bk_spd, backwards_rotation_speed)


def follow_line_from_right():
    correction_constant = 1.7
    turn_correction = correction_constant*(average - right_light.reflection())
    robot.drive(70, -turn_correction)


def follow_line():
    mail_box = connect_to_server()
    ground_color, line_color = color_initialization()
    speed = 40
    average = (ground_color + line_color) / 2
    backwards_speed = speed * -2
    rotation_speed = -1
    hard_angle = 80
    safe_distance = 100
    parking_line_safety_diff = 10
    seen_line = False
    seen_white_after_line = False

    search_line(speed, average)

    while True:
        if line_color + parking_line_safety_diff > right_light.reflection() > line_color - parking_line_safety_diff and mail_box.read() == "park":
            seen_line = True
            if seen_white_after_line:
                parking(speed, backwards_speed, safe_distance, mail_box)
                while True:
                    if line_color + parking_line_safety_diff > left_light.reflection() > line_color - parking_line_safety_diff:
                        seen_line = False
                        seen_white_after_line = False
                        robot.drive(0, 0)
                        wait(1000)
                        break
                robot.drive(speed, 10)
                wait(200)
        elif mail_box.read() == "turn":
            print(mail_box.read())
            robot.turn(180)
            follow_line_from_right()

        elif distance_sensor.distance() > safe_distance:
            if seen_line:
                seen_white_after_line = True
            turning = rotation_speed * (left_light.reflection() - average)
            if turning <= hard_angle:
                robot.drive(speed, turning)
            else:
                robot.drive(backwards_speed, turning)
        else:
            robot.drive(0, 0)


follow_line()
