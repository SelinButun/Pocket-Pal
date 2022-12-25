# Pocket-Pal

Author: Selin Butun
Date: December 14, 2022

Hardware used:
Adafruit Circuit Playground Classic (microcontroller)
8x8 LED Matrix
Adafruit l2C Backpack

Description:
Pocket-Pal is the final project I created for my Fundamentals of Digital and Embedded Systems at Swarthmore College for Fall 2022. It's a simplified version of a Tamagotchi that is run on an 8x8 LED matrix and utilizes two buttons.

The Pocket Pal is a device that contains a digital friend. Once turned on, the creature is found in a happy state. One can then choose to either press the left button to “feed” it, which switches the pal to a chewing animation; press the right button to imitate “petting”, which makes the LED matrix switch to a petting animation. If the creature is not interacted with for a long time, it will either display that it is hungry and wants to be fed, or that it is sad and wants to be pet. This function is carried out by the two different timers added into the code. I've also added a battery pack to the physical device as well which makes it portable with AAA batteries, but this is optional. 
