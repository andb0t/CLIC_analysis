import collections

from src import settings


class cuts:
    def __init__(self, name='Pre', dataName=''):
        self.name = name
        self.origN = 0
        self.oldN = 0
        self.cutEff = collections.OrderedDict()
        self.dataName = dataName

    def record_eff(self, name, df):
        newN = df.shape[0]
        if self.oldN == 0:
            print('Zero! And new is', newN)
            cutEff = None
            totCutEff = None
        else:
            cutEff = newN / self.oldN
            totCutEff = newN / self.origN
        nEvents = df[settings.SF].sum()
        self.cutEff[name] = (cutEff, totCutEff, newN, nEvents)
        self.oldN = newN

    def print_eff(self):

        print(('------------ Cut efficiency for ' + self.dataName).ljust(40, '-'), 
              ''.rjust(10, '-'), 
              ''.rjust(10, '-'), 
              ''.rjust(10, '-'), 
              ''.rjust(15, '-'), 
              sep='')
        print('Cut'.ljust(40), 
              'Single'.rjust(10), 
              'Total'.rjust(10), 
              'Entries'.rjust(10),
              'Events'.rjust(15),
              sep='') 
        for key, value in self.cutEff.items():
            print(key.ljust(40), 
                  '{0:.1%}'.format(value[0]).rjust(10), 
                  '{0:.1%}'.format(value[1]).rjust(10), 
                  '{0}'.format(value[2]).rjust(10), 
                  '{0:.1f}'.format(value[3]).rjust(15), 
                  sep='')
        print('')

    def write_eff_latex(self):
        fileName = self.dataName.replace(" ", "_").lower() + '_cut_' + self.name.lower() + '.log'
        print('Writing cut efficiencies to', fileName)
        pass

    def apply_cut(self, origDf):
        self.origN = origDf.shape[0]
        self.oldN = self.origN
        df = origDf
        self.record_eff('Before cuts', df)


        if self.name == 'Pre' or self.name == 'Final':
            df = df[getattr(df, settings.LEP + 'n') == 1]
            self.record_eff('Lepton number', df)

        if self.name == 'Final':
            df = df[getattr(df, settings.JET + 'etot') < 750]
            self.record_eff('Maximum total jet energy', df)

            # df = df[getattr(df, settings.JET + 'pt_1') > 40]
            # self.record_eff('Subleading jet pT', df)

            df = df[getattr(df, settings.LEP + 'pt_0') > 30]
            self.record_eff('Lepton pT', df)

            # df = df[getattr(df, settings.JET + 'etot') > 50]
            # self.record_eff('Minimum total jet energy', df.shape[0])

        self.print_eff()
        self.write_eff_latex()
        return df