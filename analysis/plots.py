from matplotlib import pyplot as plt

import styles

plt.rcParams['figure.figsize'] = (8.0, 5.0)


def plot_raw(physCont, regex='', save=None):
    fig, ax = plt.subplots()
    for name in physCont.names(regex):
        ax.plot(physCont.get(name), label=name, marker='.')
    ax.set(ylabel='Value', xlabel='Event')
    styles.style_raw(ax)
    if save:
        fig.savefig(save)


def plot_hist(physCont, regex='', xRange=None, nBins=30, stacked=False, chained=False, save=None):
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
    styles.style_hist(ax)
    if save:
        fig.savefig(save)
