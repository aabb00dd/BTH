import cv2
import numpy as np
from http.server import BaseHTTPRequestHandler, HTTPServer
import threading
import urllib.parse

def get_color_masks(hsv):
    masks = {}

#     # Blue
    lower_blue = np.array([100, 210, 100])
    upper_blue = np.array([110, 255, 255])
    masks["blue"] = cv2.inRange(hsv, lower_blue, upper_blue)

#     # Purple
    lower_purple = np.array([125, 105, 100])
    upper_purple = np.array([140, 135, 255])
    masks["purple"] = cv2.inRange(hsv, lower_purple, upper_purple)

#     # yellow
    lower_yellow = np.array([25, 160, 40])
    upper_yellow = np.array([35, 255, 255])
    masks["yellow"] = cv2.inRange(hsv, lower_yellow, upper_yellow)

    # Orange
    lower_orange = np.array([7, 110, 100])
    upper_orange = np.array([17, 160, 255])
    masks["orange"] = cv2.inRange(hsv, lower_orange, upper_orange)

    # Red
    lower_red_1 = np.array([0, 100, 80])
    upper_red_1 = np.array([10, 255, 255])

    lower_red_2 = np.array([168, 80, 90])
    upper_red_2 = np.array([179, 180, 230])

    red_mask_1 = cv2.inRange(hsv, lower_red_1, upper_red_1)
    red_mask_2 = cv2.inRange(hsv, lower_red_2, upper_red_2)
    masks["red"] = cv2.bitwise_or(red_mask_1, red_mask_2)

    return masks

known_distance_cm = 120
real_height_cm = 27.0         # actual height of your target object in cm
measured_box_height_px = 167.0 # h value when object is 30 cm away
flag_hight = 9.0
flag_distance = 50.0
flag_px = 140.0
# Calculate focal length from height calibration
focal_length_px = (measured_box_height_px * known_distance_cm) / real_height_cm
focal_length_px_flag = (flag_px * flag_distance) / flag_hight

print("Focal length:", focal_length_px)

def estimate_distance_from_height(real_height_cm, focal_length_px, box_height_px):
    if box_height_px <= 0:
        return None

    distance_cm = (real_height_cm * focal_length_px) / box_height_px

    return distance_cm

def detect_multiple_colors(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    masks = get_color_masks(hsv)

    box_colors = {
        "red": (0, 0, 255),
        "blue": (255, 0, 0),
        "purple": (128, 0, 128),
        "yellow": (33, 222, 255),   # white so it is visible
        "orange": (0, 165, 255),
    }

    counts = {}
    detections = []

    for color_name, mask in masks.items():

        small_kernel = np.ones((3, 3), np.uint8)
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, small_kernel, iterations=1)

        big_kernel = np.ones((9, 9), np.uint8)
        mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, big_kernel, iterations=2)
        mask = cv2.dilate(mask, big_kernel, iterations=1)

        counts[color_name] = cv2.countNonZero(mask)

        contours, _ = cv2.findContours(
            mask,
            cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE
        )

        min_area = 1

        valid_contours = [
            contour for contour in contours
            if cv2.contourArea(contour) > min_area
        ]

        if len(valid_contours) == 0:
            continue

        largest_contour = max(valid_contours, key=cv2.contourArea)

        area = cv2.contourArea(largest_contour)
        x, y, w, h = cv2.boundingRect(largest_contour)
        print("Measured box height px:", h)

        if color_name == "red":
            distance_cm = estimate_distance_from_height(
            real_height_cm=real_height_cm,
            focal_length_px=focal_length_px,
            box_height_px=h
            )
        else:
            distance_cm = estimate_distance_from_height(
            real_height_cm=flag_hight,
            focal_length_px=focal_length_px_flag,
            box_height_px=h
            )

        detections.append({
            "color": color_name,
            "x": x,
            "y": y,
            "w": w,
            "h": h,
            "area": area,
            "distance_cm": distance_cm
        })

        cv2.rectangle(
            frame,
            (x, y),
            (x + w, y + h),
            box_colors[color_name],
            2
        )

        cv2.putText(
            frame,
            f"{color_name}: {distance_cm:.1f} cm h={h}px",
            (x, y - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            box_colors[color_name],
            2
        )

    return frame, counts, detections