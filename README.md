# Wardriver!
GPS and Wifi Acces point tracking device.

I built a GPS tracker for the sake of learning, but it's proven to be kind of useful already and it gave me a chance to prove out Access Point trilaterating using location data and signal strength.

The Wifi trilaterating idea and methodology came from [this post](https://appelsiini.net/2017/trilateration-with-n-points/), but I've adapted it to my data and javascript.

The GPS data is being polled from a Neo-6M GPS device over serial using the [TinyGPS+ Library](https://github.com/mikalhart/TinyGPSPlus).

The various methods of data extrapolation are from all over the internet, although I found [this article](https://www.movable-type.co.uk/scripts/latlong.html) to be particularly helpful.

## Simple video showing the device and talking about how well it worked
[![Simple video showing the device and talking about how well it worked](http://img.youtube.com/vi/ao37SIXwli4/0.jpg)](https://www.youtube.com/watch?v=ao37SIXwli4 "GPS logging + WiFi Access Point locating Demo.")

## Video walkthrough of the hardware build
COMING SOON!

## Todo
1. Remake this on a smaller board with no controls and screen, get it as small as possible, add i2c and a pinout so it's alot more useable as a module for another device. Maybe omit the memory card reader as well, or just add an old sd card to it permanently.
2. Cleanup code, add settings for disabling hardware that isn't essential to make it easier to use for other builds ( disable SD card, incase someone builds it but doesn't want that for example. )
3. Make .net console app you can drag log into and have different file format extrapolated logs saved out automatically.

## Git contents
1. The arduino firmware can be found in the /Arduino/ directory.
2. The html / css / javascript needed to extrapolate log data can be found in the /GPS_Data_Parser/ directory. NOTE: This has only been tested in chrome, and requires an internet connection for the map functionality to work.
3. You'll find the awful graphics I drew in the /Bitmaps/ directory.

## Hardware
1. Wemos D1 Mini Pro to control everything [Amazon Link](https://www.amazon.com/gp/product/B07333L9VR/ref=oh_aui_detailpage_o07_s00?ie=UTF8&psc=1)
2. Oled sheild for display [Amazon Link](https://www.amazon.com/gp/product/B071DW4CZH/ref=oh_aui_detailpage_o08_s00?ie=UTF8&psc=1)
3. Micro SD Card Reader [Amazon Link](https://www.amazon.com/gp/product/B01JYNEX56/ref=oh_aui_detailpage_o03_s01?ie=UTF8&psc=1)
4. GPS Module [Amazon Link](https://www.amazon.com/gp/product/B015R62YHI/ref=oh_aui_detailpage_o03_s01?ie=UTF8&psc=1)
5. Wifi Antenna + ipex to sma adapter [Amazon Link](https://www.amazon.com/Highfine-Wireless-Extension-Bulkhead-Repeater/dp/B01GMBUS8O/ref=sr_1_12?s=electronics&ie=UTF8&qid=1530896884&sr=1-12&keywords=wifi+antenna+extension)
6. 2 10k Resistors
3. Tactile push buttons

## Schematic:
COMING SOON!

##Software description
For the Arduino firmware the setup code has been moved out into a boot sequence to keep the ESP Watchdog happy by spacing everything out so nothing takes too long. Also nice for indicating the boot steps. The loop uses an app state to know whether it's running the menu system or some actual tasks. I tried to separate out functionality like input, display, menu, etc. into multiple files to help keep the code easier to follow.

And for the GPS Data Parser I chose to just make a little webpage that you run locally to parse the data as it was easy to do quickly and gave me the map integration that I really wanted without any extra steps. When the page loads it prompts you for log data, then it feeds that to a WebWorker to extrapolate the coordinates and wifi data into an array of points with data such as speed, bearing, distance, time, Wifi access points found nearby, etc. Once that process is completed the WebWorker passes that info back to the page. The page will then load a google map showing all of these points and centering on it. From there you can zoom in and click on an individual point to see all the Wifi access points found nearby ( if Wifi logging was enabled. ) Then you can click on an access point from that menu, which will que the page to find all other copies of that access point that were logged and start attempting to trilaterate it's location.

## Takeaways from this project
The first thing I noticed was that the GPS device I used can be pretty inaccurate on a small scale. I knew the accuracy was only good down to around 8 meters, but It was interesting to really see. With that being said, it is really neat how much data you can derive from a moving set of positions like this.

I also realized that I could extend the life and usefulness of all the goofy gadgets I make by giving them decent serial communication. That's what I did for this, because I knew I wouldn't need a generic GPS device often, or a Wifi locator ever, but I may need GPS for another project one day, and all this work is already done for it!
	
I also learned that a lot of what we call triangulation, is actually trilateration. GPS for example; we always call it GPS triangulating, but it's actually trilaterating! 
