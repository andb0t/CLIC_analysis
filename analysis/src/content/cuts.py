class cuts:
    def __init__(self, name='Cut'):
        self.name = name

    def cut(self, df):
        cutDf = df[df.lep_n > 1]
        return cutDf
