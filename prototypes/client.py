import random
import socket
import time

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 53278))

while True:
    message, address = server_socket.recvfrom(1024)
    print(message.decode("ascii"))
    server_socket.sendto(message, address)
    time.sleep(1)