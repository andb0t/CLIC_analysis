import argparse
import functools
import os

import sklearn
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from src import settings
from src.content import containers
from src.form import yields


parser = argparse.ArgumentParser()
parser.add_argument("--full", action="store_true", default=False, help='Execute on full data files, not on small test samples')
parser.add_argument("--maxevt", nargs='?', type=int, help="Specify number maximum number of events", default=None)
parser.add_argument("--verbose", action="store_true", default=False, help='Produce plots and intermediate info')
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

    if args.verbose:
        print(data.head())
        print(data.info())
        print(data['lep_n'].value_counts())
        print(data.describe())
        data.hist(bins=50, figsize=(20, 15))
        plt.savefig(os.path.join(settings.PLOT_DIR, 'class_all_raw_vars.pdf'))

    print('Reformat data for classification')
    data = np.concatenate((sig_data, bkg_data), axis=0)
    target = np.concatenate((np.full((len(sig_data), 1), True),
                             np.full((len(bkg_data), 1), False)),
                            axis=0)


    print('Prepare train and test samples')

    def split_shuffle_train_test(X, y, test_ratio):
        assert len(X) == len(y)
        shuffled_indices = np.random.permutation(len(X))
        test_set_size = int(len(X) * test_ratio)
        test_indices = shuffled_indices[:test_set_size]
        train_indices = shuffled_indices[test_set_size:]
        return np.take(X, train_indices), np.take(X, test_indices), np.take(y, train_indices), np.take(y, test_indices)

    X_train, X_test, y_train, y_test = split_shuffle_train_test(data, target, 0.2)

    if args.verbose:
        print('Lengths of training and test sets')
        print(list(map(len, [X_train, X_test, y_train, y_test])))
