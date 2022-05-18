


import torch

if __name__ == "__main__":
    net = torch.hub.load('pytorch/vision:v0.10.0', 'vgg16', pretrained=True)
    print(net)




