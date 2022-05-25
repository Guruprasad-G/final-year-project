# Python 3 server example
'''
# Import libraries
import sys
import http.server
import socketserver

# Creating handle
HandlerClass = http.server.SimpleHTTPRequestHandler

# Creating Server
ServerClass = http.server.HTTPServer

# Defining protocol
Protocol	 = "HTTP/1.0"

# Setting TCP Address
if sys.argv[1:]:
	port = int(sys.argv[1])
else:
	port = 8000
server_address = ('192.168.43.123', port)

# invoking server
HandlerClass.protocol_version = Protocol
http = ServerClass(server_address, HandlerClass)

# Getting logs
sa = http.socket.getsockname()
print("Serving HTTP on", sa[0], "port", sa[1], "...")
http.serve_forever()
'''

from http.server import BaseHTTPRequestHandler, HTTPServer
import time

hostName = "192.168.43.123"
serverPort = 8000

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("Hello", "utf-8"))

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")

