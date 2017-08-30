import re
import itertools

import pandas as pd


class physics_container:

    def __init__(self, inputFile, maxEvt):
        print('Initializing physics object from file', inputFile, 'using', maxEvt, 'events')
        self._data = pd.read_csv(inputFile, sep="\t", comment="#", index_col=False, engine="python",
                                 header=0, nrows=maxEvt, na_values='-')
        self._names = list(self._data.dtypes.index)[:-1]
        self._namesIter = 0

    def show(self):
        print('Data loaded:', self._names)

    def names(self, regex=''):
        # exact match r'\blep_pt\b'
        # any alphanumeric char '\w_n'
        # all but one char 'lep_pt_[^4]'
        p = re.compile(regex)
        return list(filter(lambda x: re.search(p, x), self._names))

    def get(self, name=''):
        if name:
            return getattr(self._data, name)
        else:
            try:
                return getattr(self._data, self.names[self._namesIter])
            finally:
                self._namesIter += 1
                self._namesIter %= len(self.names)

    def get_chained(self, regex=''):
        return list(itertools.chain.from_iterable([self.get(name).dropna() for name in self.names(regex)]))

    def get_list(self, regex=''):
        return [self.get(name).dropna() for name in self.names(regex)]
