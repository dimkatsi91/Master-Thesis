#!/usr/bin/bash

# Run this script with Python 3/ For python 2 use Tkinter 'T' capital 

import os
import time
import numpy as np
import cv2
from Tkinter import *
from PIL import Image, ImageTk
from PIL import *



def processCallTest():
            os.system("./test")
            time.sleep(5)
            # Next add code to open log.txt file and display contents in a dialog box in GUI #
            File = open("/home/pi/Desktop/log.txt").readlines()
            File = open('new_logo.txt','w').writelines(File[1:])
            myFile = open("/home/pi/Desktop/new_logo.txt")
            DecText = Label(window, text=myFile.read(), bg="white")
            DecText.configure(width=80, height=80)
            DecText.configure( font=("Courier",6) )
            DecText.pack(side=LEFT, padx=20, pady=80)
            # Display Cutted QR code symbol Image using ImageTk #
            image = Image.open("/home/pi/Desktop/final/CUTTED_QR.png")
            photo = ImageTk.PhotoImage(image)
            label = Label(image=photo)
            label.image = photo
            label.configure(width=120, height=120, padx=100, pady=100)
            label.pack()
            # Display Captured Image #
            image = Image.open("/home/pi/Desktop/final/ORIGINAL_IMAGE.png")
            photo = ImageTk.PhotoImage(image)
            label2 = Label(image=photo)
            label2.image = photo
            label2.pack()
            # Here place all widgets nicely #
            window.geometry("680x540")
            window.focus_set()
            window.configure(background="navy")
            buttonCall.place(x=10, y=40)
            buttonCall.configure(font=("Courier",10), bd=8)
            #btShowCuttedImg.place(x=10, y= 60)
            btCloseApp.place(x=10, y=100)
            btCloseApp.configure(font=("Courier",10), bd=8)
            DecText.place(x=360, y=260, width=300, height=160)   ## Decoded Data Information 
            label.place(x=440, y=60)    ## label  -> QR code symbol Cutted Image 
            label2.place(x=10, y=260)   ## label2 -> original image
            # Lastly, add information in the top of displyed images #
            orig_img_info = Label(window, text="Captured Image")
            orig_img_info.configure(font=("Courier",10), bg="gray", fg="orange", bd=8, relief=RAISED)
            orig_img_info.place(x=125, y=220)
            cutted_img_info = Label(window, text="QR Code Symbol")
            cutted_img_info.configure(font=("Courier",8), bg="gray", fg="orange", bd=6, relief=RAISED)
            cutted_img_info.place(x=444, y=20)


def closeApp():
    exit()
        

window = Tk()
window.configure(background='white')
buttonCall = Button(window, text="Capture Image", bg="gray", fg="orange", command=processCallTest)

btCloseApp = Button(window, text="Close App", bg="gray", fg="orange", command=closeApp)


buttonCall.pack( padx=5, pady=5 )
btCloseApp.pack( padx=5, pady=5 )


window.mainloop()


