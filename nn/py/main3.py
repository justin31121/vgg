import numpy as np

from network import Network
from fclayer import FCLayer
from activationlayer import ActivationLayer
from activations import softmax, softmax_prime, tanh, tanh_prime
from losses import mse, mse_prime

if __name__ == "__main__":
    
    x_train = np.array([ [[0,0]], [[0,1]], [[1,0]], [[1,1]] ])
    y_train = np.array([ [[0]], [[0 ]], [[0]], [[1]] ])
    
    net = Network()
    net.add(FCLayer(2,3))
    net.add(ActivationLayer(tanh, tanh_prime))
    net.add(FCLayer(3,1))
    net.add(ActivationLayer(tanh, tanh_prime))
    
    out = net.predict(x_train)
    out = [[1.234749]]
    loss = mse_prime(out, y_train)
    print(out[0])
    print(loss[0])

