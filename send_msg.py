import argparse
import socket


def main(args):
    udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    msg = args.msg.encode('utf-8')
    print(msg)
    udp.sendto(msg, (args.ip, args.port))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('msg', help='Message to send via UDP')
    parser.add_argument('-p', '--port', default=4245, type=int, help='UDP target port')
    parser.add_argument('-ip', default='localhost', type=str, help='UDP host address')
    args = parser.parse_args()

    main(args)
