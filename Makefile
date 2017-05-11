.DEFAULT_GOAL := build
.PHONY: build clean upload monitor

clean:
	platformio run -t clean

build:
	platformio run

upload:
	platformio run -t upload

monitor:
	platformio serialports monitor --exit-char 88 --baud 9600
