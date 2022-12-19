from argparse import ArgumentParser
from serial import Serial # python3 -m pip install pyserial
import os


def get_args():
    parser = ArgumentParser()
    default_port = 'COM1' if os.name == 'nt' else '/dev/ttyUSB0'

    parser.add_argument('--port', '-p', type=str, default=default_port)
    parser.add_argument('--baudrate', '-b', type=int, default=9600)

    return parser.parse_args()


def main():
    args = get_args()

    try:
        arduino = Serial(port=args.port, baudrate=args.baudrate)

        while True:
            line = str(arduino.readline(), 'utf-8').replace('\n', '')
            print(line)

    except KeyboardInterrupt:
        exit(0)


if __name__ == '__main__':
    main()
