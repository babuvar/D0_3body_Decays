#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import json

os.environ['PGUSER'] = 'g0db'
os.environ['BELLE_POSTGRESS_SERVER'] = 'can01'
os.environ['USE_GRAND_REPROCESS_DATA'] = '1'
os.environ['LD_LIBRARY_PATH'] = '/sw/belle/local/neurobayes-4.3.1/lib/:%s' %(os.getenv('LD_LIBRARY_PATH'))

#------------------------------------------------------------
#-------------------    Signal-MC JOBS    -------------------
#------------------------------------------------------------

job_type='signal'
#job_type='generic'

if job_type=='signal':

	mdst_loca='/home/belle/varghese/DESY/BASF/Analysis/mcproduzh/gsim/mdst/'
	sig_mc_dict={	"D_pi0pipi" : 	{
					#"dirs" : ["D_pi0Ks_5K","Db_pi0Ks_5K"],
					"dirs" : ["D_pi0Ks_500K","Db_pi0Ks_500K"],
					"rec_flag" : 12
					}
	}


	modes=['D_pi0pipi']
	#modes=[]

	for mode in modes:
		for i in range(2):

			os.system('bsub -q s -oo log/signal_mc/%s.log'%sig_mc_dict[mode]['dirs'][i] +' basf2 -l INFO reco_3norm.py  %s' %sig_mc_dict[mode]['rec_flag'] + ' mc signal ' + '%s%s' %(mdst_loca,sig_mc_dict[mode]['dirs'][i]) +  ' root/signal_mc/%s.root' %sig_mc_dict[mode]['dirs'][i])




#------------------------------------------------------------
#-------------------    Signal-MC JOBS    -------------------
#------------------------------------------------------------




#------------------------------------------------------------
#-------------------    Generic-MC JOBS    ------------------
#------------------------------------------------------------

if job_type=='generic':

	rec_flag=12 
	#expNos = ['49']
	#expNos = ['7', '9', '11', '13', '15', '17', '19', '21', '23', '25', '27', '55'] #for congam (tree2)
	expNos = ['11'] #for testing
	#startRun = 1
	#endRun = 99

	eventTypes = ['evtgen-charm','evtgen-charged','evtgen-mixed','evtgen-uds']
	#eventTypes = ['evtgen-charm']


	dataType = 'on_resonance'
	belleLevel = 'caseB'


	#Streams = [4, 5]
	Streams = [0]

	run_list = json.load(open("runlist.json"))


	for StreamNo in Streams:

		for expNo, val2 in run_list[dataType].items():
		#for expNo in expNos:
			if dataType == 'on_resonance' and int(expNo) <= 27:
				streamNo=int('1%s'%StreamNo)
			else:
				streamNo=StreamNo
			for startRun, endRun in run_list[dataType][expNo].items():

				for eventType in eventTypes:
					filename= 'exp%s_rs%s_re%s_%s_%s_%s_st%s'%(expNo, startRun, endRun, eventType, dataType, belleLevel, streamNo)


					os.system('bsub -q l -oo log/generic_mc/stream_%s/%s.log'%(streamNo,filename) +' basf2 -l INFO reco_3norm.py  %s'%rec_flag + ' mc generic ' + ' %s '%expNo + ' %s '%startRun + ' %s '%endRun + ' %s '%eventType + ' %s '%dataType + ' %s '%belleLevel + ' %s '%streamNo + ' root/generic_mc/stream_%s/%s.root'%(streamNo,filename) )



#------------------------------------------------------------
#-------------------    Generic-MC JOBS    ------------------
#------------------------------------------------------------















