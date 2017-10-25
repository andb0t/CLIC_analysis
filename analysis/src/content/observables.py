import re
import warnings

import numpy as np

from src.content import lorentz
from src import settings


keywords = ['mean', 'mjj', 'mll', 'mln']


def calculate_mean(physCont, name):
    newRegex = re.sub('\_mean$', '', name)
    print('Calculate mean of names corresponding to regex', newRegex, 'for each event!')
    obsLists = list(physCont.get_list(regex=newRegex))
    with warnings.catch_warnings():
        warnings.filterwarnings('ignore', r'Mean of empty slice')
        mean = [np.nanmean(x) for x in zip(*obsLists)]
    mean = list(map(lambda x: x if x == x else 0, mean))
    return mean


def calculate_mjj(physCont):
    jet = []
    jetPt = getattr(physCont.df, settings.JET + 'pt_0')
    jetTheta = getattr(physCont.df, settings.JET + 'theta_0')
    jetPhi = getattr(physCont.df, settings.JET + 'phi_0')
    jetE = getattr(physCont.df, settings.JET + 'e_0')
    jet.append(lorentz.lorentz(jetPt, jetTheta, jetPhi, jetE))
    jetPt = getattr(physCont.df, settings.JET + 'pt_1')
    jetTheta = getattr(physCont.df, settings.JET + 'theta_1')
    jetPhi = getattr(physCont.df, settings.JET + 'phi_1')
    jetE = getattr(physCont.df, settings.JET + 'e_1')
    jet.append(lorentz.lorentz(jetPt, jetTheta, jetPhi, jetE))
    return (jet[0] + jet[1]).m


def calculate_mll(physCont):
    lep = []
    lepPt = getattr(physCont.df, settings.LEP + 'pt_0')
    lepTheta = getattr(physCont.df, settings.LEP + 'theta_0')
    lepPhi = getattr(physCont.df, settings.LEP + 'phi_0')
    lepE = getattr(physCont.df, settings.LEP + 'e_0')
    lep.append(lorentz.lorentz(lepPt, lepTheta, lepPhi, lepE))
    lepPt = getattr(physCont.df, settings.LEP + 'pt_1')
    lepTheta = getattr(physCont.df, settings.LEP + 'theta_1')
    lepPhi = getattr(physCont.df, settings.LEP + 'phi_1')
    lepE = getattr(physCont.df, settings.LEP + 'e_1')
    lep.append(lorentz.lorentz(lepPt, lepTheta, lepPhi, lepE))
    return (lep[0] + lep[1]).m


def calculate_mln(physCont):
    lepPt = getattr(physCont.df, settings.LEP + 'pt_0')
    lepTheta = getattr(physCont.df, settings.LEP + 'theta_0')
    lepPhi = getattr(physCont.df, settings.LEP + 'phi_0')
    lepE = getattr(physCont.df, settings.LEP + 'e_0')
    lep = lorentz.lorentz(lepPt, lepTheta, lepPhi, lepE)

    missPt = getattr(physCont.df, settings.MISS + 'pt')
    missTheta = getattr(physCont.df, settings.MISS + 'theta')
    missPhi = getattr(physCont.df, settings.MISS + 'phi')
    missE = getattr(physCont.df, settings.MISS + 'e')
    miss = lorentz.lorentz(missPt, missTheta, missPhi, missE)
    return (lep + miss).m
