import signal
import socket 
import threading
from common import is_new_client_message

run = True
def handler(signum, frame):
    global run
    run = False
    print("Exiting!")

signal.signal(signal.SIGINT, handler)

bind_ip = "127.0.0.1"
bind_port = 53278

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server.bind((bind_ip, bind_port)) 
# we tell the server to start listening with 
# a maximum backlog of connections set to 5
server.listen(1) 
# server.settimeout(2)

print(f"[+] Listening on port {bind_ip} : {bind_port}")

# client handling thread
def handle_client(client_socket): 
    # printing what the client sends 
    request = client_socket.recv(1024) 
    decoded_request = request.decode('ascii')
    print(f"[+] Recieved: '{decoded_request}'") 
    # sending back the packet 
    if cid := is_new_client_message(decoded_request):
        client_socket.send(f"CN.{cid}.ACK".encode()) 
    client_socket.close()

while run: 
    # When a client connects we receive the 
    # client socket into the client variable, and 
    # the remote connection details into the addr variable
    client, addr = server.accept() 
    print(f"[+] Accepted connection from: {addr[0]}:{addr[1]}")
    # spin up our client thread to handle the incoming data 
    client_handler = threading.Thread(target=handle_client, args=(client,))
    client_handler.start() 