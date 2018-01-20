import argparse
import functools
import sys

from src.content import containers
from src.form import yields
from src.routines import routines
from src import settings


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files, not on small test samples')
parser.add_argument("--maxevt", nargs='?', type=int, help="Specify number maximum number of events", default=None)
parser.add_argument("--sig", action="store_true", default=False, help='Only process signal files')
parser.add_argument("--bkg", action="store_true", default=False, help='Only process background files')
parser.add_argument("--yields", action="store_true", default=False, help='Only determine selection yields')
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

if args.sig:
    maxEvtBkg = 0
if args.bkg:
    maxEvtSig = 0

# load data
allCont = []
# signal
allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Signal qqln'))
# split off background from signal: W -> qqln from WW -> qqln
allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Bkg qqln'))
allCont[0] = allCont[0].cut('noSingleW', addName=False)
allCont[1] = allCont[1].cut('singleW', addName=False)
# other backgrounds
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQLL_SAMPLE['csv'], xSec=settings.QQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqll'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQLN_SAMPLE['csv'], xSec=settings.QQQQLN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqln'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQLL_SAMPLE['csv'], xSec=settings.QQQQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqll'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQ_SAMPLE['csv'], xSec=settings.QQQQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqq'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQNN_SAMPLE['csv'], xSec=settings.QQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqnn'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQNN_SAMPLE['csv'], xSec=settings.QQQQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqnn'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQ_SAMPLE['csv'], xSec=settings.QQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qq'))
# allCont[0].show()

yields.print_samples(allCont, latex=True)

otherCont = functools.reduce(lambda x, y: x + y, allCont[3:])
otherCont.set_name('Other bkg')
plotCont = [allCont[0], allCont[1], allCont[2], otherCont]

# print all cut efficiencies and yields
allCont = list(map(lambda x: x.cut('Final', latex=True), allCont))
yields.print_event_yields(allCont, name='final', latex=True)

if args.yields:
    sys.exit()

# create plots
routines.kinematic_figures(plotCont, savePrefix='raw', savePlots=True)
routines.correlation_figures(allCont[0], savePrefix='raw', savePlots=True)

# # apply cuts
plotCont = list(map(lambda x: x.cut('Pre'), plotCont))
routines.kinematic_figures(plotCont, savePrefix='pre', savePlots=True)
routines.correlation_figures(allCont[0], savePrefix='pre', savePlots=True)

# # apply cuts
plotCont = list(map(lambda x: x.cut('Final', oldNames=False), plotCont))
routines.kinematic_figures(plotCont, savePrefix='fin', savePlots=True)
routines.correlation_figures(allCont[0], savePrefix='fin', savePlots=True)

yields.print_event_yields(plotCont)
