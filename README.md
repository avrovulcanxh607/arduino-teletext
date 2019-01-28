# arduino-teletext
Library for Arduino to control STV5348 teletext decoder IC. It should be able to control the main functions of other compatible ICs like the SAA5231, SDA5243 or STV5345 but I haven't tried them so use at your own risk.
Not all available functions are implemented yet, but I do intend to implement them at some point.

## Setup Instructions
Download the repsitory as a zip or with git. Make a folder in the Arduino IDE's Library folder called 'teletext' and unzip/copy all the files into it. Make a folder inside called 'examples', with a folder inside called 'teletext_decoder' and copy the 'teletext_decoder.ino' file into it. Github won't let me upload the folders for some reason. Then you can open the IDE and point to 'File' > 'Examples' > 'teletext' > 'teletext_decoder' to open the example sketch and upload it to your arduino. When it's uploaded you can open the serial monitor. Type a page number and hit 'enter' to call a page, or 'c' and 'enter' to toggle conceal on and off. For more detailed instructions see [here](https://www.nathanmediaservices.co.uk/projects/projects.php?link=Teletext%20In-Vision%20Decoder "Project Page").
