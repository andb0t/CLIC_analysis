import matplotlib.pyplot as plt
# import numpy as np
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


def plot_corr(dataCont, colorbar=True, save=None):
    nBins = len(dataCont.names())
    df = pd.DataFrame(dataCont.data)
    # mask = np.ones(df.shape, dtype='bool')
    # mask[np.triu_indices(nBins + 1)] = False
    # df = filter(lambda:, df)
    # print(df)
    # print(mask)
    # return

    fig, ax = plt.subplots(figsize=(10, 10))
    cax = ax.matshow(df.corr(), origin='lower')
    if colorbar:
        fig.colorbar(cax)
    ax.set_title(dataCont.name + ' correlation')
    ax.xaxis.set_ticks_position('bottom')
    ax.set_xlim(right=nBins - 0.5)  # use to restrict range. Check why dataframe has one last col/row with nan
    ax.set_ylim(top=nBins - 0.3)  # use to restrict range. Check why dataframe has one last col/row with nan
    ax.set_xticks(range(nBins))
    ax.set_yticks(range(nBins))
    ax.set_xticklabels(dataCont.names(), rotation='vertical', size='small')
    ax.set_yticklabels(dataCont.names(), size='small')
    if save:
        fig.savefig(save)


def plot_raw(dataCont, regex='', save=None, ylabel='Value', xlabel='Event', nolegname=False):
    fig, ax = plt.subplots()
    for cont in dataCont:
        for name in cont.names(regex):
            legendName = ''
            if cont.name:
                legendName += cont.name + ' '
            if nolegname:
                legendName = legendName.rstrip()
            else:
                legendName += name
            ax.plot(cont.get(name), label=legendName, marker='.')
    ax.set(ylabel=ylabel, xlabel=xlabel)
    styles.style_raw(ax)
    if save:
        fig.savefig(save)


def plot_hist(dataCont,
              regex='', xRange=None, nBins=30, stacked=False, chained=False, save=None, normed=1,
              ylabel='Entries', xlabel='Value', nolegname=False):
    fig, ax = plt.subplots()
    if stacked:
        data = []
        legendNames = []
        for cont in dataCont:
            contLabels = []
            if chained:
                data.extend(cont.get_chained(regex))
                contLabels.extend(' ' + regex)
            else:
                data.extend(cont.get_list(regex))
                contLabels.extend(map(lambda x: ' ' + x, cont.names(regex)))
            if nolegname:
                contLabels = map(lambda x: x*0, contLabels)
            if cont.name:
                legendNames.extend(map(lambda x: cont.name + x, contLabels))

        ax.hist(data, nBins, normed=normed, range=xRange,
                label=legendNames, stacked=True)
    else:
        for cont in dataCont:
            alpha = 1
            print(type(dataCont))
            print(dataCont)
            if len(dataCont) > 1:
                alpha = 0.5
            for name in cont.names(regex):
                legendName = ''
                if cont.name:
                    legendName += cont.name + ' '
                if nolegname:
                    legendName = legendName.rstrip()
                else:
                    legendName += name
                ax.hist(cont.get(name), nBins, normed=normed, range=xRange,
                        label=legendName, stacked=False, alpha=alpha)
    ax.set(ylabel=ylabel, xlabel=xlabel)
    styles.style_hist(ax)
    if save:
        fig.savefig(save)
