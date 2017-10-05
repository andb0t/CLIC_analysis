import matplotlib.pyplot as plt
from IPython import get_ipython

from src.form import styles
from src import settings


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
    fig, ax = plt.subplots(figsize=(10, 10))
    cax = ax.matshow(dataCont.data.corr(), origin='lower')
    if colorbar:
        fig.colorbar(cax)
    ax.set_title(dataCont.name + ' correlation')
    ax.xaxis.set_ticks_position('bottom')
    # ax.set_xlim(right=nBins - 0.5)  # use to restrict range. Check why dataframe has one last col/row with nan
    # ax.set_ylim(top=nBins - 0.3)  # use to restrict range. Check why dataframe has one last col/row with nan
    ax.set_xlim(right=nBins + 1)
    ax.set_ylim(top=nBins + 1)
    ax.set_xticks(range(nBins + 1))
    ax.set_yticks(range(nBins + 1))
    ax.set_xticklabels(dataCont.names(), rotation='vertical', size='small')
    ax.set_yticklabels(dataCont.names(), size='small')
    if save:
        fig.tight_layout()
        fig.savefig(settings.PLOT_DIR + save)


def plot_raw(dataCont, regex='', save=None, ylabel='Value', xlabel='Event', noLegName=False):
    fig, ax = plt.subplots()
    validCont = (cont for cont in dataCont if cont.data.shape[0] > 0)
    for cont in validCont:
        for name in cont.names(regex):
            legendName = ''
            if cont.name:
                legendName += cont.name + ' '
            if noLegName:
                legendName = legendName.rstrip()
            else:
                legendName += name
            ax.plot(cont.get(name), label=legendName, marker='.')
    ax.set(ylabel=ylabel, xlabel=xlabel)
    styles.style_raw(ax)
    if save:
        fig.tight_layout()
        fig.savefig(settings.PLOT_DIR + save)


def plot_hist(dataCont,
              regex='', xRange=None, nBins=30, stacked=False, chained=False, save=None, normed=1,
              ylabel='Entries', xlabel='Value', noLegName=False):
    fig, ax = plt.subplots()
    validCont = (cont for cont in dataCont if cont.data.shape[0] > 0)
    if stacked:
        data = []
        legendNames = []
        for cont in validCont:
            contLabels = []
            if chained:
                data.extend(cont.get_chained(regex))
                contLabels.extend(' ' + regex)
            else:
                data.extend(cont.get_list(regex))
                contLabels.extend(map(lambda x: ' ' + x, cont.names(regex)))
            if noLegName:
                contLabels = map(lambda x: x*0, contLabels)
            if cont.name:
                legendNames.extend(map(lambda x: cont.name + x, contLabels))

        ax.hist(data, nBins, normed=normed, range=xRange,
                label=legendNames, stacked=True)
    else:
        nHist = 0
        for cont in validCont:
            alpha = 1
            for name in cont.names(regex):
                if nHist:
                    alpha = 0.5
                legendName = ''
                if cont.name:
                    legendName += cont.name + ' '
                if noLegName:
                    legendName = legendName.rstrip()
                else:
                    legendName += name
                ax.hist(cont.get(name), nBins, normed=normed, range=xRange,
                        label=legendName, stacked=False, alpha=alpha)
                nHist += 1
    ax.set(ylabel=ylabel, xlabel=xlabel)
    styles.style_hist(ax)
    if save:
        fig.tight_layout()
        fig.savefig(settings.PLOT_DIR + save)
