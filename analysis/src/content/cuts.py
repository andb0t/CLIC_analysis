import collections
import tabulate

from src import settings


class cuts:
    def __init__(self, name='Pre', dataName='', latex=False, silent=False):
        self.name = name
        self.origN = 0
        self.oldN = 0
        self.cutEff = collections.OrderedDict()
        self.dataName = dataName
        self.latex = latex
        self.silent = silent

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
        headers = [self.dataName + ' cut', 'Single', 'Total', 'Entries', 'Events']
        floatfmt = ['', '.1%', '.1%', 'd', '.1f']
        table = []
        for key, val in self.cutEff.items():
            table.append([key, *val])
        if not self.silent:
            print(tabulate.tabulate(table, headers=headers, floatfmt=floatfmt, tablefmt='grid'))
        if self.latex:
            fileName = settings.TEX_DIR + self.name.lower() + '_cut_' + self.dataName.replace(" ", "_").lower() + '.log'
            print('Writing cut efficiencies to', fileName)
            with open(fileName, 'w')  as myfile:
                print(tabulate.tabulate(table, headers=headers, floatfmt=floatfmt, tablefmt='latex'), file=myfile)

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
        return df