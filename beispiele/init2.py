import torch.nn as nn

class Vgg_net(nn.Module):
    def __init__(self):
        super(Vgg_net, self).__init__()
        self.in_channels = 3
        self.num_classes = 1000

        #convolutional layers
        vgg16 = [
            [64, 64], [128, 128], [256, 256, 256], [512, 512, 512], [512, 512, 512]
        ]
        layers = []
        
        for block in vgg16:
            for layer in block:
                out_channels = layer

                layers += [
                    nn.Conv2d(in_channels=in_channels, out_channels=out_channels,
                              kernel_size=(3,3), stride=(1,1), padding=(1,1)
                              )
                    nn.ReLU()
                ]

            layers += [nn.MaxPool2d(kernel_size(2,2), stride=(2,2))]

        self.convolutional_layers = nn.Sequential(layers)

        #fully_connected layers

        
