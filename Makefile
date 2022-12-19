PORT ?= /dev/ttyUSB0
BOARD ?= arduino:avr:uno
BAUDRATE ?= 9600

default: lint compile upload

lint:
	@cpplint --extensions=ino --filter=-legal/copyright *.ino || echo '[WARNING]: cpplint is not installed!'

compile:
	@arduino-cli compile -b ${BOARD}

upload:
	@arduino-cli compile -u -b ${BOARD} --port ${PORT} .

monitor:
	@python monitor.py --port ${PORT} --baudrate ${BAUDRATE}
