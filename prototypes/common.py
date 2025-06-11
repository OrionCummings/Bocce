def is_new_client_message(msg: str):

    s = msg.split(".")

    match len(s):
        case 2:
            cmd = s[0]
            cid = s[1]

            if cmd == "CN":
                print(f"Received new connection request from client {cid}")
                return cid
        case _:
            print(f"Unknown message '{msg}'")

    return -1

def is_new_client_message_ack(msg: str):

    s = msg.split(".")

    match len(s):
        case 3:
            cmd = s[0]
            cid = s[1]
            ack = s[2]

            if cmd == "CN" and ack == "ACK":
                print(f"Acknowledged new connection request from client {cid}")
                return cid
        case _:
            print(f"Unknown message '{msg}'")

    return -1