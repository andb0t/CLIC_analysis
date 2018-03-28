import argparse
import functools
import os.path

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

if args.full:
    settings.set_data_dir(settings.FULL_DATA_DIR)
    settings.set_plot_dir(settings.FULL_PLOT_DIR)
    settings.set_tex_dir(settings.FULL_TEX_DIR)
else:
    settings.set_data_dir(settings.EXAMPLE_DATA_DIR)
    settings.set_plot_dir(settings.EXAMPLE_PLOT_DIR)
    settings.set_tex_dir(settings.EXAMPLE_TEX_DIR)




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
# allCont[0].show()

# yields.print_samples(allCont, latex=False)

otherCont = functools.reduce(lambda x, y: x + y, allCont[3:])
otherCont.set_name('Other bkg')
plotCont = [allCont[0], allCont[1], allCont[2], otherCont]

# print initial event yield
print('Event yields before selection:')
yields.print_event_yields(plotCont, name='extract_raw', latex=True)

# get initial signal events
nSignalMCRaw = PhysicsNumber(plotCont[0].get_events(), 'stat',
                             statEntries=plotCont[0].get_entries())
# for later verification get total initial events in signal sample (signal + singleW bkg)
nSigSampleMCRaw = PhysicsNumber(plotCont[0].get_events() + plotCont[1].get_events(), 'stat',
                                statEntries=plotCont[0].get_entries() + plotCont[1].get_entries())

# apply cuts
plotCont = list(map(lambda x: x.cut('Final', oldNames=False, silent=True), plotCont))

# print yields
print('Event yields after selection:')
yields.print_event_yields(plotCont, name='extract_final', latex=True)

# set artificial number of data events
nData = PhysicsNumber(args.nData, 'stat')
print('Observed number of data events {:.2f}'.format(nData))
save_value_latex(name='ndata.tex', newcommand='ndata', value=nData, unit='', digits=0)


# use signal fraction from MC to get predicted number of signal events in data
nTotMCEvents = sum(map(lambda c: c.get_events(), plotCont))
nTotMCEntries = sum(map(lambda c: c.get_entries(), plotCont))
nTotMC = PhysicsNumber(nTotMCEvents, 'stat', statEntries=nTotMCEntries)
nSignalMC = PhysicsNumber(plotCont[0].get_events(), 'stat', statEntries=plotCont[0].get_entries())
nSignalFraction = nSignalMC / nTotMC
print('Signal efficiency {:.2f}'.format(nSignalFraction))
nSignal = nData * nSignalFraction
print('Predicted number of signal events after cuts {:.2f}'.format(nSignal))

# calculate efficiency
signalEfficiency = nSignalMC / nSignalMCRaw

# scale observed signal to signal before cuts
nSignalInitial = nSignal / signalEfficiency
print('Predicted number of signal events before cuts {:.2f}'.format(nSignalInitial))

# calculate cross-section
xSec = nSignalInitial / settings.LUMI
xSec.unit = 'fb'
print('The determined cross section is {:.3f}'.format(xSec))

# get comparison value using truth info
xSecComparison = nSignalMCRaw / nSigSampleMCRaw * settings.SIG_SAMPLE['xs']
xSecComparison.unit = 'fb'
print('The truth info comparison value is {:.3f}'.format(xSecComparison))

save_value_latex(name='xsec.tex', newcommand='xSec', value=xSec, unit=r'~\fb')
