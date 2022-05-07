from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib.request

hostname = "localhost"
serverport = 8020

def download_file(url, filename):
    try: urllib.URLopener().retrieve(url, filename)
    except: urllib.request.urlretrieve(url, filename)

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        url = self.headers.get('Url')
        
        print(url)
        download_file(url,'sample.jpg')
        
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.end_headers()

        self.wfile.write(bytes("hello, from python server", "utf-8"))
        

if __name__ == "__main__":
    webServer = HTTPServer((hostname, serverport), MyServer)
    print("Server started http://%s%s" % (hostname, serverport))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
