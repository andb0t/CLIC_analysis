import argparse
import functools
import importlib
import sys

from src.content import containers
from src.form import plots
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

SAVE_PLOTS = False

dataDir = settings.EXAMPLE_DATA_DIR
if args.full:
    dataDir = settings.FULL_DATA_DIR

if args.sig:
    maxEvtBkg = 0
if args.bkg:
    maxEvtSig = 0

# importlib.reload(plots)
# importlib.reload(containers)

#load data
allCont = []
allCont.append(containers.physics_container(dataDir + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Signal qqln'))
allCont.append(containers.physics_container(dataDir + settings.QQLL_SAMPLE['csv'], xSec=settings.QQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqll'))
allCont.append(containers.physics_container(dataDir + settings.QQQQLN_SAMPLE['csv'], xSec=settings.QQQQLN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqln'))
allCont.append(containers.physics_container(dataDir + settings.QQQQLL_SAMPLE['csv'], xSec=settings.QQQQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqll'))
allCont.append(containers.physics_container(dataDir + settings.ZZNN_SAMPLE['csv'], xSec=settings.ZZNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg zznn'))
allCont.append(containers.physics_container(dataDir + settings.QQQQ_SAMPLE['csv'], xSec=settings.QQQQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqq'))
allCont.append(containers.physics_container(dataDir + settings.QQNN_SAMPLE['csv'], xSec=settings.QQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqnn'))
allCont.append(containers.physics_container(dataDir + settings.QQQQNN_SAMPLE['csv'], xSec=settings.QQQQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqnn'))
# allCont[0].show()

# print all cut efficiencies and yields
allCont = list(map(lambda x: x.cut('Final', latex=True), allCont))
yields.print_event_yields(allCont, name='final', latex=True)


if args.yields:
	sys.exit()

# create plots
otherCont = functools.reduce(lambda x, y: x + y, allCont[2:])
otherCont.name = 'Other bkg'
plotCont = [allCont[0], allCont[1], otherCont]

routines.kinematic_figures(plotCont, savePrefix='raw', savePlots=True )
routines.correlation_figures(allCont[0], savePrefix='raw', savePlots=True )

# # apply cuts
plotCont = list(map(lambda x: x.cut('Pre'), plotCont))
routines.kinematic_figures(plotCont, savePrefix='pre', savePlots=True )
routines.correlation_figures(allCont[0], savePrefix='pre', savePlots=True )

# # apply cuts
plotCont = list(map(lambda x: x.cut('Final'), plotCont))
routines.kinematic_figures(plotCont, savePrefix='fin', savePlots=True )
routines.correlation_figures(allCont[0], savePrefix='fin', savePlots=True )

yields.print_event_yields(plotCont)