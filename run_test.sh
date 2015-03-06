#!/bin/sh

datapath=$1
module=$2
codepath=`pwd`

make test_${module}

cd $datapath

${codepath}/test_${module} `wc -l < u.txt` `wc -l < i.txt` `wc -l < total_u.txt`

cd $codepath
