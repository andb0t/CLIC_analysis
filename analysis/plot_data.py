# import math
import re
import itertools

from matplotlib import pyplot as plt
# import scipy
# import numpy as np

import containers


MAX_EVT = 1000
# MAX_EVT = None

plt.rcParams['figure.figsize'] = (8.0, 5.0)
# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


# ### Importing data with pandas
fname = "all_output_small.txt"
physCont = containers.physics_container(fname, MAX_EVT)
physCont.show()


def plot_raw(regex='', save=None):
    p = re.compile(regex)
    fig, ax = plt.subplots()
    labels = [name for name in physCont.names if re.search(p, name)]
    for name in labels:
        if re.search(p, name):
            ax.plot(physCont.get(name), label=name)
    # ax.plot(data.lep_n, label="n leptons")
    # ax.plot(data.jet_DH_n, label="n jets DH")
    # ax.plot(data.jet_KT_R05_n, label="n jets KT5")
    ax.grid(True, which='both')
    ax.set(ylabel='Value', xlabel='Event')
    ax.legend(loc="best")
    ax.margins(x=0)
    if save:
        fig.savefig(save)


def plot_hist(regex='', xRange=None, nBins=30, stacked=False, compressed=False, save=None):
    p = re.compile(regex)
    fig, ax = plt.subplots()
    labels = [name for name in physCont.names if re.search(p, name)]
    if stacked:
        if compressed:
            ax.hist(list(itertools.chain.from_iterable([physCont.get(name).dropna() for name in labels])),
                    nBins, normed=1, range=xRange, label=regex, stacked=True)
        else:
            ax.hist([physCont.get(name).dropna() for name in labels],
                    nBins, normed=1, range=xRange, label=labels, stacked=True)
    else:
        for name in labels:
            ax.hist(physCont.get(name).dropna(), nBins, normed=1, range=xRange, label=name, stacked=False)
    ax.grid(True, which='both')
    ax.set(ylabel='Entries', xlabel='Value')
    ax.legend(loc="best")
    ax.margins(x=0)
    if save:
        fig.savefig(save)


plot_raw('\w*_n')
plot_hist('\w*_n', (0, 10), 10)
plot_raw('lep_pt')
plot_hist('lep_pt', (0, 200))
plot_hist('lep_pt_[^4]', (0, 20), stacked=True)
plot_hist('jet_DH_pt', (0, 200), stacked=True)
plot_hist('jet_DH_pt', (0, 200), stacked=True, compressed=True)
plot_hist('lep_pt', (0, 200), stacked=True, compressed=True)
