


def train_network(net, x_train, y_train, epochs):

    #Training-Parameter
    learning_rate = 0.001
    momentum = 0.9

    #Loss function
    criterion = nn.CrossEntropyLoss()
    #Optimizer
    optimizer = optim.SGD(net.parameters(),
                          lr=learning_rate, momentum=momentum)
    
    for epoch in range(epochs):

        #reset the gradients in net
        optimizer.zero_grad()

        #forward
        outputs = net(x_train)

        #calculate loss
        loss = criterion(outputs, y_train)
        #calculate gradients in net
        loss.backward()

        #backward
        optimizer.step()






        
