import argparse
import functools
import os

import sklearn
import pandas as pd

from src import settings
from src.content import containers
from src.form import yields


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files, not on small test samples')
parser.add_argument("--maxevt", nargs='?', type=int, help="Specify number maximum number of events", default=None)
args = parser.parse_args()

maxEvtSig = None
maxEvtBkg = None
if args.maxevt:
    maxEvtSig = args.maxevt
    maxEvtBkg = args.maxevt

if args.full:
    settings.set_data_dir(settings.FULL_DATA_DIR)
    settings.set_plot_dir(settings.FULL_PLOT_DIR)
    settings.set_tex_dir(settings.FULL_TEX_DIR)
else:
    settings.set_data_dir(settings.EXAMPLE_DATA_DIR)
    settings.set_plot_dir(settings.EXAMPLE_PLOT_DIR)
    settings.set_tex_dir(settings.EXAMPLE_TEX_DIR)


if __name__ == '__main__':
    print('Starting the classification...')

    # load data
    allCont = []
    # signal
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Signal qqln'))
    # split off background from signal: W -> qqln from WW -> qqln
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.SIG_SAMPLE['csv'], xSec=settings.SIG_SAMPLE['xs'], maxEvt=maxEvtSig, name='Bkg qqln'))
    allCont[0] = allCont[0].cut('noSingleW', addName=False)
    allCont[1] = allCont[1].cut('singleW', addName=False)
    # other backgrounds
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQLL_SAMPLE['csv'], xSec=settings.QQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqll'))
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQLN_SAMPLE['csv'], xSec=settings.QQQQLN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqln'))
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQLL_SAMPLE['csv'], xSec=settings.QQQQLL_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqll'))
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQ_SAMPLE['csv'], xSec=settings.QQQQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqq'))
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQNN_SAMPLE['csv'], xSec=settings.QQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqnn'))
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQQQNN_SAMPLE['csv'], xSec=settings.QQQQNN_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qqqqnn'))
    allCont.append(containers.physics_container(settings.DATA_DIR + settings.QQ_SAMPLE['csv'], xSec=settings.QQ_SAMPLE['xs'], maxEvt=maxEvtBkg, name='Bkg qq'))

    yields.print_samples(allCont)

    print('Split into signal and background')

    sig_data = allCont[0].df
    bkg_data = functools.reduce(lambda x, y: x + y, allCont[1:]).df


    print(bkg_data.head())
    print(bkg_data.info())
