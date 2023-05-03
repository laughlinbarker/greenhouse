import time
import zmq

context = zmq.Context()
socket = context.socket(zmq.PULL)
socket.bind("tcp://*:1234")

while True:
    #  Wait for next request from client
    message = socket.recv()
    print("Received request: %s" % message)
