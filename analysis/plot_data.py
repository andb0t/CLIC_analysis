import importlib

import containers
import plots


MAX_EVT = 100


# tells matplotlib how to show you the plots, there are multiple differnt options in addition to 'inline'
get_ipython().magic('matplotlib inline')


importlib.reload(containers)
fname = "all_output_small.txt"
physCont = containers.physics_container(fname, MAX_EVT, verbose=1)
# physCont.show()

plots.plot_raw(physCont, '_n')
plots.plot_raw(physCont, 'lep_pt')
plots.plot_raw(physCont, 'lep_pt_mean')
plots.plot_hist(physCont, 'lep_pt_mean')
plots.plot_raw(physCont, '\w*_n')
plots.plot_hist(physCont, '\w*_n', (0, 10), 10)
plots.plot_hist(physCont, 'lep_pt', (0, 300))
plots.plot_hist(physCont, 'lep_pt', (0, 300), stacked=True)
plots.plot_hist(physCont, 'lep_pt', (0, 300), stacked=True, chained=True)
