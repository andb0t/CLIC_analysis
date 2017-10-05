import itertools
import re

import pandas as pd

from . import observables
from . import cuts


class physics_container:

    def __init__(self, inputFile=None, maxEvt=None, verbose=0, name=''):
        if verbose:
            if maxEvt is None:
                print('Initializing physics object from file', inputFile, 'using all events')
            else:
                print('Initializing physics object from file', inputFile, 'using', maxEvt, 'events')
        try:
            self.data = pd.read_csv(inputFile, sep="\t", comment="#", index_col=False, engine="python",
                                    header=0, nrows=maxEvt, na_values='-')
        except ValueError:
            self.data = inputFile
        self._names = list(self.data.dtypes.index)[:-1]
        self._namesIter = 0
        self.name = name
        self._verbose = verbose
        if self._verbose > 1:
            print('Loaded those data:')
            print(self.data)

    def __iter__(self):
        return iter([self])

    def __len__(self):
        return 1

    def __add__(self, other):
        sumData = self.data.append(other.data)
        sumName = self.name + ' + ' + other.name
        return physics_container(inputFile=sumData, name=sumName)

    def show(self):
        print('Data loaded:', self._names)

    def cut(self, cutName):
        cut = cuts.cuts(name=cutName)
        cutData = cut.apply_cut(self.data)
        cutName = cut.name + ' ' + self.name.lower()
        return physics_container(inputFile=cutData, name=cutName)

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
                return getattr(self.data, name)
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
                return getattr(self.data, self.names[self._namesIter])
            finally:
                self._namesIter += 1
                self._namesIter %= len(self.names)

    def get_chained(self, regex=''):
        return list(itertools.chain.from_iterable([self.get(name) for name in self.names(regex)]))

    def get_list(self, regex=''):
        return [self.get(name) for name in self.names(regex)]
