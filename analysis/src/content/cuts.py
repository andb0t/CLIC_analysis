class cuts:
    def __init__(self, name='Cut'):
        self.name = name

    def apply_cut(self, df):
        cutDf = df[getattr(df, 'lep_n') == 1]
        return cutDf
