import importlib

import containers
import plots


MAX_EVT = 100


# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


importlib.reload(containers)
importlib.reload(plots)
fileName = "all_output_small.txt"
sigCont = containers.physics_container(fileName, MAX_EVT, verbose=1, name='Signal')
# physCont.show()
fileName = "all_output_small.txt"
bkgCont = containers.physics_container(fileName, MAX_EVT, verbose=1, name='Bkg')

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
plots.plot_hist(sigCont, 'lep_n', (0, 10), 10)
plots.plot_hist([sigCont, bkgCont], 'lep_n', (0, 10), 10, stacked=True)
plots.plot_hist([sigCont, bkgCont], 'lep_n', (0, 10), 10, stacked=True, chained=True)
