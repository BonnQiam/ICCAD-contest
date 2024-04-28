import numpy as np
import matplotlib.pyplot as plt

data = []
with open('sM1.txt', 'r') as file:
    lines = file.readlines()
    for line in lines:
        split_line = line.split()
        # get the third column
        data.append(float(split_line[2]))
        
data = np.array(data).reshape(8, 20)

# plot data
plt.imshow(data, cmap='hot', interpolation='nearest')
plt.colorbar()
plt.show()