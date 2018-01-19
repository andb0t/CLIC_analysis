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

SAVE_PLOTS = False

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

# datasets to compare
allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Old'))
allCont.append(containers.physics_container(settings.DATA_DIR + settings.NEWDET_SIG_SAMPLE['csv'], xSec=settings.NEWDET_SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='New'))


yields.print_samples(allCont, name='comp', latex=True)
yields.print_event_yields(allCont, name='comp', latex=True, silent=False)

if args.yields:
    sys.exit()

# create plots
routines.kinematic_figures(allCont, savePrefix='comp_raw', mode='chained', normed=True, savePlots=True)

# # apply cuts
allCont = list(map(lambda x: x.cut('Comp', latex=True), allCont))
routines.kinematic_figures(allCont, savePrefix='comp_pre', mode='chained', normed=True, savePlots=True)

yields.print_event_yields(allCont)
