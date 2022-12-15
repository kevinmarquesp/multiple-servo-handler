PORT ?= /dev/ttyUSB0
BOARD ?= arduino:avr:uno
BAUDRATE ?= 9600

default: lint compile upload

lint:
	@cplint --extensions=ino --filter=-legal/copyright *.ino || echo '[WARNING]: cpplint is not installed!'

compile:
	@arduino-cli compile -b ${BOARD}

upload:
	@arduino-cli upload -b ${BOARD} --port ${BOARD} .

monitor:
	@python monitor.py --port ${PORT} --baudrate ${BAUDRATE}
