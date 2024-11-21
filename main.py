import socket

# protocol = socket.SOCK_STREAM # TCP
protocol = socket.SOCK_DGRAM # UDP
server = '10.136.12.124'# 10.136.12.124
port = 8080

s = socket.socket(socket.AF_INET, protocol)
if protocol == socket.SOCK_STREAM:
    s.connect((server, port))

while True:
    message = input('Enter your message: ')
    if protocol == socket.SOCK_DGRAM:
        s.sendto(message.encode(), (server, port))
        if message == 'q':
            break
        response, addr = s.recvfrom(1024)
        print('server: '+response.decode())
        continue
    s.send(message.encode())
    if message == 'q':
        break
    response = s.recv(1024)
    print('server: '+response.decode())


s.close()