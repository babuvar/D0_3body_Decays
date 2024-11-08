#!/bin/bash

function usage {
    cat <<EOF

This script returns process_event commands for the skimmed MC. Empty index files are skipped.

Usage:
  $0 module data_type event_type stream exp [nfiles skip]
    modules are listed in the modules-*.txt files
    data_type = on_resonance, continuum, 5S_onresonance
    event_type = charged, mixed, uds, charm, bsbs, nonbsbs
    nfiles = return only [nfiles] files
    skip  = skip the first [nfiles]*[skip] files

Examples:
  $0 dhcp on_resonance charged 0 65
  $0 dhcp on_resonance charged 0 65 10 0
  $0 dhcp on_resonance charged 0 65 10 1

EOF
    exit
}

[ $# -ne 5 -a $# -ne 7 ] && usage

module=$1
datatype=$2
eventtype=$3
stream=$4
exp=$5

limitoffsetsql=""
if [ ! -z "$7" ];then
offset=$((  $6 * $7 ))
limitoffsetsql="LIMIT $6 OFFSET $offset"
fi

sqlitedb=/group/belle/g96home/g0mc/skimming/indexdb/databases/mc-${module}.db

if [ ! -e $sqlitedb ];then
echo "DB not found for module $module: $module doesn't exist probably."
exit
fi

# changed 04/10/2010 - Philipp Hamer
# work-around to fix bfarm -> KEKCC pathes
# should be fixed in SQLite db soon..

#sqlite3 $sqlitedb "SELECT 'process_event ' || path || '/' || filename || ' 0' FROM files WHERE skim='$module' AND exp=${exp} AND stream=${stream} AND event_type='${eventtype}' AND data_type='${datatype}' AND nevent!=0 ORDER BY run0 $limitoffsetsql "

for event in `sqlite3 $sqlitedb "SELECT path || '/' || filename FROM files WHERE skim='$module' AND exp=${exp} AND stream=${stream} AND event_type='${eventtype}' AND data_type='${datatype}' AND nevent!=0 ORDER BY run0 $limitoffsetsql "`; do

kekccevent=`echo $event | sed -e "s/.*g0mc/\/group\/belle\/bdata-files\/g0mc/"`

echo "process_event ${kekccevent} 0"

done
