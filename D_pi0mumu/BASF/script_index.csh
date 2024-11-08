#! /bin/tcsh -f

setenv BELLE_LEVEL b20090127_0910
setenv BASF_USER_IF basfsh.so
setenv BASF_NPROCESS 0
setenv BELLE_DEBUG opt
setenv USE_GRAND_REPROCESS_DATA 1
setenv BASF_USER_INIT geant_init

source /sw/belle/local/etc/cshrc_general

setenv FILE DtoKsPi_index


basf <<EOF >! ./log/$FILE.out


path create main
path create Skim

module register fix_mdst d0rad
path add_module main fix_mdst
path add_module Skim d0rad

path add_condition main >:0:Skim
path add_condition main =<:0:KILL

initialize

histogram define ./hbook/$FILE.hbk  

process_event  /group/belle/g96home/g0mc/skimming/skim4S/index/d0rad/on_resonance/e000007/evtgen-charm/s10/d0rad-e000007r000006r000872-s10-evtgen-charm-on_resonance-b20090127_0910.index 0

terminate 

EOF






