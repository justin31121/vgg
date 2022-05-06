import torch
import urllib
from PIL import Image
from torchvision import transforms

#pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113

if __name__ == "__main__":

    #download image
    url, filename = ("https://github.com/pytorch/hub/raw/master/images/dog.jpg", "dog.jpg")
    try: urllib.URLopener().retrieve(url, filename)
    except: urllib.request.urlretrieve(url, filename)

    #load model
    model = torch.hub.load('pytorch/vision:v0.10.0', 'vgg11', pretrained=True)
    model.eval()

    #sample
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
        print('cuda')
        input_batch = input_batch.to('cuda')
        model.to('cuda')

    with torch.no_grad():
        output = model(input_batch)

    #print(output[0])

    probabilities = torch.nn.functional.softmax(output[0], dim=0)
    #print(probabilities)

    with open("imagenet_classes.txt", "r") as f:
        categories = [s.strip() for s in f.readlines()]

    
    top5_prob, top5_catid = torch.topk(probabilities, 5)

    for i in range(top5_prob.size(0)):
        print(categories[top5_catid[i]], top5_prob[i].item())
