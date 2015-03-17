#!/bin/sh

datapath=$1
module=$2
data=$3
bin=$4
codepath=`pwd`

make test_${module}

cd $datapath

${codepath}/test_${module} $data `wc -l < u.txt` `wc -l < i.txt` `wc -l < total_u.txt` $bin

cd $codepath
