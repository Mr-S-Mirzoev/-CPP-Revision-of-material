# Dependencies

- libnotify:

Could be installed on Linux as:

	sudo apt-get install libnotify-dev

# Compilation

	g++ -o dynamic dynamic.cpp -std=c++17 `pkg-config --cflags --libs libnotify`

# What can this programm do:
  
## Timer
It can set up a timer which will send a notification to you even if you interrupted the programm with SIGINT (in most cases ^C) or even SIGHUP (for example when bash terminal is closed).

For example, for bash command:
  
	  ./dynamic timer 10 Pochistit zubi

If you wait till it finishes:

![Image of Finish](https://github.com/Mr-S-Mirzoev/-CPP-Revision-of-material/blob/master/for_fun/no-interrupt.png?raw=true)

If you don't:

![Image of Finish](https://github.com/Mr-S-Mirzoev/-CPP-Revision-of-material/blob/master/for_fun/interrupt.png?raw=true)

## Progress bar:

	  ./dynamic progress 20

Generates a random progress bar with length 20 in symbols.
