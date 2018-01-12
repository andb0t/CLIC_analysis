import collections
import math
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
            cutEff = None
            totCutEff = None
        else:
            cutEff = newN / self.oldN
            totCutEff = newN / self.origN
        nEvents = df[settings.SF].sum()
        nEvents = 0 if math.isnan(nEvents) else nEvents
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
            fileName = settings.TEX_DIR + self.name.lower() + '_cut_' + self.dataName.replace(" ", "_").lower() + '.tex'
            print('Writing cut efficiencies to', fileName)
            with open(fileName, 'w') as myfile:
                print(r'\begin{table}', file=myfile)
                print(tabulate.tabulate(table, headers=headers, floatfmt=floatfmt, tablefmt='latex'), file=myfile)
                print('\caption{Selection efficiencies for the', self.dataName.lower(), 'sample}', file=myfile)
                print(r'\end{table}', file=myfile)

    def apply_cut(self, origDf):
        self.origN = origDf.shape[0]
        self.oldN = self.origN
        df = origDf
        self.record_eff('Before cuts', df)

        if self.name == 'Comp':
            df = df[getattr(df, settings.LEP + 'n') == 1]
            self.record_eff('Lepton number', df)

            df = df[getattr(df, settings.LEP + 'pt_0') > 10]
            self.record_eff('Lepton pT', df)

        if self.name == 'Pre' or self.name == 'Final':
            df = df[getattr(df, settings.LEP + 'n') == 1]
            self.record_eff('Lepton number', df)

            df = df[abs(getattr(df, settings.LEP + 'type_0')) == 13]
            self.record_eff('Muons only', df)

        if self.name == 'Final':
            thetaLowCut = math.acos(0.95)
            thetaHighCut = math.acos(-0.95)
            df = df[(getattr(df, settings.JET + 'theta_0') > thetaLowCut) &
                    (getattr(df, settings.JET + 'theta_1') > thetaLowCut) &
                    (getattr(df, settings.JET + 'theta_0') < thetaHighCut) &
                    (getattr(df, settings.JET + 'theta_1') < thetaHighCut)]
            self.record_eff('Forward jets', df)

            df = df[getattr(df, settings.JET + 'etot') < 750]
            self.record_eff('Maximum total jet energy', df)

            # df = df[getattr(df, settings.JET + 'pt_1') > 40]
            # self.record_eff('Subleading jet pT', df)

            df = df[getattr(df, settings.LEP + 'pt_0') > 30]
            self.record_eff('Lepton pT', df)

            # df = df[getattr(df, settings.JET + 'etot') > 50]
            # self.record_eff('Minimum total jet energy', df.shape[0])

            # invatiant mass cut of 1.2 TeV (for 1.4 TeV smaples) and 2.8 TeV (for 3 TeV samples)

        if self.name == 'singleW':
            df = df[(getattr(df, settings.MC + 'ln_m') > 100) |
                    (getattr(df, settings.MC + 'qq_m') > 100)]
            self.record_eff('Leptonic W mass', df)


        if self.name == 'noSingleW':
            df = df[(getattr(df, settings.MC + 'ln_m') < 100) &
                    (getattr(df, settings.MC + 'qq_m') < 100)]
            self.record_eff('Leptonic W mass', df)

        self.print_eff()
        return df
