checks = -Wall -pedantic-errors
libs = -lgf0 -lgfcore0
no_console = -Wl,-subsystem,windows

default:
	g++ -std=c++17 -O2 ../src/*.cpp -o app $(checks) -I ../include -L ../lib $(libs)