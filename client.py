# libraries
import json
import socket
# import threading
# modules: db, requests
from modules.brain import Brain
from modules.db import DB

print("*** Lambda IoT Client ***")

# make the brain instance
brain = Brain()

#####################################################
################ Establish Connection ###############
#####################################################

# instance the db
db = DB('db/')
# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# try to connect the server
try:
    # Connect to the server
    client_socket.connect((db['host'], db['port']))
except:
    raise OSError("Connection Error: connection failed")

#####################################################
################ User Initialization ################
#####################################################

# this will regist the user on the server
client_socket.send(f'+{db["id"]}'.encode('utf-8'))
# and this will regist the user devices
client_socket.send(f'*{db["id"]}*{json.dumps(db["devices"])}'.encode('utf-8'))

#####################################################
################# Message Receiving #################
#####################################################


# Receive messages from the server
while True:
    # Receive a message from the server
    message = client_socket.recv(1024).decode('utf-8')
    
    # if there is an empty message 
    if not message:
        continue

    # Print the message
    print(f'\n[incoming] {message}')

    # if the message start is correct
    if message[0] == '-':
        print("procesando mensaje...")
        answer = ""
        # try to use the brain
        try:
            # get the answer directly from the arduino
            answer = brain(message)
        except:
            # HERE YOU DEFINE YOUR ERROR ANSWER
            answer = f"Lo siento <@{db['id']}> algo salió mal jajajaj"

        # finally struct the answer and send it 
        answer_message = f'-{db["id"]}-lambda-{answer}'
        print("[outgoing]", answer_message, '\n')
        client_socket.send(answer_message.encode('utf-8'))

# Close the connection
client_socket.close()