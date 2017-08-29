# import math

from matplotlib import pyplot as plt
import pandas as pd
# import scipy
# import numpy as np


plt.rcParams['figure.figsize'] = (8.0, 8.0)

# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


# ### Importing data with pandas

# path to the cernbox with HexDAQ data in it
root = "."

fname = "{}\\all_output_small.txt".format(root)
data = pd.read_csv(fname, sep="\t", comment="#", index_col=False, engine="python", header=0)
names = list(data.dtypes.index)[:-1]


def plot_raw(suffix=''):
    fig, ax = plt.subplots()
    for name in names:
        if name.find(suffix) != -1:
            ax.plot(getattr(data, name), label=name)
    # ax.plot(data.lep_n, label="n leptons")
    # ax.plot(data.jet_DH_n, label="n jets DH")
    # ax.plot(data.jet_KT_R05_n, label="n jets KT5")
    ax.grid(True, which='both')
    ax.set(ylabel='Value', xlabel='Event')
    ax.legend(loc="best")
    ax.margins(x=0)
    fig.savefig("raw.pdf")


plot_raw('_n')


def plot_hist(suffix=''):
    fig, ax = plt.subplots()
    num_bins = 10
    for name in names:
        if name.find(suffix) != -1:
            n, bins, patches = ax.hist(getattr(data, name), num_bins, normed=1, range=(0, 10), label=name)
    ax.grid(True, which='both')
    ax.set(ylabel='Entries', xlabel='Value')
    ax.legend(loc="best")
    ax.margins(x=0)
    fig.savefig("hist.pdf")


plot_hist('_n')
