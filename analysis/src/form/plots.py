import matplotlib.pyplot as plt
import numpy as np
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


class plots:
    def __init__(self, savePrefix='', noLegName=False, savePlots=True):
        self.savePrefix = settings.PLOT_DIR + savePrefix + '_'
        self.noLegName = noLegName
        self.savePlots = savePlots

    def save_plot(self, save, fig):
        if save and self.savePlots:
            fig.tight_layout()
            fig.savefig(self.savePrefix + save)
            plt.close(fig)
            print('Created', self.savePrefix + save)

    def plot_corr(self, dataCont, colorbar=True, save=None):
        nBins = len(dataCont.names())
        fig, ax = plt.subplots(figsize=(10, 10))
        cax = ax.matshow(dataCont.df.corr(), origin='lower')
        if colorbar:
            fig.colorbar(cax)
        ax.set_title(dataCont.name + ' correlation')
        ax.xaxis.set_ticks_position('bottom')
        ax.set_xticks(range(nBins))
        ax.set_yticks(range(nBins))
        ax.set_xticklabels(dataCont.names(), rotation='vertical', size='small')
        ax.set_yticklabels(dataCont.names(), size='small')
        self.save_plot(save, fig)

    def plot_raw(self, dataCont, regex='', save=None, ylabel='Value', xlabel='Event'):
        fig, ax = plt.subplots()
        validCont = [cont for cont in dataCont if cont.df.shape[0] > 0]
        for cont in validCont:
            for name in cont.names(regex):
                legendName = ''
                if cont.name:
                    legendName += cont.name + ' '
                if self.noLegName:
                    legendName = legendName.rstrip()
                else:
                    legendName += name
                data = cont.get(name)['data']
                ax.plot(data, label=legendName, marker='.')
        ax.set(ylabel=ylabel, xlabel=xlabel)
        styles.style_raw(ax)
        self.save_plot(save, fig)

    def plot_hist(self, dataCont,
                  regex='', xRange=None, nBins=30, mode=None, save=None, normed=0,
                  weighted=True, ylabel='auto', xlabel='Value'):
        fig, ax = plt.subplots()
        validCont = [cont for cont in dataCont if cont.df.shape[0] > 0]

        if mode == 'allstacked':
            data = []
            weights = []
            legendNames = []
            for cont in validCont:
                dataDict = cont.get_stacked(regex)
                data.extend(dataDict['data'])
                weights.extend(dataDict['weights'])
                contLabels = cont.names(regex)
                contLabels.extend(map(lambda x: ' ' + x, contLabels))
                if self.noLegName:
                    contLabels = map(lambda x: x*0, contLabels)
                if cont.name:
                    legendNames.extend(map(lambda x: cont.name + x, contLabels))
            if not weighted:
                weights = None
            ax.hist(data, nBins, weights=weights, normed=normed, range=xRange, label=legendNames, stacked=True)

        elif mode == 'stacked':
            data = []
            weights = []
            legendNames = []
            for cont in validCont:
                dataDict = cont.get_chained(regex)
                data.append(dataDict['data'])
                weights.append(dataDict['weights'])
                contLabels = ' ' + regex
                if self.noLegName:
                    contLabels = ''
                if cont.name:
                    legendNames.append(cont.name + contLabels)
            if not weighted:
                weights = None
            ax.hist(data, nBins, weights=weights, normed=normed, range=xRange, label=legendNames, stacked=True)

        elif mode == 'chained':
            nHist = 0
            for cont in validCont:
                alpha = 1
                if nHist:
                    alpha = 0.5
                dataDict = cont.get_chained(regex)
                data = dataDict['data']
                weights = dataDict['weights']
                contLabels = ' ' + regex
                if self.noLegName:
                    contLabels = map(lambda x: x*0, contLabels)
                if cont.name:
                    legendNames = map(lambda x: cont.name + x, contLabels)
                if not weighted:
                    weights = None
                ax.hist(data, nBins, weights=weights, normed=normed, range=xRange, label=legendNames, stacked=True, alpha=alpha)
                nHist += 1

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
                    if self.noLegName:
                        legendName = legendName.rstrip()
                    else:
                        legendName += name
                    dataDict = cont.get(name)
                    data = dataDict['data']
                    weights = dataDict['weights']
                    if not weighted:
                        weights = None
                    ax.hist(data, nBins, weights=weights, normed=normed, range=xRange, label=legendName, stacked=False, alpha=alpha)
                    nHist += 1

        ylabel = self.get_hist_ylabel(validCont, regex, ylabel, weighted)
        ax.set(ylabel=ylabel, xlabel=xlabel)
        styles.style_hist(ax)
        self.save_plot(save, fig)


    def get_hist_ylabel(self, validCont, regex, ylabel, weighted):
        if ylabel == 'auto':
            if not weighted:
                return 'Entries'
            ylabel = 'Events'
            for cont in validCont:
                contLabels = cont.names(regex)
                if len(contLabels) > 1:
                    return 'Entries'
        return ylabel


    def plot_scatter(self, dataCont,
                  regexX='', regexY='', xRange=None, yRange=None, mode=None, save=None, normed=0,
                  xlabel='Value 0', ylabel='Value 1', zlabel='Entries'):
        fig, ax = plt.subplots()
        validCont = [cont for cont in dataCont if cont.df.shape[0] > 0]
        nHist = 0
        alpha = 1
        for cont in validCont:
            if len(cont.names(regexX)) > 1:
                print('Warning: regex', regexX, 'produced more than one match! Abort scatter plot.')
                return
            if len(cont.names(regexY)) > 1:
                print('Warning: regex', regexY, 'produced more than one match! Abort scatter plot.')
                return
            for nameX in cont.names(regexX):
                for nameY in cont.names(regexY):
                    if nHist:
                        alpha = max(alpha / 2, 0.1)
                    dataDictX = cont.get(nameX)
                    dataX = dataDictX['data']
                    weightsX = dataDictX['weights']
                    dataDictY = cont.get(nameY)
                    dataY = dataDictY['data']
                    weightsY = dataDictY['weights']
                    mask = []
                    for x, y in zip(dataX, dataY):
                        xOutRange = (x < xRange[0] or x > xRange[1])
                        yOutRange = (y < yRange[0] or y > yRange[1])
                        mask.append(xOutRange or yOutRange)
                    dataX = np.ma.masked_where(mask, dataX)
                    dataY = np.ma.masked_where(mask, dataY)
                    ax.scatter(dataX, dataY, norm=normed, alpha=alpha, marker='.')
                    nHist += 1

        ax.set(ylabel=ylabel, xlabel=xlabel)
        styles.style_scatter(ax)
        self.save_plot(save, fig)