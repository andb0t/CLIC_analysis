import argparse
import functools
import os.path

import numpy as np

from src.content import containers
from src.form import yields
from src import settings
from src.utils.number import PhysicsNumber
from src.utils.latex import save_value_latex


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files')
parser.add_argument("--maxevt", nargs='?', type=int, help="Specify maximum number of events", default=None)
parser.add_argument("--nData", nargs='?', type=int, help="Specify number of found data events", default=175171)
args = parser.parse_args()

maxEvtSig = None
maxEvtBkg = None
if args.maxevt:
    maxEvtSig = args.maxevt
    maxEvtBkg = args.maxevt

settings.init_globals(args.full)

# load data
allCont = []
# signal
allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Signal qqln'))
# split off background from signal: W -> qqln from WW -> qqln
allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Bkg qqln'))
allCont[0] = allCont[0].cut('noSingleW', addName=False, silent=True)
allCont[1] = allCont[1].cut('singleW', addName=False, silent=True)
# other backgrounds
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQLL_SAMPLE['csv'], xSec=settings.QQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqll'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQLN_SAMPLE['csv'], xSec=settings.QQQQLN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqln'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQLL_SAMPLE['csv'], xSec=settings.QQQQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqll'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQ_SAMPLE['csv'], xSec=settings.QQQQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqq'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQNN_SAMPLE['csv'], xSec=settings.QQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqnn'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQNN_SAMPLE['csv'], xSec=settings.QQQQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqnn'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQ_SAMPLE['csv'], xSec=settings.QQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qq'))

sepSamples = 1
otherCont = functools.reduce(lambda x, y: x + y, allCont[sepSamples:])
plotCont = [*allCont[:sepSamples], otherCont]
plotCont[0].set_name('Signal')
plotCont[1].set_name('Background')

# print initial event yield
print('Event yields before selection:')
yields.print_event_yields(allCont)
yields.print_event_yields(plotCont, name='extract_raw', latex=True)

# get initial signal events
nSignalMCRaw = PhysicsNumber(plotCont[0].get_events(), plotCont[0].get_events_unc())
# for later verification get total initial events in signal sample (signal + singleW bkg)
nSigSampleMCRaw = PhysicsNumber(allCont[0].get_events() + allCont[1].get_events(), (allCont[0] + allCont[1]).get_events_unc())
sumCont = functools.reduce(lambda x, y: x + y, plotCont)
nTotMCRaw = PhysicsNumber(sumCont.get_events(), sumCont.get_events_unc())

# apply cuts
plotCont = list(map(lambda x: x.cut('Final', oldNames=False, silent=True), plotCont))

# print yields
print('Event yields after selection:')
yields.print_event_yields(list(map(lambda x: x.cut('Final', silent=True), allCont)))
# yields.print_samples(allCont)
yields.print_event_yields(plotCont, name='extract_final', latex=True)

# set artificial number of data events
nData = PhysicsNumber(args.nData, np.sqrt(args.nData))
print('Observed number of data events {:.2f}'.format(nData))

save_value_latex(name='ndata.tex', newcommand='ndata', value=nData, unit='', digits=0)
save_value_latex(name='datalumi.tex', newcommand='datalumi', value=settings.LUMI, unit=r'\invfb', digits=0)
save_value_latex(name='signalxs.tex', newcommand='signalxs', value=settings.SIG_SAMPLE['xs'], unit=r'\fb', digits=1)


# use signal fraction from MC to get predicted number of signal events in data
sumCont = functools.reduce(lambda x, y: x + y, plotCont)
nTotMC = PhysicsNumber(sumCont.get_events(), sumCont.get_events_unc())
nSignalMC = PhysicsNumber(plotCont[0].get_events(), plotCont[0].get_events_unc())
nSignalFraction = nSignalMC / nTotMC
print('Signal fraction {:.2f}'.format(nSignalFraction))
nSignal = nData * nSignalFraction
print('Predicted number of signal events after cuts: {:.2f}'.format(nSignal))

# calculate efficiency
signalEfficiency = nSignalMC / nSignalMCRaw
print('Signal efficiency {:.2f}'.format(signalEfficiency))

# scale observed signal to signal before cuts
nSignalInitial = nSignal / signalEfficiency
print('Predicted number of signal events before cuts: {:.2f}'.format(nSignalInitial))

# calculate cross-section
xSec = nSignalInitial / settings.LUMI
xSec.unit = 'fb'
print('The determined cross section is {:.3f}'.format(xSec))

# get comparison value using truth info
xSecComparison = nSignalMCRaw / nSigSampleMCRaw * settings.SIG_SAMPLE['xs']
xSecComparison.unit = 'fb'
print('The truth info comparison value is {:.3f}'.format(xSecComparison))

# calculate S / sqrt(S*B)
totalEfficiency = nTotMC / nTotMCRaw
print('Total selection efficiency {:.2f}'.format(totalEfficiency))

nTotalInitial = nData / totalEfficiency
print('Predicted total number of events before cuts: {:.2f}'.format(nTotalInitial))

xSecUnc = nSignalInitial / nTotalInitial ** 0.5
print('Actual uncertainty on cross-section S / sqrt(S*B): {:.3f}'.format(xSecUnc))
xSec.uncertainty = xSecUnc.val()
save_value_latex(name='xsec.tex', newcommand='xSec', value=xSec, unit=r'\fb')

