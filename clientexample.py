import socket
import sys
import json
import struct

# EXIT PROGRAM CONSTS
ERROR_IN_RUN = 1
SUCCESSFUL_RUN = 0

# SERVER CONSTS
# the IP (the localhost)
SERVER_ADDRESS = '127.0.0.1'
# the port of the server
SERVER_PORT = 8876
# the maximum valid port
MAXIMUM_PORT = 65535
# the minimum valid port
MINIMUM_PORT = 1024

# SERVER COMMUNICATION CONSTS
# number of maximum bytes to read from the socket
BUFFER_SIZE = 1024
# the character that represents the desire for a user to continue trying an action
CONTINUE_ACTION = 'y'
# the character that represents the fact that a user does not want to continue trying an action
DONT_CONTINUE_ACTION = 'n'
# Signup action code
SIGNUP_CODE = 1
# login action code
LOGIN_CODE = 2
# the amount of bytes that represent the code of the response
BYTES_FOR_CODE = 1
# the amount of bytes that represent the length of the data JSON
BYTES_FOR_JSON_LENGTH = 4
# the amount of bits in a byte
BITS_IN_BYTE = 8

# check if the port is valid (between 1024 and 65535)
def validate_port(port):
    
    if (port > MAXIMUM_PORT or port < MINIMUM_PORT):
        print(f"Error: Invalid port. Please use a port between {MINIMUM_PORT} and {MAXIMUM_PORT}.")
        sys.exit(ERROR_IN_RUN)

# create the socket - this is the main connection object
def create_socket():
    try:
        return socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    except socket.error as err:
        print(f"Socket creation failed: {err}")
        sys.exit(ERROR_IN_RUN)

# try to connect to the server. if it fails, ask the user if they want to keep trying
def connect_to_server(sock, address, port):
    while True:
        try:
            sock.connect((address, port))
            print(f"Connected to server at {address}:{port}")
            break  # successful connection, so we exit the loop
            
        except ConnectionRefusedError:
            print(f"Connection refused.\nNo server found at {address}:{port}.")
            
        except socket.error as err:
            print(f"Connection failed: {err}")
        
        # ask the user if they want to retry connecting
        choice = input(f"Do you want to retry? ({CONTINUE_ACTION}/{DONT_CONTINUE_ACTION}): ").strip().lower()
        if choice.strip().lower() != CONTINUE_ACTION:
            sock.close()
            sys.exit("Exiting client.")

# Prompt for user sign-up or login
def user_login_or_signup():
    print("Welcome to Python Trivia Client Example")
    while True:
        try:
            choice = int(input("Choose an option:\n1. Sign Up\n2. Log In\nEnter choice: ").strip())
            if choice in [SIGNUP_CODE, LOGIN_CODE]:
                return choice
            print("Choose a valid option!")
        except ValueError:
            print("Choose an integer value!")

# Get user details for login or sign up
def get_user_details(choice):
    
    if choice == SIGNUP_CODE:  # Sign Up
        user_name = input("Enter your username: ").strip()
        password = input("Enter your password: ").strip()
        email = input("Enter your email: ").strip()
        return {'userName': user_name, 'password': password, 'email': email}
    
    elif choice == LOGIN_CODE:  # Log In
        user_name = input("Enter your username: ").strip()
        password = input("Enter your password: ").strip()
        return {'userName': user_name, 'password': password}
    
    else:
        print("Invalid choice! Exiting.")
        sys.exit(ERROR_IN_RUN)

# Create protocol message (byte sequence)
def create_protocol_message(choice, user_details):
    # Step 1: Determine the request type code
    request_type = choice

    # Step 2: Serialize the user data to JSON
    user_data_json = json.dumps(user_details)

    # Step 3: Calculate the length of the JSON data (4 bytes)
    json_data_length = len(user_data_json)

    # Step 4: Create the binary protocol message
    message = struct.pack('!B', request_type)  # 1 byte for request type
    message += struct.pack('!I', json_data_length)  # 4 bytes for the length of the JSON data
    message += user_data_json.encode('utf-8')  # Variable length for user data

    # Convert each byte in the message to its 8-bit binary representation
    binary_message = str(''.join(format(byte, '08b') for byte in message))
    
    return binary_message

def decode_received_message(sock):
    try:
        # Receive 1 byte from the server
        first_byte = sock.recv(BYTES_FOR_CODE)  # Receives 1 byte
        
        if not first_byte:
            print("Error: No data received from server.")
            sys.exit(ERROR_IN_RUN)
            
        # convert the first byte into an int
        first_byte_int = int.from_bytes(first_byte, 'big')  # Convert the byte to an integer
        
        # Receive the next 4 bytes for JSON length
        json_length_bytes = sock.recv(BYTES_FOR_JSON_LENGTH)
        if not json_length_bytes:
            print("Error: No data received for the JSON length.")
            sys.exit(ERROR_IN_RUN)
        
        # Convert the 4 bytes to an integer (big-endian byte order)
        json_data_length = int.from_bytes(json_length_bytes, byteorder='big')  # convert the 4 bytes to an integer
        
        # Now, we know how many bytes to expect for the JSON body
        response_data = b""  # This will store the JSON body
        while len(response_data) < json_data_length:
            # Receive the remaining JSON data
            chunk = sock.recv(json_data_length - len(response_data))
            if not chunk:
                print("Error: No data received for the JSON body.")
                sys.exit(ERROR_IN_RUN)
            response_data += chunk

        # Decode and deserialize the JSON data
        response_json = response_data.decode('utf-8')
        response_data = json.loads(response_json)
        
        return create_protocol_message(first_byte_int, response_data), response_data
        
    except socket.error as err:
        print(f"Error during communication: {err}")
        sys.exit(ERROR_IN_RUN)

# Send the message to the server and receive response
def send_receive_message(sock, message):
    try:
        # Send the protocol message to the server
        sock.sendall(message.encode())
        response_full_message, response_data = decode_received_message(sock)
        print(f"JSON data: {response_data}")
        
    except socket.error as err:
        print(f"Error during communication: {err}")
        sys.exit(ERROR_IN_RUN)

# main function - runs the whole client flow
def main():
    
    validate_port(SERVER_PORT)
    client_socket = create_socket()
    connect_to_server(client_socket, SERVER_ADDRESS, SERVER_PORT)

    choice = user_login_or_signup()
    user_details = get_user_details(choice)
    protocol_message = create_protocol_message(choice, user_details)
    
    send_receive_message(client_socket, protocol_message)

    client_socket.close()
    print("Connection closed.")
    
    return SUCCESSFUL_RUN

# run the client
if __name__ == "__main__":
    main()