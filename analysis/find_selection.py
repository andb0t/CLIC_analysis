import argparse
import functools
import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sklearn
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import SGDClassifier
from sklearn.metrics import confusion_matrix, precision_score, recall_score, f1_score
from sklearn.tree import DecisionTreeClassifier
from sklearn.tree import export_graphviz
from sklearn.utils import shuffle
import tabulate

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

settings.init_globals(args.full)

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

    if args.verbose:
        print('Before drop:')
        print(sig_data.info())


    def get_NaN_cols(data):
        data_nans = data.isnull().sum()
        data_nans = str(data_nans).split(sep='\n')
        data_nans = {d[0]: d[1] for d in map(lambda x: x.rsplit(' ', 1), data_nans) if d[1] != '0'}
        data_nans.pop('dtype:')
        print('The following columns contain NaN values:')
        nan_cols_counts = [(key, data_nans[key]) for key in sorted(data_nans)]
        nan_cols_counts = list(map(lambda x: (x[0].strip(), x[1]), nan_cols_counts))
        print(*nan_cols_counts, sep='\n')
        nan_cols = list(map(lambda x: x[0], nan_cols_counts))
        return nan_cols

    cols_to_drop = get_NaN_cols(sig_data)
    print('Drop NaN containing columns', cols_to_drop)
    for col_name in cols_to_drop:
        sig_data.drop(col_name, axis=1, inplace=True)
        bkg_data.drop(col_name, axis=1, inplace=True)

    sig_data_weights = sig_data[[settings.SF]]
    bkg_data_weights = bkg_data[[settings.SF]]
    print('Drop scale factor column', settings.SF)
    sig_data.drop(settings.SF, axis=1, inplace=True)
    bkg_data.drop(settings.SF, axis=1, inplace=True)

    cols_to_drop = []
    cols_to_drop.extend([name for name in list(sig_data) if name.startswith('mc_')])
    cols_to_drop.extend([name for name in list(sig_data) if name.startswith('beam_')])
    print('Drop truth columns', cols_to_drop)
    for col_name in cols_to_drop:
        sig_data.drop(col_name, axis=1, inplace=True)
        bkg_data.drop(col_name, axis=1, inplace=True)

    print('Final columns for training:')
    print('Data', sig_data.info())
    print('Weights', sig_data_weights.info())

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
                    None,
                    )
        else:
            return (X[train_indices, :],
                    X[test_indices, :],
                    np.take(y, train_indices),
                    np.take(y, test_indices),
                    w[train_indices, :][:, 0],
                    w[test_indices, :][:, 0],
                    )

    X_train, X_test, y_train, y_test, w_train, w_test = split_shuffle_train_test(data, target, data_weights, 0.2)

    print('Lengths of training and test sets')
    print(list(map(len, [X_train, X_test, y_train, y_test, w_train, w_test])))
    print('The types and shapes of the train variables')
    print('X_train type:', type(X_train), ', shape:', X_train.shape)
    print('y_train type:', type(y_train), ', shape:', y_train.shape)
    print('w_train type:', type(w_train), ', shape:', w_train.shape)

    print('Train SGDClassifier')
    sgd_clf = SGDClassifier(random_state=1337)
    sgd_clf.fit(X_train, y_train, sample_weight=w_train)

    print('Predict single instance')
    prediction = sgd_clf.predict([X_test[1]])
    print(prediction, y_test[1])

    print('Train DecisionTreeClassifier')
    tree_clf = DecisionTreeClassifier(random_state=1337, max_depth=3)
    tree_clf.fit(X_train, y_train, sample_weight=w_train)

    print('Analyse decision tree model')
    export_graphviz(
        tree_clf,
        out_file='decision_tree.dot',
        feature_names=list(sig_data),
        class_names=['Bkg', 'Sig'],
        rounded=True,
        filled=True,
        )
    os.system('dot -Tpng decision_tree.dot -o decision_tree.png')

    print('Train RandomForestClassifier')
    forest_clf = RandomForestClassifier(random_state=1337)
    forest_clf.fit(X_train, y_train, sample_weight=w_train)

    print('Evaluate the classifiers using cross-validation')

    def cross_val_score_weighted(clf, X_train, y_train, w_train, cv=3):
        X_shuff, y_shuff, w_shuff = shuffle(X_train, y_train, w_train, random_state=1337)
        split_bounds = [int(x / cv * len(X_shuff)) for x in range(cv) if x != 0]
        X_splits = np.split(X_shuff, split_bounds)
        y_splits = np.split(y_shuff, split_bounds)
        w_splits = np.split(w_shuff, split_bounds)
        score_precision = 0
        score_recall = 0
        score_f1 = 0
        score_conf_matrix = np.zeros((2, 2))
        for idx in range(cv):
            X_sp_test = X_splits[idx]
            y_sp_test = y_splits[idx]
            w_sp_test = w_splits[idx]
            train_indices = [i for i in range(cv) if i != idx]
            X_sp_train = np.concatenate(list((X_splits[i] for i in train_indices)), axis=0)
            y_sp_train = np.concatenate(list((y_splits[i] for i in train_indices)), axis=0)
            w_sp_train = np.concatenate(list((w_splits[i] for i in train_indices)), axis=0)
            clf.fit(X_sp_train, y_sp_train, sample_weight=w_sp_train)
            y_sp_pred = clf.predict(X_sp_test)
            precision = precision_score(y_sp_test, y_sp_pred, sample_weight=w_sp_test)
            recall = recall_score(y_sp_test, y_sp_pred, sample_weight=w_sp_test)
            f1 = f1_score(y_sp_test, y_sp_pred, sample_weight=w_sp_test)
            conf_matrix = confusion_matrix(y_sp_test, y_sp_pred, sample_weight=w_sp_test)
            score_precision += precision
            score_recall += recall
            score_f1 += f1
            score_conf_matrix += conf_matrix
        score_precision /= cv
        score_recall /= cv
        score_f1 /= cv
        score_conf_matrix /= cv
        result_dict = {
            'precision': np.round(score_precision, 3),
            'recall': np.round(score_recall, 3),
            'f1': np.round(score_f1, 3),
            'confusion': np.round(score_conf_matrix, 3),
            }
        return result_dict

    table = []
    headers = []
    for clf in (sgd_clf, tree_clf, forest_clf):
        cvs = cross_val_score_weighted(clf, X_train, y_train, w_train, cv=3)
        sorted_keys = sorted(cvs.keys())
        if not headers:
            headers = sorted_keys
        sorted_vals = [clf.__class__.__name__]
        sorted_vals.extend([cvs[key] for key in sorted_keys])
        table.append(sorted_vals)
    print(tabulate.tabulate(table, headers=headers, tablefmt='grid'))
