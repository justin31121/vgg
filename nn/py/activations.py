import numpy as np

def softmax(x):
    exp_values = np.exp(x - np.max(x, axis=1, keepdims=True))
    probabilities = exp_values / np.sum(exp_values, axis=1, keepdims=True)
    return probabilities

def softmax_prime(x):
    return softmax(x)*(1.0-softmax(x))

def tanh(x):
    return np.tanh(x)

def tanh_prime(x):
    return 1-np.tanh(x)**2

def relu(x):
    return np.maximum(0,x)

def relu_prime(x):
    return np.array(x>0, dtype=np.float32)

def softplus(x):
    return np.log1p(np.exp(x))

def softplus_prime(x):
    return 1 / (1 + np.exp(-x))
