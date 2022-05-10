import torch
import urllib.request
from PIL import Image
from torchvision import transforms

model = None
categories = None

def guess():
    global model
    
    input_image = Image.open("sample.jpg")
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

    for i in range(top5_prob.size(0)):
        print(categories[top5_catid[i]], top5_prob[i].item())

def download(url1):

    ext = ".jpg"
    if ".png" in url1:
        ext = ".png"
    
    url, filename = (url1, "sample"+ext)

    try:
        urllib.request.urlretrieve(url, filename)
    except e:
        return "Can not download img", False

    return "", True
    
    
if __name__ == "__main__":
    model = torch.hub.load('pytorch/vision:v0.10.0', 'vgg11', pretrained=True)
    model.eval()

    if torch.cuda.is_available():
        print('cude')
        model.to('cuda')

    with open("imagenet_classes.txt", "r") as f:
        categories = [s.strip() for s in f.readlines()]
                                       
    while True:
        inp = input()
        
        if inp == "q":
            break

        mess, success = download(inp)
        if success:
            guess()
        else:
            print(mess)
