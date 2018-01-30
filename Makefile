.DEFAULT_GOAL := build
.PHONY: dependencies build clean upload monitor

dependencies:
	platformio lib --storage-dir ../lib install

clean:
	platformio run -t clean

build:
	platformio run

upload:
	platformio run -t program

monitor:
	platformio serialports monitor --exit-char 88 --baud 9600
