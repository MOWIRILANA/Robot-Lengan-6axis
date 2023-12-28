import cv2
import imutils
import sys
import os
import numpy as np
import argparse
import time
import pickle, struct, socket, urllib.request
from imutils.video import VideoStream



url= 'http://192.168.43.147/cam-lo.jpg'
im=None
# vs = VideoStream(src=0).start()
# time.sleep(2.0)

# Function to send data to ESP32
def send_data(client_socket, data):
    data = pickle.dumps(data)
    # Remove spaces from the serialized data
    data = data.replace(b' ', b'')
    message_size = struct.pack("L", len(data))  # Pack the message size as a long integer
    client_socket.sendall(message_size + data)

# ESP32 server address and port
esp32_ip = '192.168.43.232'  # Replace with your ESP32's IP address
esp32_port = 12345  # Replace with the port your ESP32 is listening on

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the ESP32 server
client_socket.connect((esp32_ip, esp32_port))

ARUCO_DICT = {
    "DICT_4X4_50": cv2.aruco.DICT_4X4_50,
    "DICT_4X4_100": cv2.aruco.DICT_4X4_100,
    "DICT_4X4_250": cv2.aruco.DICT_4X4_250,
    "DICT_4X4_1000": cv2.aruco.DICT_4X4_1000,
}
# arucoDict = cv2.aruco.Dictionary_get(ARUCO_DICT[args['type']])
# arucoParams = cv2.aruco.DetectorParameters_create()
arucoParams = cv2.aruco.DetectorParameters()
arucoParams_2 = cv2.aruco.DetectorParameters()
arucoParams_3 = cv2.aruco.DetectorParameters()
arucoParams_4 = cv2.aruco.DetectorParameters()

arucoDict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
arucoDict_2 = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
arucoDict_3 = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
arucoDict_4 = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)


CACHED_PTS = None
CACHED_IDS = None
Line_Pts = None
measure = None

CACHED_PTS_2 = None
CACHED_IDS_2 = None
Line_Pts_2 = None
measure_2 = None

CACHED_PTS_3 = None
CACHED_IDS_3 = None
Line_Pts_3 = None
measure_3 = None

CACHED_PTS_4 = None
CACHED_IDS_4 = None
Line_Pts_4 = None
measure_4 = None

while True:
    
    Dist = []

    img_resp=urllib.request.urlopen(url)
    imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    image = cv2.imdecode(imgnp,-1)

    # image = vs.read()
    image = imutils.resize(image, width=800)
    detector = cv2.aruco.ArucoDetector(arucoDict,arucoParams)
    markerCorners, markerIDs, rejectedImgPoints = detector.detectMarkers(image)
    frame_markers = cv2.aruco.drawDetectedMarkers(image.copy(), markerCorners, markerIDs)

    if len(markerCorners) <= 0:
        if CACHED_PTS is not None:
            markerCorners = CACHED_PTS
    if len(markerCorners) > 0:
        CACHED_PTS = markerCorners
        if markerIDs is not None:
           markerIDs = markerIDs.flatten()
           CACHED_IDS = markerIDs
        else:
           if CACHED_IDS is not None:
              markerIDs = CACHED_IDS
        if len(markerCorners) < 2:
           if len(CACHED_PTS) >= 2:
              corners = CACHED_PTS
        for (markerCorners, markerIDs) in zip(markerCorners, markerIDs):
            corners_abcd = markerCorners.reshape((4, 2))
            (topLeft, topRight, bottomRight, bottomLeft) = corners_abcd
            topRightPoint = (int(topRight[0]), int(topRight[1]))
            topLeftPoint = (int(topLeft[0]), int(topLeft[1]))
            bottomRightPoint = (int(bottomRight[0]), int(bottomRight[1]))
            bottomLeftPoint = (int(bottomLeft[0]), int(bottomLeft[1]))
            cv2.line(image, topLeftPoint, topRightPoint, (0, 255, 0), 2)
            cv2.line(image, topRightPoint, bottomRightPoint, (0, 255, 0), 2)
            cv2.line(image, bottomRightPoint, bottomLeftPoint, (0, 255, 0), 2)
            cv2.line(image, bottomLeftPoint, topLeftPoint, (0, 255, 0), 2)
            cX = int((topLeft[0] + bottomRight[0])//2)
            cY = int((topLeft[1] + bottomRight[1])//2)

#Perhitungan jarak antara aruco 1
            measure = abs(3.5 / (topLeft[0] - cX)) * 2.54  # Konversi ke sentimeter
            cv2.circle(image, (cX, cY), 4, (255, 0, 0), -1)
            cv2.putText(image, str(int(markerIDs)), (int(topLeft[0] - 10), int(topLeft[1] - 10)), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255))
            Dist.append((cX, cY))
            if len(Dist) == 0:
                if Line_Pts is not None:
                    Dist = Line_Pts
            if len(Dist) == 2:
                Line_Pts = Dist
            if len(Dist) == 2:
                cv2.line(image, Dist[0], Dist[1], (255, 0, 255), 2)
                ed = ((Dist[0][0] - Dist[1][0]) ** 2 + ((Dist[0][1] - Dist[1][1]) ** 2)) ** 0.5
                # Konversi panjang dari inci ke sentimeter
                ed = measure * ed

                ed_int = round(ed)
                # ed_int = max(0, min(ed_int, 255))
                # ed_bytes = struct.pack("B", ed_int)
                edlast = round(measure * ed)
                pesan = str(edlast)
                send_data(client_socket, str(pesan).encode('utf-8'))
                cv2.putText(image, str(int(edlast)) + "cm", (int(300), int(300)), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255))
                print("ed", edlast)
                # eds = 25
                

    cv2.imshow("display", image)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows()
client_socket.close()
# url.stop()
