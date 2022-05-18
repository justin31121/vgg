from init import VGG_net
from PIL import Image
from torchvision import transforms
import torch


def pre_process(image):
    preprocess = transforms.Compose([
        transforms.Resize(256),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])

    input_tensor = preprocess(image)
    return input_tensor.unsqueeze(0)

def loadCategories(path):
    with open("imagenet_classes.txt", "r") as f:
        categories = [s.strip() for s in f.readlines()]

    return categories

def findHighestProb(probs, categories):
    prob, catid =  torch.topk(probs, 1)

    return categories[catid[0]], prob[0].item()























#get network
net = net = torch.hub.load('pytorch/vision:v0.10.0', 'vgg16', pretrained=True)

#get categories
categories = loadCategories('imagenet_classes.txt')

#get image
image = Image.open('dog1.jpg')




#preprocess image
input_image = pre_process(image)

#forward
with torch.no_grad():
    output = net(input_image)



#apply softmax
probs = torch.nn.functional.softmax(output[0], dim=0)

#result
guess, guess_prob = findHighestProb(probs, categories)
print()
print(guess, guess_prob)

