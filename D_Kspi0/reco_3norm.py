#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import basf2 as b2
import modularAnalysis as ma
from b2biiConversion import convertBelleMdstToBelleIIMdst, setupB2BIIDatabase
import json
import mdst_tools as mt


#General
os.environ['USE_GRAND_REPROCESS_DATA'] = '1'
reco_path = b2.create_path()

#if len(sys.argv) != 5:
#    sys.exit('Must provide two input parameters: [mc|data] [input_Belle_MDST_file][output_BelleII_ROOT_file].')

#Flag that determines which modes to reconstruct
rec_flag=int(sys.argv[1])
split_flags=list('{0:0b}'.format(rec_flag))

#MC or data?
mc_or_data = sys.argv[2].lower()
isMC = {"mc": True, "data": False}.get(mc_or_data, None)
if isMC is None:
    sys.exit('First parameter must be "mc" or "data" to indicate whether we run on MC or real data')
setupB2BIIDatabase(isMC)

if mc_or_data == 'mc':
        mc_kind=sys.argv[3].lower()
        #Deal with signal-MC
        if mc_kind == 'signal':
                input_dir = sys.argv[4]
                outputBelle2ROOTFile = sys.argv[5]

                inputBelleMDSTFiles=[]
                for filename in os.listdir(input_dir):
                        #if '55' in filename:
                        inputBelleMDSTFiles.append('%s/%s'%(input_dir,filename))
        
        #Deal with generic-MC
        if mc_kind == 'generic':
                expNo = sys.argv[4]
                startRun = sys.argv[5]
                endRun = sys.argv[6]
                eventType = sys.argv[7]
                dataType = sys.argv[8]
                belleLevel = sys.argv[9]
                streamNo = sys.argv[10]
                outputBelle2ROOTFile = sys.argv[11]
                inputBelleMDSTFiles = mt.getBelleMdst_mc(expNo, startRun, endRun, eventType, dataType, belleLevel, streamNo)



#Convert Belle -> BelleII
setupB2BIIDatabase(isMC)
convertBelleMdstToBelleIIMdst(inputBelleMDSTFiles, applyHadronBJSkim=True, path=reco_path)



# Reconstruction



#variable dictionary
var_dict = json.load(open("variables_3norm.json"))


tuple_vars = []
for key, val in var_dict['D_vars'].items():
        ma.variables.addAlias('%s_0' % key, '%s' % val)

for key, val in var_dict['Dst_vars'].items():
        ma.variables.addAlias('%s' % key, '%s' % val)
        tuple_vars += ['%s' % key]



#FSPs: get all tracks
ma.fillParticleList('pi+:all', '', path=reco_path)


#Define cuts
cuts_D='1.68 < D_mass_0 < 2.06'


cuts_Dst='0.1445 < DeltaM < 0.1465 and Dst_mom_st > 2.8 and 0.119 < pi0_mass < 0.151 and gam1_energy > 0.05 and gam2_energy > 0.05 and Ks_good == 1'


#Reconstruct D -> pi0 pi pi 
if int(split_flags[1]) == 1:


        #Rare D
        ma.reconstructDecay('D0:kspi0 -> pi0:mdst  K_S0:mdst', cut=cuts_D, path=reco_path)
        ma.reconstructDecay('D*+:kspi0 -> D0:kspi0 pi+:all',cut=cuts_Dst, path=reco_path)
        ma.matchMCTruth('D*+:kspi0', path=reco_path)
        ma.buildEventShape( default_cleanup=True, allMoments=False, cleoCones=True, collisionAxis=True, foxWolfram=True, harmonicMoments=True, jets=True, sphericity=True, thrust=True, path=reco_path)
        #ma.vertexTree('D*+:kspi0', ipConstraint=False, updateAllDaughters=False, conf_level=-2.0, path=reco_path)
        ma.variablesToNtuple('D*+:kspi0', tuple_vars, filename=outputBelle2ROOTFile, treename = 'tree1', path=reco_path)


# progress
progress = b2.register_module('Progress')
reco_path.add_module(progress)
b2.process(path=reco_path)
#b2.process(path=reco_path, max_event=5000)


# Print call statistics
print(b2.statistics)






