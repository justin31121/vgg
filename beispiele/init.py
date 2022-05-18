
import torch.nn as nn

vgg16 = [[64, 64], [128, 128], [256, 256, 256], [512, 512, 512], [512, 512, 512]]
vgg19 = [[64, 64], [128, 128], [256, 256, 256, 256], [512, 512, 512, 512], [512, 512, 512, 512]]

class VGG_net(nn.Module):
    def __init__(self, in_channels=3, num_classes=1000, architecture=vgg16):
        super(VGG_net, self).__init__()

        #Convolutional Layers
        self.conv_layers = self.create_conv_layers(in_channels, architecture)

        #Fully Connected Layers
        self.fully_connected_layers = self.create_fully_connected_layers(num_classes)

    def forward(self, x):
        # x = [2, 3, 224, 224]
        
        x = self.conv_layers(x)
        # x = [2, 512, 7, 7]
        
        x = x.reshape(x.shape[0], -1)
        # x = [[1 .. (512x7x7)], [1 .. (512x7x7)]]
        
        x = self.fully_connected_layers(x)
        # x = [[1 .. 1000], [1 .. 1000]]
        
        return x

    def create_fully_connected_layers(self, num_classes):
        fully_connected_layers = nn.Sequential(
            nn.Linear(512*7*7, 4096),
            nn.ReLU(),
            nn.Dropout(p=0.5),
            nn.Linear(4096, 4096),
            nn.ReLU(),
            nn.Dropout(p=0.5),
            nn.Linear(4096, num_classes),
            nn.Softmax(dim=0)
        )

        return fully_connected_layers

    def create_conv_layers(self, in_channels, architecture):
        layers = []
        
        #[[64, 64], [128, 128], [256, 256, 256], [512, 512, 512], [512, 512, 512]]
        for block in architecture:
            #[64, 64]
            for layer in block:
                out_channels = layer

                #conv-layer + relu
                layers += [
                    nn.Conv2d(in_channels=in_channels, out_channels=out_channels,
                              kernel_size=(3,3), stride=(1,1), padding=(1,1)),
                    nn.ReLU()
                ]

                in_channels = out_channels

            #max-pooling
            layers += [nn.MaxPool2d(kernel_size=(2,2), stride=(2,2))]
        return nn.Sequential(*layers)

if __name__ == "__main__":
    net = VGG_net(in_channels=3, num_classes=1000, architecture=vgg16)
    print(net)






    
"""    def forward(self, x):
        #      n  (rgb)  w    h
        # x = [2, 3,     224, 224]
        
        x = self.conv_layers(x)
        # x = [2, 512, 7, 7]

        
        x = x.reshape(x.shape[0], -1)
        # x = [[1 .. (512x7x7)], [1 .. (512x7x7)]]

        
        x = self.fully_connected_layers(x)
        # x = [[1 .. 1000], [1 .. 1000]]
        
        return x
"""



