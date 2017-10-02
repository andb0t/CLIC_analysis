import matplotlib.pyplot as plt
import pandas as pd
from IPython import get_ipython

import styles


def run_from_ipython():
    try:
        __IPYTHON__
        return True
    except NameError:
        return False


# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
if run_from_ipython():
    get_ipython().magic('matplotlib inline')

plt.rcParams['figure.figsize'] = (5.0, 2.0)


def plot_corr(dataCont, colorbar=False, save=None):
    df = pd.DataFrame(dataCont.data)
    fig, ax = plt.subplots(figsize=(10, 10))
    cax = ax.matshow(df.corr(), origin='lower')
    if colorbar:
        fig.colorbar(cax)
    ax.set_title(dataCont.name + ' correlation')
    ax.xaxis.set_ticks_position('bottom')
    nBins = len(dataCont.names())
    ax.set_xlim(right=nBins - 0.5)
    ax.set_ylim(top=nBins - 0.3)
    ax.set_xticks(range(nBins))
    ax.set_yticks(range(nBins))
    ax.set_xticklabels(dataCont.names(), rotation='vertical', size='small')
    ax.set_yticklabels(dataCont.names(), size='small')
    if save:
        fig.savefig(save)


def plot_raw(dataCont, regex='', save=None):
    fig, ax = plt.subplots()
    for cont in dataCont:
        for name in cont.names(regex):
            if cont.name:
                legendName = cont.name + ' ' + name
            else:
                legendName = name
            ax.plot(cont.get(name), label=legendName, marker='.')
    ax.set(ylabel='Value', xlabel='Event')
    styles.style_raw(ax)
    if save:
        fig.savefig(save)


def plot_hist(dataCont, regex='', xRange=None, nBins=30, stacked=False, chained=False, save=None, normed=1):
    fig, ax = plt.subplots()
    if stacked:
        data = []
        labels = []
        for cont in dataCont:
            contLabels = []
            if chained:
                data.extend(cont.get_chained(regex))
                contLabels.extend(regex)
            else:
                data.extend(cont.get_list(regex))
                contLabels.extend(cont.names(regex))
            if cont.name:
                labels.extend(map(lambda x: cont.name + ' ' + x, contLabels))
        ax.hist(data, nBins, normed=normed, range=xRange,
                label=labels, stacked=True)
    else:
        for cont in dataCont:
            for name in cont.names(regex):
                if cont.name:
                    legendName = cont.name + ' ' + name
                else:
                    legendName = name
                ax.hist(cont.get(name), nBins, normed=normed, range=xRange,
                        label=legendName, stacked=False)
    ax.set(ylabel='Entries', xlabel='Value')
    styles.style_hist(ax)
    if save:
        fig.savefig(save)
