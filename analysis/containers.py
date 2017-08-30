import re
import itertools

import pandas as pd
import numpy as np


class physics_container:

    def __init__(self, inputFile, maxEvt, verbose=0):
        print('Initializing physics object from file', inputFile, 'using', maxEvt, 'events')
        self._data = pd.read_csv(inputFile, sep="\t", comment="#", index_col=False, engine="python",
                                 header=0, nrows=maxEvt, na_values='-')
        self._names = list(self._data.dtypes.index)[:-1]
        self._namesIter = 0
        self._verbose = verbose
        if self._verbose > 1:
            print('Loaded those data:')
            print(self._data)

    def show(self):
        print('Data loaded:', self._names)

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
                return getattr(self._data, name)
            else:
                if self._verbose:
                    print('"' + str(name) + '" not in names list. Apply defined functions!')
                if name.endswith('_mean'):
                    newRegex = re.sub('\_mean$', '', name)
                    print('Calculate mean of names corresponding to regex', newRegex, 'for each event!')
                    obsLists = list(self.get_list(regex=newRegex))
                    mean = [np.nanmean(x) for x in zip(*obsLists)]
                    mean = list(map(lambda x: x if x == x else 0, mean))
                    print('mean:', mean)
                    return mean
                else:
                    print('Error: neither found regex nor corresponding defined function to name' +
                          str(name) + '. Return None!')
                    return None
        else:
            try:
                return getattr(self._data, self.names[self._namesIter])
            finally:
                self._namesIter += 1
                self._namesIter %= len(self.names)

    def get_chained(self, regex=''):
        return itertools.chain.from_iterable([self.get(name) for name in self.names(regex)])

    def get_list(self, regex=''):
        return [self.get(name) for name in self.names(regex)]
