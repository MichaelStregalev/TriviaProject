import socket
import sys

# the IP (the localhost)
SERVER_ADDRESS = '127.0.0.1'
# the port of the server
SERVER_PORT = 8876
# number of bytes to read from the socket
BUFFER_SIZE = 5

# check if the port is valid (between 1024 and 65535)
def validate_port(port):
    
    if (port > 65535 or port < 1024):
        print("Error: Invalid port. Please use a port between 1024 and 65535.")
        sys.exit(1)

# create the socket - this is the main connection object
def create_socket():
    try:
        return socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    except socket.error as err:
        print(f"Socket creation failed: {err}")
        sys.exit(1)

# try to connect to the server. if it fails, ask the user if they want to keep trying
def connect_to_server(sock, address, port):
    while True:
        try:
            sock.connect((address, port))
            print(f"Connected to server at {address}:{port}")
            break  # successful connection, so we exit the loop
            
        except ConnectionRefusedError:
            print(f"Connection refused. No server found at {address}:{port}.")
            
        except socket.error as err:
            print(f"Connection failed: {err}")
        
        # ask the user if they want to retry connecting
        choice = input("Do you want to retry? (y/n): ").strip().lower()
        if choice != 'y':
            sock.close()
            sys.exit("Exiting client.")

# receive the 5 bytes from the server
def receive_hello(sock):
    try:
        data = sock.recv(BUFFER_SIZE).decode('utf-8')
        print(f"Received message: {data}")
        return data
    except socket.error as err:
        print(f"Error receiving data: {err}")
        sock.close()
        sys.exit(1)

# send "Hello" back to the server (only if the message matched)
def send_hello(sock, message):
    try:
        sock.sendall(message.encode('utf-8'))
        print("Message sent back to the server.")
    except socket.error as err:
        print(f"Failed to send message: {err}")
        sock.close()
        sys.exit(1)

# main function - runs the whole client flow
def main():
    validate_port(SERVER_PORT)
    client_socket = create_socket()
    connect_to_server(client_socket, SERVER_ADDRESS, SERVER_PORT)

    message = receive_hello(client_socket)
    # only send back if the message was exactly "Hello"
    
    print("MESSAGE FROM THE SERVER: " + message)
    
    if message == "Hello":
        print("Server sent 'Hello' as expected. Sending it back.")
        send_hello(client_socket, message)
    else:
        print("Unexpected message received.")

    client_socket.close()
    print("Connection closed.")

# run the client
if __name__ == "__main__":
    main()
