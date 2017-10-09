import itertools
import re

import pandas as pd

from src.content import observables
from src.content import cuts


class physics_container:

    def __init__(self, input, maxEvt=None, verbose=0, name=''):
        try:
            self.df = pd.read_csv(input, sep="\t", comment="#", index_col=-1, engine="python",
                                    header=0, nrows=maxEvt, na_values='-')
            print('Loaded', name, 'data from file', input)
        except ValueError:
            self.df = input
        self._names = list(self.df.dtypes.index)
        self._namesIter = 0
        self.name = name
        self._verbose = verbose
        if self._verbose > 1:
            print('Loaded those data:')
            print(self.df)

    def __iter__(self):
        return iter([self])

    def __len__(self):
        return 1

    def __add__(self, other):
        sumDf = self.df.append(other.df)
        sumName = self.name + ' + ' + other.name
        return physics_container(sumDf, name=sumName)

    def show(self):
        print('Data loaded:', self._names)

    def cut(self, cutName):
        cut = cuts.cuts(name=cutName, dataName=self.name)
        cutDf = cut.apply_cut(self.df)
        cutName = cut.name + ' ' + self.name.lower()
        return physics_container(cutDf, name=cutName)

    def filter(self, items=None, regex=None):
        filterDf = self.df.filter(items=items, regex=regex)
        return physics_container(filterDf, name=self.name)

    def names(self, regex=''):
        # exact match r'\blep_pt\b'
        # any alphanumeric char '\w_n'
        # all but one char 'lep_pt_[^4]'
        p = re.compile(regex)
        filteredNames = list(filter(lambda x: re.search(p, x), self._names))
        if filteredNames:
            return filteredNames
        else:
            if self._verbose:
                print('No name with regex "' + str(regex) + '" found. Return regex!')
            return [regex]

    def get(self, name=''):
        if name:
            if name in self._names:
                return getattr(self.df, name)
            else:
                if self._verbose:
                    print('"' + str(name) + '" not in names list. Apply defined functions!')
                if name.endswith('_mean'):
                    return observables.calculate_mean(self, name)
                elif name == 'minv':
                    return observables.calculate_minv(self)
                elif name == 'minvll':
                    return observables.calculate_minvll(self)
                else:
                    print('Error: neither found regex nor corresponding defined function to name' +
                          str(name) + '. Return None!')
                    return None
        else:
            try:
                return getattr(self.df, self.names[self._namesIter])
            finally:
                self._namesIter += 1
                self._namesIter %= len(self.names)

    def get_chained(self, regex=''):
        return list(itertools.chain.from_iterable([self.get(name) for name in self.names(regex)]))

    def get_list(self, regex=''):
        return [self.get(name) for name in self.names(regex)]
