import signal
import socket 
import time

run = True
def handler(signum, frame):
    global run
    run = False
    print("Exiting!")

signal.signal(signal.SIGINT, handler)

bind_ip = "127.0.0.1"
bind_port = 53278

for pings in range(1000000):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(1.0)
    message = f"{pings}".encode('ascii')
    addr = ("127.0.0.1", 53278)

    start = time.time()
    client_socket.sendto(message, addr)
    try:
        data, server = client_socket.recvfrom(1024)
        end = time.time()
        elapsed = end - start
        # print(f'{data} {pings} {elapsed}')
        print(data.decode('ascii'))
    except socket.timeout:
        # print('REQUEST TIMED OUT')
        ...