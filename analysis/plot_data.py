# import math
import re

from matplotlib import pyplot as plt
import pandas as pd
# import scipy
# import numpy as np


MAX_EVT = 1000
# MAX_EVT = None

plt.rcParams['figure.figsize'] = (8.0, 5.0)
# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


# ### Importing data with pandas
root = "."
fname = "{}\\all_output_small.txt".format(root)
data = pd.read_csv(fname, sep="\t", comment="#", index_col=False, engine="python",
                   header=0, nrows=MAX_EVT, na_values='-')
names = list(data.dtypes.index)[:-1]


def plot_raw(regex=''):
    p = re.compile(regex)
    fig, ax = plt.subplots()
    for name in names:
        if re.search(p, name):
            ax.plot(getattr(data, name), label=name)
    # ax.plot(data.lep_n, label="n leptons")
    # ax.plot(data.jet_DH_n, label="n jets DH")
    # ax.plot(data.jet_KT_R05_n, label="n jets KT5")
    ax.grid(True, which='both')
    ax.set(ylabel='Value', xlabel='Event')
    ax.legend(loc="best")
    ax.margins(x=0)
    fig.savefig("raw.pdf")


def plot_hist(regex='', xRange=None, nBins=30):
    p = re.compile(regex)
    fig, ax = plt.subplots()
    for name in names:
        if re.search(p, name):
            n, bins, patches = ax.hist(getattr(data, name).dropna(), nBins, normed=1, range=xRange, label=name)
    ax.grid(True, which='both')
    ax.set(ylabel='Entries', xlabel='Value')
    ax.legend(loc="best")
    ax.margins(x=0)
    fig.savefig("hist.pdf")


plot_raw('\w*_n')
plot_hist('\w*_n', (0, 10), 10)
plot_raw('lep_pt')
plot_hist('lep_pt', (0, 200))
