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
#convertBelleMdstToBelleIIMdst(inputBelleMDSTFiles, path=reco_path)


# Reconstruction



#variable dictionary
#var_dict = json.load(open("variables_3body.json"))
var_dict = json.load(open("variables_3body_test2.json"))


tuple_vars = []
for key, val in var_dict['D_vars'].items():
        ma.variables.addAlias('%s_0' % key, '%s' % val)

for key, val in var_dict['Dst_vars'].items():
        ma.variables.addAlias('%s' % key, '%s' % val)
        tuple_vars += ['%s' % key]


tuple_vars_cg = []
#var_dict_cg = json.load(open("variables_congam.json"))
var_dict_cg = json.load(open("variables_congam_test.json"))
for key, val in var_dict_cg['congam_vars'].items():
        ma.variables.addAlias('%s_cg' % key, '%s' % val)
        tuple_vars_cg += ['%s_cg' % key]


#FSPs: get all tracks
ma.fillParticleList('pi+:all', '', path=reco_path)
#ma.fillParticleList('K+:all', '', path=reco_path)
ma.fillParticleList('e+:all', '', path=reco_path)


#Define cuts
cuts_D='1.68 < D_mass_0 < 2.06 and 0.119 < pi0_mass_0 < 0.151 and gam1_energy_0 > 0.05 and gam2_energy_0 > 0.05 and e1_PID_0 > 0.4 and e2_PID_0 > 0.4'


#cuts_Dst='0.1445 < DeltaM < 0.1465 and Dst_mom_st > 2.5'
cuts_Dst='0.138 < DeltaM < 0.155 and Dst_mom_st > 2.5'


#Reconstruct D -> pi0 e e 
if int(split_flags[1]) == 1:

        #Converted photons
        #ma.matchMCTruth('gamma:v0mdst', path=reco_path)
        #ma.copyParticles('gamma:kfitter', 'gamma:v0mdst', path=reco_path)
        #ma.vertexTree('gamma:kfitter', ipConstraint=False, updateAllDaughters=False, conf_level=-2.0, path=reco_path)
        ##ma.variablesToNtuple('gamma:v0mdst', tuple_vars_cg, filename=outputBelle2ROOTFile, treename = 'tree2', path=reco_path)


        #Brem-recovery
        ma.cutAndCopyLists('e-:trim', 'e-:all', 'eIDBelle > 0.4', path=reco_path)
        ma.cutAndCopyLists('e+:trim', 'e+:all', 'eIDBelle > 0.4', path=reco_path)
        ma.cutAndCopyLists('gamma:trim', 'gamma:mdst','E > 0.05', path=reco_path)
        #ma.correctFSR('e-:brem', 'e-:trim', 'gamma:trim', angleThreshold=2.9, energyThreshold=1.5, path=reco_path)
        ma.correctBremsBelle('e-:brem', 'e-:trim', 'gamma:trim',True, 0.05, 0.05,False, path=reco_path)
        #ma.correctBremsBelle('e+:brem', 'e+:trim', 'gamma:trim',True, 0.05, 0.05,False, path=reco_path)

        #Rare D
        ma.reconstructDecay('D0:pi0ee -> pi0:mdst e+:all e-:all', cut=cuts_D, path=reco_path)
        #ma.vertexKFit('D0:pi0ee', decay_string='D0 -> ^e- ^e+', conf_level=-1.1, path=reco_path)
        #ma.vertexTree('D0:pi0ee', conf_level= -2.0, updateAllDaughters=False, ipConstraint=False, path=reco_path)
        ma.reconstructDecay('D*+:pi0ee -> D0:pi0ee pi+:all',cut=cuts_Dst, path=reco_path)
        ma.matchMCTruth('D*+:pi0ee', path=reco_path)
        ma.buildEventShape( default_cleanup=True, allMoments=False, cleoCones=True, collisionAxis=True, foxWolfram=True, harmonicMoments=True, jets=True, sphericity=True, thrust=True, path=reco_path)
        #ma.vertexTree('D*+:pi0ee', ipConstraint=False, updateAllDaughters=False, conf_level=-2.0, path=reco_path)
        ma.variablesToNtuple('D*+:pi0ee', tuple_vars, filename=outputBelle2ROOTFile, treename = 'tree1', path=reco_path)


        ma.reconstructDecay('D0:brem -> pi0:mdst e+:brem e-:brem', cut=cuts_D, path=reco_path)
        ma.reconstructDecay('D*+:brem -> D0:brem pi+:all',cut=cuts_Dst, path=reco_path)
        ma.matchMCTruth('D*+:brem', path=reco_path)
        ma.variablesToNtuple('D*+:brem', tuple_vars, filename=outputBelle2ROOTFile, treename = 'tree1brem', path=reco_path)



# progress
progress = b2.register_module('Progress')
reco_path.add_module(progress)

b2.process(path=reco_path)
#b2.process(path=reco_path, max_event=500)


# Print call statistics
print(b2.statistics)






