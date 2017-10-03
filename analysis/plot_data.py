#!/usr/bin/env python
import importlib

import containers
import plots

MAX_EVT_SIG = 100
MAX_EVT_BKG = 20

importlib.reload(containers)
importlib.reload(plots)
fileName = "example_data/test_sig_sample.csv"
sigCont = containers.physics_container(fileName, MAX_EVT_SIG, name='Signal')
# sigCont.show()
fileName = "example_data/test_bkg_sample.csv"
bkgCont = containers.physics_container(fileName, MAX_EVT_BKG, name='Bkg')


plots.plot_raw(sigCont, '_n')
plots.plot_raw(sigCont, 'lep_pt')
plots.plot_raw(sigCont, 'lep_pt_mean')
plots.plot_hist(sigCont, 'lep_pt_mean')
plots.plot_raw(sigCont, '\w*_n')
plots.plot_hist(sigCont, '\w*_n', (0, 10), 10)
plots.plot_hist(sigCont, 'lep_pt', (0, 300))
plots.plot_hist(sigCont, 'lep_pt', (0, 300), stacked=True)
plots.plot_hist(sigCont, 'lep_pt', (0, 300), stacked=True, chained=True)

plots.plot_raw([sigCont, bkgCont], 'lep_n')
plots.plot_hist([sigCont, bkgCont], 'lep_n', (0, 10), 10)
plots.plot_hist([sigCont, bkgCont], 'lep_n', (0, 10), 10, stacked=True)
plots.plot_hist([sigCont, bkgCont], 'lep_n', (0, 10), 10, stacked=True, chained=True)

plots.plot_corr(sigCont)
plots.plot_corr(bkgCont)
