#!/usr/bin/env python
import importlib

import containers
import plots

MAX_EVT_SIG = 100
MAX_EVT_BKG = 100

importlib.reload(containers)
importlib.reload(plots)
fileName = "example_data/3249.csv"
sigCont = containers.physics_container(fileName, MAX_EVT_SIG, name='Signal qqln')
# sigCont.show()
fileName = "example_data/5572.csv"
bkg0Cont = containers.physics_container(fileName, MAX_EVT_BKG, name='Bkg qqqqll')
fileName = "example_data/3246.csv"
bkg1Cont = containers.physics_container(fileName, MAX_EVT_BKG, name='Bkg qqll')

plots.plot_raw(sigCont, '_n')
plots.plot_raw(sigCont, 'lep_pt')
plots.plot_raw(sigCont, 'lep_pt_mean')
plots.plot_hist(sigCont, 'lep_pt_mean')
plots.plot_raw(sigCont, '\w*_n')
plots.plot_hist(sigCont, '\w*_n', (0, 10), 10)
plots.plot_hist(sigCont, 'lep_pt', (0, 300))
plots.plot_hist(sigCont, 'lep_pt', (0, 300), stacked=True)
plots.plot_hist(sigCont, 'lep_pt', (0, 300), stacked=True, chained=True)

plots.plot_raw([sigCont, bkg0Cont, bkg1Cont], 'lep_n')
plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10)
plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, stacked=True)
plots.plot_hist([sigCont, bkg0Cont, bkg1Cont], 'lep_n', (0, 10), 10, stacked=True, chained=True)

plots.plot_corr(sigCont)
plots.plot_corr(bkg0Cont)
plots.plot_corr(bkg1Cont)
