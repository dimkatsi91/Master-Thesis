#!/usr/bin/bash

import os
import time
import numpy as np
import cv2
from Tkinter import *
from PIL import Image, ImageTk
from PIL import *


class QR_GUI:
    def __init__(self, root):
        self.root=root
        self.root.geometry("720x540")
        self.root.configure(background="snow2")
        self.root.title("QR DECODE APP")
        self.root.focus_set()
    def mainProcedure(self):
        # Buttons for Opening Camera & exiting GUI (PYTHON IDLE) #
        self.buttonCall = Button(self.root, text="Capture Image", bg="navy", fg="yellow", command=self.Decode)
        self.buttonCall.configure(font=("Courier",10), bd=5)
        self.btCloseApp = Button(self.root, text="Close App", bg="navy", fg="yellow", command=self.closeApp)
        self.btCloseApp.configure(font=("Courier",10), bd=5)
        self.buttonCall.pack(side=LEFT, padx=5, pady=5)
        self.btCloseApp.pack(side=RIGHT, padx=5, pady=5)
        self.buttonCall.place(x=10, y=40)
        self.btCloseApp.place(x=500, y=460)
    def Decode(self):
        os.system("./test")
        time.sleep(3)
        # Label for displaying decoded data #
        File = open("/home/pi/Desktop/log.txt").readlines()
        File = open('new_log.txt','w').writelines(File[1:])
        myFile = open("/home/pi/Desktop/new_log.txt")
        self.DecText = Label(window, text=myFile.read(), bg="white")
        self.DecText.configure( font=("Courier",6) )
        self.DecText.pack(side=LEFT, padx=20, pady=80)
        self.DecText.configure(width=180, height=80)
        self.DecText.place(x=360, y=260, width=300, height=160) 
        # Label for displaying captured image #
        image = Image.open("/home/pi/Desktop/final/ORIGINAL_IMAGE.png")
        photo = ImageTk.PhotoImage(image)
        self.label2 = Label(image=photo)
        self.label2.image = photo
        self.label2.pack()
        self.label2.place(x=10, y=260)   ## label2 -> original image
        # Label for displaying cutted QR code symbol image #
        image = Image.open("/home/pi/Desktop/final/CUTTED_QR.png")
        photo = ImageTk.PhotoImage(image)
        self.label = Label(image=photo)
        self.label.image = photo
        self.label.configure(width=120, height=120, padx=100, pady=100)
        self.label.pack()
        self.label.place(x=440, y=60)    ## label  -> QR code symbol Cutted Image
        # Information on top of two images about them #
        self.orig_img_info = Label(window, text="Captured Image")
        self.orig_img_info.configure(font=("Courier",10), bg="navy", fg="yellow", bd=8, relief=FLAT)
        self.orig_img_info.place(x=125, y=224)
        self.cutted_img_info = Label(window, text="QR Code Symbol")
        self.cutted_img_info.configure(font=("Courier",8), bg="navy", fg="yellow", bd=6, relief=FLAT)
        self.cutted_img_info.place(x=444, y=30)
    def closeApp(self):
        exit()       
        



window = Tk()
my_GUI = QR_GUI(window)
my_GUI.mainProcedure()
window.mainloop()
