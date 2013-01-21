# Echo client program
import socket
import struct

text = ''
#try read file
f = open("dohttp.py", "r")
text = f.read()

#try json
import json

param = dict()
param["rs"] = 1
#try insert new task ==1 query task result ==2
param["ri"] = 1
param["i"] = 7
#try insert task 
#param["ri"] = 1
param["n"] = "test 1+2"
param["bt"] = "2012-07-10 20:43:12"
param["et"] = "2013-07-10 20:43:13"
param["iv"] = 1
param["c"] = text

text = json.dumps(param)


format = "i%ds" % len(text)
datasize = len(text)
packed_data = struct.pack(format,datasize, text)



host = '127.0.0.1'                 # Symbolic name meaning all available interfaces
port = 5568           # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect( (host,port) )
# test send
s.send(packed_data)

recvHead = s.recv(4)
recvSize,= struct.unpack("i",recvHead)

buffer = s.recv(recvSize-4)
print(buffer)
result = json.loads(buffer)

print(result["rc"])
print(result["ri"])





s.close()

