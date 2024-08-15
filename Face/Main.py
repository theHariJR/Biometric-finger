import face_recognition
import cv2
from time import sleep
#from openpyxl import Workbook
import datetime
import csv
import json
import time
import os
import urllib.request
import time
import os
import serial
#ser = serial.Serial('COM5', 9600, timeout = 1)
import urllib.request
import numpy as np


url='http://192.168.137.182/capture'

import serial

import time, traceback

import schedule

from threading import Timer

class RepeatedTimer(object):
    def __init__(self, interval, function, *args, **kwargs):
        self._timer     = None
        self.interval   = interval
        self.function   = function
        self.args       = args
        self.kwargs     = kwargs
        self.is_running = False
        self.start()

    def _run(self):
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False
        
        


def do_something():
    global ft
    ser.write(ft.encode())
    time.sleep(0.01)
    ser.flush()
    
ser = serial.Serial('COM5', 9600, timeout = 1) # ttyACM1 for Arduino board
#rt = RepeatedTimer(3, do_something)

x=0
y=0
z=0
name=""
xyu=""
top = 4
right = 4
bottom = 4
left = 4
image_1 = face_recognition.load_image_file("1.jpg")
image_1_face_encoding = face_recognition.face_encodings(image_1)[0]
image_2 = face_recognition.load_image_file("2.jpg")
image_2_face_encoding = face_recognition.face_encodings(image_2)[0]
known_face_encodings = [
    image_1_face_encoding,
    image_2_face_encoding,
    ]
known_face_names = ["HARI","CHARLIE"]
face_locations = []
face_encodings = []
face_names = []
video_capture = cv2.VideoCapture(0)
first_match_index="9"
while True:
	imgResp=urllib.request.urlopen(url)
	global ft
	imgNp=np.array(bytearray(imgResp.read()),dtype=np.uint8)
	frame=cv2.imdecode(imgNp,-1)
	#cv2.imwrite("temp.jpg",frame)
	process_this_frame = True
	#ret, frame = video_capture.read()
	small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)
	rgb_small_frame = small_frame[:, :, ::-1]
	name=""
	if process_this_frame:
		face_locations = face_recognition.face_locations(rgb_small_frame)
		face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)
		print(len(face_encodings))
		first_match_index=9
		for face_encoding in face_encodings:
		    matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
		    name = "Unknown"
		    cv2.imwrite("int.jpg",frame)
		    if True in matches:
		        first_match_index = matches.index(True)
		        print(first_match_index)
		        name = known_face_names[first_match_index]
		    face_names.append(name)
		    process_this_frame = not process_this_frame
		    for (top, right, bottom, left), name in zip(face_locations, face_names):
		        top *= 4
		        right *= 4
		        bottom *= 4
		        left *= 4
		cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)
		cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
		font = cv2.FONT_HERSHEY_DUPLEX
		cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)
	cv2.imshow('Video', frame)
	face_names=[]
	print(name)
	if "CHARLIE" in name and y==0:
		ft="b"
		y=1
		x=0
		z=0
		print("hi")
		do_something()
	if  "HARI" in name and x==0:
		y=0
		x=1
		z=0
		ft="a"
		print("hi")
		do_something()
	if "Unknown" in name and z==0:
		y=0
		x=0
		z=1
		ft="u"
		print("hi")
		do_something()
	cv2.waitKey(1)
video_capture.release()
cv2.destroyAllWindows()
