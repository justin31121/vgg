import numpy as np

from network import Network
from fclayer import FCLayer
from activationlayer import ActivationLayer
from activations import tanh, tanh_prime, softmax, softmax_prime
from losses import mse, mse_prime

from tensorflow.keras.datasets import mnist
from keras.utils import np_utils

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_train = x_train.reshape(x_train.shape[0], 1, 28*28)
x_train = x_train.astype('float32')
x_train /= 255

y_train = np_utils.to_categorical(y_train)

x_test = x_test.reshape(x_test.shape[0], 1, 28*28)
x_test = x_test.astype('float32')
x_test /= 255

y_test = np_utils.to_categorical(y_test)

net = Network()
net.add(FCLayer(28*28, 1000))
net.add(ActivationLayer(tanh, tanh_prime))
net.add(FCLayer(1000, 50))
net.add(ActivationLayer(tanh, tanh_prime))
net.add(FCLayer(50, 10))
net.add(ActivationLayer(softmax, softmax_prime))

net.use(mse, mse_prime)
net.fit(x_train[0:1000], y_train[0:1000], epochs=35, learning_rate=0.1)

out = net.predict(x_test[0:3])

print(out)
print("========")
print(y_test[0:3])
