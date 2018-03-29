import itertools
import re

import numpy as np
import pandas as pd

from src.content import observables
from src.content import cuts
from src import settings


class physics_container:
    """A container for physics data in form of pandas dataframes

    Keyword arguments:
    input    -- input file path or dataframe
    maxEvt   -- maximum number of events to read from file
    verbose  -- verbosity of operations
    name     -- initial name of the dataset (may be modified by operations)
    origName -- original initial name of the dataset
    xSec     -- the cross-section value for the scaling of entries to events
    fileName -- the name of the file the dataset was read from
    """

    def __init__(self, input, maxEvt=None, verbose=0, name='', origName=None, xSec=1, fileName=None, nEvtUnc=None):
        self.xSec = xSec
        try:
            self.df = pd.read_csv(input, sep="\t", comment="#", index_col=0, engine="python",
                                  header=0, nrows=maxEvt, na_values='-')
            self.fileName = input
            try:
                lumiScaleFactor = settings.LUMI * self.xSec / self.df.shape[0]
            except ZeroDivisionError:
                lumiScaleFactor = 1
            self.df[settings.SF] = lumiScaleFactor
            self.df[settings.SF].astype(np.float64)
            self.nEvtUnc = lumiScaleFactor * np.sqrt(self.df.shape[0]) if nEvtUnc is None else nEvtUnc
            print('Loaded', name, 'data from file', input)
        except ValueError:
            self.df = input
            self.fileName = fileName
            self.nEvtUnc = nEvtUnc
        self._names = list(self.df.dtypes.index)
        self._namesIter = 0
        self.name = name
        if origName is None:
            self.origName = name
        else:
            self.origName = origName
        self._verbose = verbose
        if self._verbose > 1:
            print('Loaded those data:')
            print(self.df)

    def __iter__(self):
        return iter([self])

    def __len__(self):
        return 1

    def __add__(self, other):
        sumDf = self.df.append(other.df, ignore_index=True)
        sumName = self.name + ' + ' + other.name
        nEvtUnc = np.sqrt(self.nEvtUnc ** 2 + other.nEvtUnc ** 2)
        return physics_container(sumDf, name=sumName, xSec=self.xSec, fileName=self.fileName, nEvtUnc=nEvtUnc)

    def show(self):
        print('Data loaded:', self._names)

    def describe(self):
        print(self.df.describe())

    def set_name(self, name):
        self.name = name
        self.origName = name

    def cut(self, cutName, addName=True, oldNames=True, **kwargs):
        cut = cuts.cuts(name=cutName, dataName=self.name, **kwargs)
        cutDf = cut.apply_cut(self.df)
        if addName:
            if oldNames:
                cutName = cut.name + ' ' + self.name.lower()
            else:
                cutName = cut.name + ' ' + self.origName.lower()
        else:
            if oldNames:
                cutName = self.name
            else:
                cutName = self.origName
        # frac = np.sqrt(getattr(cutDf, settings.SF).pow(2).sum()) / np.sqrt(getattr(self.df, settings.SF).pow(2).sum())
        frac = cutDf.shape[0] / self.df.shape[0]
        # frac = getattr(cutDf, settings.SF).sum() / getattr(self.df, settings.SF).sum()
        nEvtUnc = self.nEvtUnc * np.sqrt(frac)
        # frac = cutDf.shape[0] / self.df.shape[0]
        # deviation = self.nEvtUnc / np.sqrt(self.df.shape[0])
        # nEvtUnc = deviation * np.sqrt(self.df.shape[0]) * np.sqrt(frac)
        # nEvtUnc = deviation * np.sqrt(getattr(cutDf, settings.SF).sum()) * np.sqrt(frac)
        # nEvtUnc = deviation * np.sqrt(cutDf.shape[0]) * np.sqrt(frac)
        return physics_container(cutDf, name=cutName, origName=self.origName, xSec=self.xSec, fileName=self.fileName, nEvtUnc=nEvtUnc)

    def filter(self, items=None, regex=None):
        filterDf = self.df.filter(items=items, regex=regex)
        nEvtUnc = 0
        return physics_container(filterDf, name=self.name, xSec=self.xSec, fileName=self.fileName, nEvtUnc=nEvtUnc)

    def names(self, regex=''):
        if regex in observables.keywords:
            return [regex]
        # exact match r'\blep_pt\b'
        # any alphanumeric char '\w_n'
        # all but one char 'lep_pt_[^4]'
        if settings.EL in regex or settings.MU in regex:
            whatLep = settings.EL if settings.EL in regex else settings.MU
            tmpRegex = regex.replace(whatLep, settings.LEP)
            p = re.compile(tmpRegex)
            filteredNames = list(filter(lambda x: re.search(p, x), self._names))
            filteredNames = list(map(lambda s: s.replace(settings.LEP, whatLep), filteredNames))
        else:
            p = re.compile(regex)
            filteredNames = list(filter(lambda x: re.search(p, x), self._names))
        if filteredNames:
            return filteredNames
        else:
            if self._verbose:
                print('No name with regex "' + str(regex) + '" found. Return regex!')
            return [regex]

    def _get(self, name=''):
        if name:
            if name in self._names:
                return getattr(self.df, name)
            else:
                if self._verbose:
                    print('"' + str(name) + '" not in names list. Apply defined functions!')
                if name.endswith('_mean'):
                    return observables.calculate_mean(self, name)
                elif name == 'mjj':
                    return observables.calculate_mjj(self)
                elif name == 'mll':
                    return observables.calculate_mll(self)
                elif name == 'mln':
                    return observables.calculate_mln(self)
                elif name == 'theta_W':
                    return observables.calculate_theta_W(self)
                elif name == 'theta_L':
                    return observables.calculate_theta_L(self)
                elif name == 'theta_H':
                    return observables.calculate_theta_H(self)
                elif name == 'phi_L':
                    return observables.calculate_phi_L(self)
                elif name == 'phi_H':
                    return observables.calculate_phi_H(self)
                elif settings.EL in name or settings.MU in name:
                    return observables.special_lepton_type(self, name)
                else:
                    print('Error: neither found regex nor corresponding defined function to name',
                          str(name) + '. Return None!')
                    nanArr = np.empty(self.df.shape[0])
                    nanArr.fill(np.nan)
                    return nanArr
        else:
            try:
                return getattr(self.df, self.names[self._namesIter])
            finally:
                self._namesIter += 1
                self._namesIter %= len(self.names)

    def get(self, name=''):
        data = self._get(name)
        weights = getattr(self.df, settings.SF)
        return self.data_dict(data, weights)

    def get_chained(self, regex=''):
        data = list(itertools.chain.from_iterable([self._get(name) for name in self.names(regex)]))
        weights = list(itertools.chain.from_iterable([self._get(settings.SF) for name in self.names(regex)]))
        return self.data_dict(data, weights)

    def get_stacked(self, regex=''):
        data = [self._get(name) for name in self.names(regex)]
        weights = [self._get(settings.SF) for name in self.names(regex)]
        return self.data_dict(data, weights)

    def data_dict(self, data, weights):
        return {'data': data, 'weights': weights}

    def get_events(self):
        return getattr(self.df, settings.SF).sum()

    def get_entries(self):
        return self.df.shape[0]

    def get_events_unc(self):
        return self.nEvtUnc

    def get_entries_unc(self):
        return np.sqrt(self.df.shape[0])