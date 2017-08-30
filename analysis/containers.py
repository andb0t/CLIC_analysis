import re

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
        p = re.compile(regex)
        return filter(lambda x: re.search(p, x), self._names)

    def get(self, name=''):
        if name:
            return getattr(self._data, name)
        else:
            try:
                return getattr(self._data, self.names[self._namesIter])
            finally:
                self._namesIter += 1
                self._namesIter %= len(self.names)
