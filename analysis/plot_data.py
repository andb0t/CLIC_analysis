import importlib

from matplotlib import pyplot as plt

import containers
import styles


MAX_EVT = 5
# MAX_EVT = None

plt.rcParams['figure.figsize'] = (8.0, 5.0)
# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


importlib.reload(containers)
fname = "all_output_small.txt"
physCont = containers.physics_container(fname, MAX_EVT, verbose=2)
# physCont.show()


def plot_raw(regex='', save=None):
    fig, ax = plt.subplots()
    for name in physCont.names(regex):
        ax.plot(physCont.get(name), label=name)
    ax.set(ylabel='Value', xlabel='Event')
    styles.style(ax)
    if save:
        fig.savefig(save)


def plot_hist(regex='', xRange=None, nBins=30, stacked=False, chained=False, save=None):
    fig, ax = plt.subplots()
    if stacked:
        if chained:
            ax.hist(physCont.get_chained(regex), nBins, normed=1, range=xRange, label=regex, stacked=True)
        else:
            ax.hist(physCont.get_list(regex), nBins, normed=1, range=xRange, label=physCont.names(regex), stacked=True)
    else:
        for name in physCont.names(regex):
            ax.hist(physCont.get(name), nBins, normed=1, range=xRange, label=name, stacked=False)
    ax.set(ylabel='Entries', xlabel='Value')
    styles.style(ax)
    if save:
        fig.savefig(save)


plot_raw('lep_pt_mean')
plot_raw('lep_n')
plot_raw('\w*_n')
plot_hist('\w*_n', (0, 10), 10)
plot_hist('lep_pt', (0, 300))
plot_hist('lep_pt', (0, 300), stacked=True)
plot_hist('lep_pt', (0, 300), stacked=True, chained=True)
