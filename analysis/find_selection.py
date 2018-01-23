import argparse
import functools
import os

import sklearn
import sklearn.linear_model
import sklearn.tree
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

    # yields.print_samples(allCont)

    print('Apply preselection')
    allCont = list(map(lambda x: x.cut('Pre'), allCont))

    print('Split into signal and background')

    sig_data = allCont[0].df
    bkg_data = functools.reduce(lambda x, y: x + y, allCont[1:]).df

    if args.verbose:
        print(sig_data.head())
        print(sig_data.info())
        print(sig_data['lep_n'].value_counts())
        print(sig_data.describe())
        sig_data.hist(bins=50, figsize=(20, 15))
        plt.savefig(os.path.join(settings.PLOT_DIR, 'class_all_raw_sig_vars.pdf'))
        bkg_data.hist(bins=50, figsize=(20, 15))
        plt.savefig(os.path.join(settings.PLOT_DIR, 'class_all_raw_bkg_vars.pdf'))

    print('Deal with NaN values')

    def print_NaN_cols(data):
        data_nans = data.isnull().sum()
        data_nans = str(data_nans).split(sep='\n')
        data_nans = {d[0]: d[1] for d in map(lambda x: x.rsplit(' ', 1), data_nans) if d[1] != '0'}
        data_nans.pop('dtype:')
        print('The following columns contain NaN values:')
        print(*[(key, data_nans[key]) for key in sorted(data_nans)], sep='\n')

    print_NaN_cols(sig_data)

    if args.verbose:
        print('Before drop:')
        print(sig_data.info())

    cols_to_drop = ['lep_type_1', 'lep_type_2', 'lep_type_3', 'lep_type_4',
                    'lep_pt_1', 'lep_pt_2', 'lep_pt_3', 'lep_pt_4',
                    'lep_theta_1', 'lep_theta_2', 'lep_theta_3', 'lep_theta_4',
                    'lep_phi_1', 'lep_phi_2', 'lep_phi_3', 'lep_phi_4',
                    'lep_e_1', 'lep_e_2', 'lep_e_3', 'lep_e_4',
                    'lep_charge_1', 'lep_charge_2', 'lep_charge_3', 'lep_charge_4',
                    'Unnamed: 53']

    for col_name in cols_to_drop:
        sig_data.drop(col_name, axis=1, inplace=True)
        bkg_data.drop(col_name, axis=1, inplace=True)

    print_NaN_cols(sig_data)

    print('Drop scale factor column')
    sig_data_weights = sig_data[[settings.SF]]
    bkg_data_weights = bkg_data[[settings.SF]]
    sig_data.drop(settings.SF, axis=1, inplace=True)
    bkg_data.drop(settings.SF, axis=1, inplace=True)

    print('After dropping NaN columns:')
    print(sig_data.info())
    print(sig_data_weights.info())

    print('Reformat data for classification')

    data = np.concatenate((sig_data, bkg_data), axis=0)
    data_weights = np.concatenate((sig_data_weights, bkg_data_weights), axis=0)
    target = np.concatenate((np.full((len(sig_data), 1), True),
                             np.full((len(bkg_data), 1), False)),
                            axis=0)

    print('Prepare train and test samples')

    def split_shuffle_train_test(X, y, w=None, test_ratio=0.2, random_seed=1337):
        assert len(X) == len(y)
        np.random.seed(random_seed)
        shuffled_indices = np.random.permutation(len(X))
        test_set_size = int(len(X) * test_ratio)
        test_indices = shuffled_indices[:test_set_size]
        train_indices = shuffled_indices[test_set_size:]
        if w is None:
            return (X[train_indices, :],
                    X[test_indices, :],
                    np.take(y, train_indices),
                    np.take(y, test_indices),
                    None,
                    None)
        else:
            return (X[train_indices, :],
                    X[test_indices, :],
                    np.take(y, train_indices),
                    np.take(y, test_indices),
                    w[train_indices, :],
                    w[test_indices, :])

    X_train, X_test, y_train, y_test, w_train, w_test = split_shuffle_train_test(data, target, data_weights, 0.2)

    print('Lengths of training and test sets')
    print(list(map(len, [X_train, X_test, y_train, y_test, w_train, w_test])))
    print('The types and shapes of the train variables')
    print('X_train type:', type(X_train), ', shape:', X_train.shape)
    print('y_train type:', type(y_train), ', shape:', y_train.shape)
    print('w_train type:', type(w_train), ', shape:', w_train.shape)

    print('Train SGDClassifier')
    sgd_clf = sklearn.linear_model.SGDClassifier(random_state=1337)
    sgd_clf.fit(X_train, y_train)

    print('Predict single instance')
    prediction = sgd_clf.predict([X_test[1]])
    print(prediction, y_test[1])

    print('Train DecisionTreeClassifier')
    tree_clf = sklearn.tree.DecisionTreeClassifier(random_state=1337, max_depth=3)
    tree_clf.fit(X_train, y_train)

    print('Analyse decision tree model')
    sklearn.tree.export_graphviz(
        tree_clf,
        out_file='decision_tree.dot',
        feature_names=list(sig_data),
        class_names=['Bkg', 'Sig'],
        rounded=True,
        filled=True,
        )
    os.system('dot -Tpng decision_tree.dot -o decision_tree.png')

    # TODO: take scale factor out of training, add weights to samples
