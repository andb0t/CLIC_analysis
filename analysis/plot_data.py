import argparse
import importlib

from src.content import containers
from src.form import plots
from src import settings


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files')
parser.add_argument("--maxevt", nargs='?', help="Specify number maximum number of events", default=None)
parser.add_argument("--sig", action="store_true", default=False, help='Only process signal files')
parser.add_argument("--bkg", action="store_true", default=False, help='Only process background files')
args = parser.parse_args()


MAX_EVT_SIG = None
MAX_EVT_BKG = None
if args.maxevt:
    MAX_EVT_SIG = args.maxevt
    MAX_EVT_BKG = args.maxevt

SAVE_PLOTS = False

DATA_DIR = settings.EXAMPLE_DATA_DIR
if args.full:
    DATA_DIR = settings.FULL_DATA_DIR

if args.sig:
    MAX_EVT_BKG = 0
if args.bkg:
    MAX_EVT_SIG = 0

importlib.reload(plots)
importlib.reload(containers)

sigCont = containers.physics_container(DATA_DIR + settings.SIG_SAMPLE, maxEvt=MAX_EVT_SIG, name='Signal qqln')
bkg0Cont = containers.physics_container(DATA_DIR + settings.QQQQLL_SAMPLE, maxEvt=MAX_EVT_BKG, name='Bkg qqqqll')
bkg1Cont = containers.physics_container(DATA_DIR + settings.QQLL_SAMPLE, maxEvt=MAX_EVT_BKG, name='Bkg qqll')
# sigCont.show()
allCont = sigCont + bkg0Cont + bkg1Cont
allCont.name = 'Total'

# # cuts
# cutCont = sigCont.cut('Cut')
# plots.plot_hist([sigCont, cutCont], 'lep_n', (0, 5), 5, normed=0, save='cut.pdf')
#
# # filters and correlations
# filterCont = sigCont.filter(items=['lep_n', 'lep_pt_0', 'lep_phi_0', 'jet_DH_e_0', 'jet_DH_pt_0', 'depp'])
# plots.plot_corr(filterCont, save='corr_filtered.pdf')
# plots.plot_corr(sigCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
# plots.plot_corr(sigCont, save='corr.pdf')
# plots.plot_corr(bkg0Cont)
# plots.plot_corr(bkg1Cont)

# plots.plot_raw(sigCont, '_n')
# plots.plot_raw(sigCont, 'lep_pt')
# plots.plot_raw(sigCont, 'lep_pt_mean')
# plots.plot_hist(sigCont, 'lep_pt_mean')
# plots.plot_raw(sigCont, '\w*_n')
# plots.plot_hist(sigCont, '\w*_n', (0, 10), 10)
# plots.plot_hist(sigCont, 'lep_pt', (0, 300))
# plots.plot_hist(sigCont, 'lep_pt', (0, 300), stacked=True)
# plots.plot_hist(sigCont, 'lep_pt', (0, 300), stacked=True, chained=True)
#
# plots.plot_raw([sigCont, bkg0Cont, bkg1Cont], 'lep_n', xLabel='N$_{lep}$', noLegName=True)
# plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, xLabel='N$_{lep}$', noLegName=True)
# plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, stacked=True, xLabel='N$_{lep}$', noLegName=True)
# plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, stacked=True, chained=True, xLabel='N$_{lep}$')
#


plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'minv', (0, 200), 40, save='minv.pdf', normed=0)
plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'minvll', (0, 200), 40, save='minvll.pdf')
plots.plot_hist(sigCont, 'jet_DH_pt_[01]', (0, 50), 40, save='pt.pdf')
plots.plot_hist(sigCont, 'jet_DH_theta_[01]', (0, 5), 40, save='theta.pdf')
plots.plot_hist(sigCont, 'jet_DH_phi_[01]', (-4, 4), 40, save='phi.pdf')
plots.plot_hist(sigCont, 'jet_DH_e_[01]', (0, 100), 40, save='e.pdf')
