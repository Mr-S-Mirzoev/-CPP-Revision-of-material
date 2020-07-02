# Dependencies

## For linux
- libnotify:

Could be installed on Linux as:

	sudo apt-get install libnotify-dev
	
## For MacOS
There aren't any.

# Compilation

## On linux

	g++ -o dynamic dynamic.cpp notifications.cpp -std=c++17 `pkg-config --cflags --libs libnotify`
	
## On MacOS

	g++ -o dynamic dynamic.cpp notifications.cpp -std=c++17

# What can this programm do:

Both programms work correctly on Linux and MacOS.
  
## Timer
It can set up a timer which will send a notification to you even if you interrupted the programm with SIGINT (in most cases ^C) or even SIGHUP (for example when bash terminal is closed).

For example, for bash command:
  
	  ./dynamic timer 10 Brush my teeth

If you wait till it finishes:

![Image of Finish](https://github.com/Mr-S-Mirzoev/-CPP-Revision-of-material/blob/master/for_fun/no-interrupt.png?raw=true)

If you don't:

![Image of Finish](https://github.com/Mr-S-Mirzoev/-CPP-Revision-of-material/blob/master/for_fun/interrupt.png?raw=true)

## Progress bar:

	  ./dynamic progress 20

Generates a random progress bar with length 20 in symbols.
