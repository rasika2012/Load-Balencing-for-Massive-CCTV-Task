# import socket
# serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# serv.bind(('0.0.0.0', 8080))
# serv.listen(5)
# while True:
#     conn, addr = serv.accept()
#     from_client = ''
#     while True:
#         data = conn.recv(4096)
#         if not data: break
#         from_client += data
#         print from_client
#         conn.send("I am SERVER<br>")
#     conn.close()
#     print 'client disconnected'
import socket
import time
import os
def fun():
    time.sleep(2)
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # port = int(input("PORT : "))
    client.connect(('192.168.80.3', 4090))

    while(True):
        from_server = client.recv(4096)
        if not from_server: break
        client.send(from_server)
    # send over
    print("send over")
    client.close()
connections = int(input("Num of connection : "))
fun()
# connections = int(input("Num of connection : "))
# while(i<connections):  
#     i = i + 1
#     try:
#         n = os.fork()
#         print(n) 
#         if(n == 0):
#             fun()
#             break        
#     except:
#         print("Error: unable to start thread")
#     if(i>connections):
#         print("stop")
#         break