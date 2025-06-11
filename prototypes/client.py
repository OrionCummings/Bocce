import socket
import time, traceback
from common import is_new_client_message_ack

def send(msg: str):
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print(f"Connecting to {HOST}:{PORT}")
        s.connect((HOST, PORT))

        print(f"Sending '{msg}'")
        s.sendall(msg.encode('ascii'))

        decoded_request = s.recv(1024).decode('ascii')
        if cid := is_new_client_message_ack(decoded_request):
            s.send("New client connection ack received".encode()) 
        time.sleep(1)

###################################################

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 53278  # The port used by the server
# start_time = 0
# end_time = 0
# while(end_time < start_time + 2):
#     start_time = time.time()
#     send("CN.1")
#     end_time = time.time()

send("CN.1")
