#!/usr/bin/env python

'''
Script to submit VLC jobs to the cluster

Created on Jan 27, 2017
'''

import subprocess

def main():

    mainPath = "/afs/cern.ch/user/l/lstroem/clicdp/analysis/steering/chain/topasymmetry_wflavourtag/submit/"

    sixFermionSample = True
    nrFilesPerJob = 1
    radius = 1.5
    beta = 1.0
    gamma = 1.0
    deltaR = 0.05
    deltaP = 0.05
    cos_theta_W_max = 1.0
    
    #[6589, 6592, 6595, 6598, 6601, 6604, 6607, 6610, 6613, 6616, 6619, 6622, 6634, 6637] #P(e-)=-80%
    #[6646, 6652, 6655, 6658, 6661, 6664, 6667, 6670, 6673, 6676, 6679, 6691, 6694, 6697] #P(e+)=+80%
    for prodID in [6589, 6592, 6595, 6598, 6601, 6604, 6607, 6610, 6613, 6616, 6619, 6622, 6637]:
        suffix = "run1_%s_1jun2017"%prodID

        callStr = 'python {path}submit_grid_topasymmetry_chain.py --prodID={prodID:04.0f} --suffix={suffix} --sixFermionSample={sixFermionSample} --nrFilesPerJob={nrFilesPerJob:d} --jetRadius={radius:.1f} --jetBeta={beta:.1f} --jetGamma={gamma:.1f} --deltaR={deltaR:.2f} --deltaP={deltaP:.2f} --cos_theta_W_max={cos_theta_W_max:.2f}'.format(path = mainPath, prodID = prodID, suffix = suffix, sixFermionSample = sixFermionSample, nrFilesPerJob = nrFilesPerJob, radius = radius, beta = beta, gamma = gamma, deltaR = deltaR, deltaP = deltaP, cos_theta_W_max = cos_theta_W_max)
        print(callStr)
        subprocess.call(callStr, shell = True)
    
if __name__ == "__main__":
    main()



