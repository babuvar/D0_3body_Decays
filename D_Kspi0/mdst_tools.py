#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import urllib.request


def getMdstfromLink(link):

	theFile = urllib.request.urlopen(link)
	content = str(theFile.read(), 'utf-8').split()
	return_list=[]
	for i in content:
		if 'group' in i and 'mdst' in i:
			return_list.append(i)
	return return_list


def getBelleMdst_mc(expNo, startRun, endRun, eventType, dataType, belleLevel, streamNo):

	link = 'http://bweb3/montecarlo.php?'+'ex=' + str(expNo) +'&rs=' + str(startRun) +'&re=' + str(endRun) +'&ty=' + eventType +'&dt=' + dataType +'&bl=' + belleLevel +'&st=' + str(streamNo)
	return getMdstfromLink(link)

def getBelleMdst_data(expNo, startRun, endRun, skimType, dataType, belleLevel):

	link = 'http://bweb3/mdst.php?'+'ex=' + str(expNo) + '&rs=' + str(startRun) + '&re=' + str(endRun) + '&skm=' + skimType +'&dt=' + dataType +'&bl=' + belleLevel
	return getMdstfromLink(link)















