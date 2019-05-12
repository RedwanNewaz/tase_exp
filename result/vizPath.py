import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


if __name__ == '__main__':
    data = pd.read_csv('log.csv')
    x = np.array(data)
    plt.plot(x[:,0],x[:,1])
    plt.grid()
    plt.axis([0,4,0,4])
    plt.show()