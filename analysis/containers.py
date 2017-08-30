import pandas as pd


class physics_container:

    def __init__(self, inputFile, maxEvt):
        print('Initializing physics object from file', inputFile, 'using', maxEvt, 'events')
        self._data = pd.read_csv(inputFile, sep="\t", comment="#", index_col=False, engine="python",
                                 header=0, nrows=maxEvt, na_values='-')
        self.names = list(self._data.dtypes.index)[:-1]

    def show(self):
        print('Data loaded:')
        print(self.names)

    def get(self, name):
        return getattr(self._data, name)
