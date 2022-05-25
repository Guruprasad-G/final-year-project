
import socket
mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
mysock.connect(('192.168.4.1', 80))
cmd = 'GET 192.168.4.1 HTTP/1.0\r\n\r\n'.encode()
mysock.send(cmd)
while True:
    data = mysock.recv(20)
    if (len(data) < 1):
        break
    print(data.decode(),end='')
mysock.close()








'''from time import sleep
import urllib.request

home_page = urllib.request.urlopen('192.168.4.1')
current_node_page = urllib.request.urlopen('192.168.4.1/current_node')
current_orientation_page = urllib.request.urlopen('192.168.4.1/current_orientation')
servo_position_page = urllib.request.urlopen('192.168.4.1/servo_position')
distance_page = urllib.request.urlopen('192.168.4.1/distance')

print("Home page =====>>>>>")
for line in home_page:
    print(line.decode().strip())

while True:
    print("Current Node =====>>>>>")
    for line in current_node_page:
        print(line.decode().strip())
    print("Current Orientation =====>>>>>")
    for line in current_orientation_page:
        print(line.decode().strip())
    print("Servo Postion =====>>>>>")
    for line in servo_postion_page:
        print(line.decode().strip())
    print("Ultra Sound Distance =====>>>>>")
    for line in distance_page:
        print(line.decode().strip())
    sleep(3)'''
        


































