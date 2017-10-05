import re
import warnings

import numpy as np

from . import lorentz


def calculate_mean(physCont, name):
    newRegex = re.sub('\_mean$', '', name)
    print('Calculate mean of names corresponding to regex', newRegex, 'for each event!')
    obsLists = list(physCont.get_list(regex=newRegex))
    with warnings.catch_warnings():
        warnings.filterwarnings('ignore', r'Mean of empty slice')
        mean = [np.nanmean(x) for x in zip(*obsLists)]
    mean = list(map(lambda x: x if x == x else 0, mean))
    return mean


def calculate_minv(physCont):
    jet = []
    jetPt = getattr(physCont.data, 'jet_DH_pt_0')
    jetTheta = getattr(physCont.data, 'jet_DH_theta_0')
    jetPhi = getattr(physCont.data, 'jet_DH_phi_0')
    jetE = getattr(physCont.data, 'jet_DH_e_0')
    jet.append(lorentz.lorentz(jetPt, jetTheta, jetPhi, jetE))
    # print(jet[0].m)
    jetPt = getattr(physCont.data, 'jet_DH_pt_1')
    jetTheta = getattr(physCont.data, 'jet_DH_theta_1')
    jetPhi = getattr(physCont.data, 'jet_DH_phi_1')
    jetE = getattr(physCont.data, 'jet_DH_e_1')
    jet.append(lorentz.lorentz(jetPt, jetTheta, jetPhi, jetE))
    # print(jet[1].m)
    return (jet[0] + jet[1]).m


def calculate_minvll(physCont):
    lep = []
    lepPt = getattr(physCont.data, 'lep_pt_0')
    lepTheta = getattr(physCont.data, 'lep_theta_0')
    lepPhi = getattr(physCont.data, 'lep_phi_0')
    lepE = getattr(physCont.data, 'lep_e_0')
    lep.append(lorentz.lorentz(lepPt, lepTheta, lepPhi, lepE))
    # print(lep[0].m)
    lepPt = getattr(physCont.data, 'lep_pt_1')
    lepTheta = getattr(physCont.data, 'lep_theta_1')
    lepPhi = getattr(physCont.data, 'lep_phi_1')
    lepE = getattr(physCont.data, 'lep_e_1')
    lep.append(lorentz.lorentz(lepPt, lepTheta, lepPhi, lepE))
    # print(lep[1].m)
    return (lep[0] + lep[1]).m