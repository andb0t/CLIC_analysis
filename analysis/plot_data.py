import argparse
import importlib
# import sys

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
sigCont = containers.physics_container(DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=MAX_EVT_SIG, name='Signal qqln')
bkg0Cont = containers.physics_container(DATA_DIR + settings.QQLL_SAMPLE['csv'], xSec=settings.QQLL_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg qqll')
bkg1Cont = containers.physics_container(DATA_DIR + settings.QQQQLL_SAMPLE['csv'], xSec=settings.QQQQLL_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg qqqqll')
bkg2Cont = containers.physics_container(DATA_DIR + settings.QQQQLN_SAMPLE['csv'], xSec=settings.QQQQLN_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg qqqqln')
bkg3Cont = containers.physics_container(DATA_DIR + settings.QQNN_SAMPLE['csv'], xSec=settings.QQNN_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg qqnn')
bkg4Cont = containers.physics_container(DATA_DIR + settings.QQQQ_SAMPLE['csv'], xSec=settings.QQQQ_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg qqqq')
bkg5Cont = containers.physics_container(DATA_DIR + settings.QQQQNN_SAMPLE['csv'], xSec=settings.QQQQNN_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg qqqqnn')
bkg6Cont = containers.physics_container(DATA_DIR + settings.ZZNN_SAMPLE['csv'], xSec=settings.ZZNN_SAMPLE['xs'], maxEvt=MAX_EVT_BKG, name='Bkg zznn')
# sigCont.show()
otherCont = bkg1Cont + bkg2Cont + bkg3Cont + bkg4Cont + bkg5Cont + bkg6Cont
otherCont.name = 'Other bkg'
allCont = [sigCont, bkg0Cont, otherCont]


rawPlots = plots(savePrefix='raw', noLegName=True, savePlots=True)
# rawPlots.plot_hist(allCont, 'minvll', (0, 200), 40, xlabel='m$_{dilep}$ [GeV]', save='minvll.pdf')
rawPlots.plot_hist(allCont, settings.SF, (0, 1000), 40, xlabel='Scale factor', mode='stacked', save='sf.pdf')
rawPlots.plot_hist(allCont, 'minv', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', mode='stacked', save='minv.pdf')
rawPlots.plot_hist(allCont, settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', mode='stacked', save='lep_n.pdf')
rawPlots.plot_hist(allCont, settings.LEP + 'etot', (0, 1000), 40, xlabel='Lepton E$_{tot}$ [GeV]', mode='stacked', save='lep_etot.pdf')
rawPlots.plot_hist(allCont, settings.LEP + 'pt', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', mode='stacked', save='lep_pt.pdf')
rawPlots.plot_hist(allCont, settings.LEP + 'e', (0, 1000), 40, xlabel='Lepton E [GeV]', mode='stacked', save='lep_e.pdf')
rawPlots.plot_hist(allCont, settings.LEP + 'theta', (0, 3.2), 40, xlabel=r'Lepton $\theta$', mode='stacked', save='lep_theta.pdf')
rawPlots.plot_hist(allCont, settings.LEP + 'phi', (-3.2, 3.2), 40, xlabel='Lepton $\phi$', mode='stacked', save='lep_phi.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'n', (0, 6), 6, xlabel='N$_{jet}$', mode='stacked', save='jet_n.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', mode='stacked', save='jet_etot.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'theta_[\d]', (0, 3.2), 40, xlabel=r'Jet $\theta$', mode='stacked', save='jet_theta.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, xlabel='Jet $\phi$', mode='stacked', save='jet_phi.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode='stacked', save='jet_e.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'pt_0', (0, 400), 40, xlabel='Leading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_0.pdf')
rawPlots.plot_hist(allCont, settings.JET + 'pt_1', (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_1.pdf')
# rawPlots.plot_scatter(allCont, settings.JET + 'pt_1', settings.JET + 'pt_0', (0, 600), (0, 400),  xlabel='Subleading jet p$_{T}$ [GeV]', ylabel='Leading jet p$_{T}$ [GeV]',save='jet_pt0_vs_pt1.pdf')

filterCont = sigCont.filter(items=[settings.JET + 'pt_0', settings.JET + 'pt_1'])
rawPlots.plot_corr(filterCont, save='corr_filtered.pdf')
rawPlots.plot_corr(sigCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
rawPlots.plot_corr(sigCont, save='corr.pdf')

# # apply cuts
allCont = list(map(lambda x: x.cut('Pre'), allCont))

cutPlots = plots(savePrefix='pre', noLegName=True, savePlots=True)
cutPlots.plot_hist(allCont, settings.SF, (0, 1000), 40, xlabel='Scale factor', mode='stacked', save='sf.pdf')
cutPlots.plot_hist(allCont, 'minv', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', mode='stacked', save='minv.pdf')
cutPlots.plot_hist(allCont, settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', mode='stacked', save='lep_n.pdf')
cutPlots.plot_hist(allCont, settings.LEP + 'etot', (0, 1000), 40, xlabel='Lepton E$_{tot}$ [GeV]', mode='stacked', save='lep_etot.pdf')
cutPlots.plot_hist(allCont, settings.LEP + 'pt', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', mode='stacked', save='lep_pt.pdf')
cutPlots.plot_hist(allCont, settings.LEP + 'e', (0, 1000), 40, xlabel='Lepton E [GeV]', mode='stacked', save='lep_e.pdf')
cutPlots.plot_hist(allCont, settings.LEP + 'theta', (0, 3.2), 40, xlabel=r'Lepton $\theta$', mode='stacked', save='lep_theta.pdf')
cutPlots.plot_hist(allCont, settings.LEP + 'phi', (-3.2, 3.2), 40, xlabel='Lepton $\phi$', mode='stacked', save='lep_phi.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'n', (0, 6), 6, xlabel='N$_{jet}$', mode='stacked', save='jet_n.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', mode='stacked', save='jet_etot.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'theta_[\d]', (0, 3.2), 40, xlabel=r'Jet $\theta$', mode='stacked', save='jet_theta.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, xlabel='Jet $\phi$', mode='stacked', save='jet_phi.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode='stacked', save='jet_e.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'pt_0', (0, 400), 40, xlabel='Leading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_0.pdf')
cutPlots.plot_hist(allCont, settings.JET + 'pt_1', (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_1.pdf')

filterCont = sigCont.filter(items=[settings.JET + 'pt_0', settings.JET + 'pt_1'])
cutPlots.plot_corr(filterCont, save='corr_filtered.pdf')
cutPlots.plot_corr(sigCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
cutPlots.plot_corr(sigCont, save='corr.pdf')


# # apply cuts
allCont = list(map(lambda x: x.cut('Final'), allCont))

finalPlots = plots(savePrefix='fin', noLegName=True, savePlots=True)
finalPlots.plot_hist(allCont, settings.SF, (0, 1000), 40, xlabel='Scale factor', mode='stacked', save='sf.pdf')
finalPlots.plot_hist(allCont, 'minv', (0, 200), 40, xlabel='m$_{dijet}$ [GeV]', mode='stacked', save='minv.pdf')
finalPlots.plot_hist(allCont, settings.LEP + 'n', (0, 6), 6, xlabel='N$_{lep}$', mode='stacked', save='lep_n.pdf')
finalPlots.plot_hist(allCont, settings.LEP + 'etot', (0, 1000), 40, xlabel='Lepton E$_{tot}$ [GeV]', mode='stacked', save='lep_etot.pdf')
finalPlots.plot_hist(allCont, settings.LEP + 'pt', (0, 600), 40, xlabel='Lepton p$_{T}$ [GeV]', mode='stacked', save='lep_pt.pdf')
finalPlots.plot_hist(allCont, settings.LEP + 'e', (0, 1000), 40, xlabel='Lepton E [GeV]', mode='stacked', save='lep_e.pdf')
finalPlots.plot_hist(allCont, settings.LEP + 'theta', (0, 3.2), 40, xlabel=r'Lepton $\theta$', mode='stacked', save='lep_theta.pdf')
finalPlots.plot_hist(allCont, settings.LEP + 'phi', (-3.2, 3.2), 40, xlabel='Lepton $\phi$', mode='stacked', save='lep_phi.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'n', (0, 6), 6, xlabel='N$_{jet}$', mode='stacked', save='jet_n.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'etot', (0, 1000), 40, xlabel='Jet E$_{tot}$ [GeV]', mode='stacked', save='jet_etot.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'pt_[\d]', (0, 400), 40, xlabel='Jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'theta_[\d]', (0, 3.2), 40, xlabel=r'Jet $\theta$', mode='stacked', save='jet_theta.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'phi_[\d]', (-3.2, 3.2), 40, xlabel='Jet $\phi$', mode='stacked', save='jet_phi.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'e_[\d]', (0, 400), 40, xlabel='Jet E [GeV]', mode='stacked', save='jet_e.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'pt_0', (0, 400), 40, xlabel='Leading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_0.pdf')
finalPlots.plot_hist(allCont, settings.JET + 'pt_1', (0, 400), 40, xlabel='Subleading jet p$_{T}$ [GeV]', mode='stacked', save='jet_pt_1.pdf')

filterCont = sigCont.filter(items=[settings.JET + 'pt_0', settings.JET + 'pt_1'])
finalPlots.plot_corr(filterCont, save='corr_filtered.pdf')
finalPlots.plot_corr(sigCont.filter(regex='jet'), save='corr_filtered_regex.pdf')
finalPlots.plot_corr(sigCont, save='corr.pdf')

