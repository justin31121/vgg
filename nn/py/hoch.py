import torch
import torch.nn as nn
import matplotlib.pyplot as plt

X = torch.tensor([range(-10,11)]).float().view(-1, 1)
Y = X*X

model = nn.Sequential(
    nn.Linear(1, 10),
    nn.Tanh(),
    nn.Linear(10, 1)
)

for param in model[0].parameters():
    print(param)

for param in model[2].parameters():
    print(param)

"""
optimizer = torch.optim.SGD(model.parameters(), lr=1e-3)
loss_func = nn.MSELoss()
for _ in range(15000):
    optimizer.zero_grad()
    pred = model(X)
    loss = loss_func(pred, Y)
    loss.backward()
    optimizer.step()

x = torch.linspace(-12, 12, steps=200).view(-1, 1)
y = model(x)
f = x*x

plt.plot(x.detach().view(-1).numpy(), y.detach().view(-1).numpy(), 'r.', linestyle='None')
plt.plot(x.detach().view(-1).numpy(), f.detach().view(-1).numpy(), 'b')
plt.show()
"""
