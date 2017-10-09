import argparse
import importlib

from src.content import containers
from src.form.plots import plots
from src import settings


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files, not on small test samples')
parser.add_argument("--maxevt", nargs='?', type=int, help="Specify number maximum number of events", default=None)
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

# importlib.reload(plots)
# importlib.reload(containers)

#load data
sigCont = containers.physics_container(DATA_DIR + settings.SIG_SAMPLE, maxEvt=MAX_EVT_SIG, name='Signal qqln')
bkg0Cont = containers.physics_container(DATA_DIR + settings.QQQQLL_SAMPLE, maxEvt=MAX_EVT_BKG, name='Bkg qqqqll')
bkg1Cont = containers.physics_container(DATA_DIR + settings.QQLL_SAMPLE, maxEvt=MAX_EVT_BKG, name='Bkg qqll')
# sigCont.show()




allCont = sigCont + bkg0Cont + bkg1Cont
allCont.name = 'Total'




# plots.plot_raw(sigCont, '_n')
# plots.plot_raw(sigCont, 'lep_pt')
# plots.plot_raw(sigCont, 'lep_pt_mean')
# plots.plot_hist(sigCont, 'lep_pt_mean')
# plots.plot_raw(sigCont, '\w*_n')
# plots.plot_hist(sigCont, '\w*_n', (0, 10), 10)
# plots.plot_hist(sigCont, 'lep_pt', (0, 300))
# plots.plot_hist(sigCont, 'lep_pt', (0, 300), mode='stacked')
# plots.plot_hist(sigCont, 'lep_pt', (0, 300), mode='chained')
#
# plots.plot_raw([sigCont, bkg0Cont, bkg1Cont], 'lep_n', xlabel='N$_{lep}$', noLegName=True)
# plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, mode='stacked', xlabel='N$_{lep}$', noLegName=True)
# plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, mode='chained', xlabel='N$_{lep}$')
# plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'minvll', (0, 200), 40, xlabel='m$_{dilep}$ [GeV]', noLegName=True, save='minvll.pdf')
#

rawPlots = plots(savePrefix='raw', noLegName=True, savePlots=True)
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'minv', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', save='minv.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', save='lep_n.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.LEP + 'pt_0', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', save='lep_pt.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.LEP + 'e_0', (0, 1000), 40, xlabel='Lepton E [GeV]', save='lep_e.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', save='jet_etot.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode='chained', save='jet_pt.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'theta_[\d]', (0, 3.2), 40, mode='chained', save='jet_theta.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, mode='chained', save='jet_phi.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode='chained', save='jet_e.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'e_0', (0, 400), 40, xlabel='Leading jet E [GeV]', save='jet_e_0.pdf')
rawPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'e_1', (0, 400), 40, xlabel='Subleading jet E [GeV]', save='jet_e_1.pdf')

filterCont = sigCont.filter(items=['lep_n', 'lep_pt_0', 'lep_phi_0', 'jet_DH_e_0', 'jet_DH_pt_0'])
rawPlots.plot_corr(filterCont, save='corr_filtered.pdf')
rawPlots.plot_corr(sigCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
rawPlots.plot_corr(sigCont, save='corr.pdf')
rawPlots.plot_corr(bkg0Cont)
rawPlots.plot_corr(bkg1Cont)

# apply cuts
sigCont = sigCont.cut('Cut')
bkg0Cont = bkg0Cont.cut('Cut')
bkg1Cont = bkg1Cont.cut('Cut')

cutPlots = plots(savePrefix='cut', noLegName=True, savePlots=True)
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'minv', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', save='minv.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', save='lep_n.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.LEP + 'pt_0', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', save='lep_pt.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.LEP + 'e_0', (0, 1000), 40, xlabel='Lepton E [GeV]', save='lep_e.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', save='jet_etot.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode='chained', save='jet_pt.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'theta_[\d]', (0, 3.2), 40, mode='chained', save='jet_theta.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, mode='chained', save='jet_phi.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode='chained', save='jet_e.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'e_0', (0, 400), 40, xlabel='Leading jet E [GeV]', save='jet_e_0.pdf')
cutPlots.plot_hist([sigCont, bkg0Cont, bkg1Cont], settings.JET + 'e_1', (0, 400), 40, xlabel='Subleading jet E [GeV]', save='jet_e_1.pdf')

filterCont = sigCont.filter(items=['lep_n', 'lep_pt_0', 'lep_phi_0', 'jet_DH_e_0', 'jet_DH_pt_0'])
cutPlots.plot_corr(filterCont, save='corr_filtered.pdf')
cutPlots.plot_corr(sigCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
cutPlots.plot_corr(sigCont, save='corr.pdf')
cutPlots.plot_corr(bkg0Cont)
cutPlots.plot_corr(bkg1Cont)