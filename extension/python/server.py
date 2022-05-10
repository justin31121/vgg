from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib.request
from PIL import Image
from torchvision import transforms
import torch

hostname = "localhost"
serverport = 8020

model = None
categories = None

def download_file(url1):

    ext = ".jpg"
    if ".png" in url1:
        ext = ".png"
        
    url, filename = (url1, "sample"+ext)

    try:
        urllib.request.urlretrieve(url, filename)
    except e:
        try:
            response = urllib.request.urlopen(url)
            with open('image.jpg', 'wb') as f:
                f.write(response.file.read())
        except:
            return "", False

        return "image.jpg", True

    return filename, True


class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        url = self.headers.get('Url')

        mess = ""
        filename, success = download_file(url)
        if success:
            try:
                mess = guess(filename)
            except:
                mess = "server error"
            
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.end_headers()

        self.wfile.write(bytes(mess, "utf-8"))

def guess(filename):
    global model

    
    
    input_image = Image.open(filename)
    preprocess = transforms.Compose([
        transforms.Resize(256),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406],
                             std=[0.229, 0.224, 0.225]
                             )
    ])

    input_tensor = preprocess(input_image)
    input_batch = input_tensor.unsqueeze(0)

    

    if torch.cuda.is_available():
        input_batch = input_batch.to('cuda')

    output = None
    
    with torch.no_grad():
        output = model(input_batch)

    probabilities = torch.nn.functional.softmax(output[0], dim=0)

    top5_prob, top5_catid = torch.topk(probabilities, 5)

    mess = ""
    for i in range(top5_prob.size(0)):
        mess = mess +  "\n\n" + str(categories[top5_catid[i]]) + ": " + str(top5_prob[i].item())

    return mess

if __name__ == "__main__":
    model = torch.hub.load('pytorch/vision:v0.10.0', 'vgg11', pretrained=True)
    model.eval()

    if torch.cuda.is_available():
        print('cude')
        model.to('cuda')

    with open("imagenet_classes.txt", "r") as f:
        categories = [s.strip() for s in f.readlines()]
        
    webServer = HTTPServer((hostname, serverport), MyServer)
    print("Server started http://%s%s" % (hostname, serverport))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
