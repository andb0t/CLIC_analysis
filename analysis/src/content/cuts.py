import collections

from src import settings


class cuts:
    def __init__(self, name='Cut', dataName=''):
        self.name = name
        self.origN = 0
        self.oldN = 0
        self.cutEff = collections.OrderedDict()
        self.dataName = dataName

    def record_eff(self, name, newN):
        if self.oldN == 0:
            print('Zero! And new is', newN)
            cutEff = None
            totCutEff = None
        else:
            cutEff = newN / self.oldN
            totCutEff = newN / self.origN
        self.cutEff[name] = (cutEff, totCutEff)
        self.oldN = newN

    def print_eff(self):
        print(('------------ Cut efficiency for ' + self.dataName).ljust(50, '-'), ''.rjust(10, '-'), sep='')
        print('Cut'.ljust(40), 'Single'.rjust(10), 'Total'.rjust(10), sep='')
        for key, value in self.cutEff.items():
            print(key.ljust(40), '{0:.1%}'.format(value[0]).rjust(10), '{0:.1%}'.format(value[1]).rjust(10), sep='')
        print('')

    def apply_cut(self, origDf):
        self.origN = origDf.shape[0]
        self.oldN = self.origN
        df = origDf
        if self.name == 'Cut':
            df = df[getattr(df, settings.LEP + 'n') == 1]
            self.record_eff('Lepton number', df.shape[0])

            df = df[getattr(df, settings.JET + 'etot') < 750]
            self.record_eff('Maximum total jet energy', df.shape[0])

            # df = df[getattr(df, settings.JET + 'pt_1') > 40]
            # self.record_eff('Subleading jet pT', df.shape[0])

            df = df[getattr(df, settings.LEP + 'pt_0') > 30]
            self.record_eff('Lepton pT', df.shape[0])

            # df = df[getattr(df, settings.JET + 'etot') > 50]
            # self.record_eff('Minimum total jet energy', df.shape[0])

        self.print_eff()
        return df


