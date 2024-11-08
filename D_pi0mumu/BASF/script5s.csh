#! /bin/tcsh -f
                        
cd /gpfs/fs02/belle/users/varghese/PhiGammaAsymMC_Simple

source ~/Env/cshrc.pre
setenv BASF_USER_INIT geant_init

setenv FILE 5s_mc_$1_s$2_exp$3_op

basf <<EOF >! ./log$4/log_$FILE.out

path create main
path create Skim

module register fix_mdst d0rad
path add_module main fix_mdst
path add_module Skim d0rad

path add_condition main >:0:Skim
path add_condition main =<:0:KILL

initialize

histogram define ./hbook$4Y5S/$FILE.hbk  

`/gpfs/home/belle/nishida6/public/fileloc/jwicht_script/skim-process_event-mc.sh d0rad 5S_onresonance $1 $2 $3`
 
terminate 

EOF






