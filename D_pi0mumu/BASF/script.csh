#! /bin/tcsh -f

setenv BELLE_LEVEL b20090127_0910
setenv BASF_USER_IF basfsh.so
setenv BASF_NPROCESS 0
setenv BELLE_DEBUG opt
setenv USE_GRAND_REPROCESS_DATA 1
setenv BASF_USER_INIT geant_init

source /sw/belle/local/etc/cshrc_general

foreach FILE ( D_pi0mumu_50K Db_pi0mumu_50K )
#setenv FILE output


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

process_dir  /home/belle/varghese/DESY/BASF/Analysis/mcproduzh/gsim/mdst/$FILE/

terminate 

EOF

end




