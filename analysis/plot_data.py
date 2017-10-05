#!/usr/bin/env python
import importlib

from src.content import containers
from src.form import plots


MAX_EVT_SIG = 1000
MAX_EVT_BKG = 1000
SAVE_PLOTS = False


def main():
    global SAVE_PLOTS
    SAVE_PLOTS = True
    print('Plotting and saving data!')


if __name__ == '__main__':
    main()

importlib.reload(plots)
importlib.reload(containers)

sigCont = containers.physics_container(inputFile="example_data/3249.csv", maxEvt=MAX_EVT_SIG, name='Signal qqln')
bkg0Cont = containers.physics_container(inputFile="example_data/5572.csv", maxEvt=MAX_EVT_BKG, name='Bkg qqqqll')
bkg1Cont = containers.physics_container(inputFile="example_data/3246.csv", maxEvt=MAX_EVT_BKG, name='Bkg qqll')
# sigCont.show()
allCont = sigCont + bkg0Cont + bkg1Cont
allCont.name = 'Total'

importlib.reload(containers)
cutCont = sigCont.cut('stdCuts')
plots.plot_hist([sigCont, cutCont], 'lep_n', (0, 5), 5, normed=0)

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
# plots.plot_corr(sigCont, save='corr.pdf')
# plots.plot_corr(bkg0Cont)
# plots.plot_corr(bkg1Cont)

plots.plot_hist([allCont, sigCont, bkg0Cont, bkg1Cont], 'minv', (0, 200), 40, save='minv.pdf', normed=0)
plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'minvll', (0, 200), 40, save='minvll.pdf')
plots.plot_hist(sigCont, 'jet_DH_pt_[01]', (0, 50), 40, save='pt.pdf')
plots.plot_hist(sigCont, 'jet_DH_theta_[01]', (0, 5), 40, save='theta.pdf')
plots.plot_hist(sigCont, 'jet_DH_phi_[01]', (-4, 4), 40, save='phi.pdf')
plots.plot_hist(sigCont, 'jet_DH_e_[01]', (0, 100), 40, save='e.pdf')
