from matplotlib import pyplot as plt

import styles

plt.rcParams['figure.figsize'] = (5.0, 3.0)


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
