import json
import time
from datetime import datetime
import zmq

context = zmq.Context()
socket = context.socket(zmq.PULL)
socket.bind("tcp://*:7753")

while True:
    #  Wait for next request from client
    message = socket.recv()
    current_datetime = datetime.now()
    current_date_time = current_datetime.strftime("%m/%d/%Y, %H:%M:%S")
    data_dict = json.loads(message.decode('ascii'))
    print("Recieved: %s" % current_date_time)
    print(data_dict)
    print('\n')
    #print("Received request: %s" % message)
