#!/bin/sh

datapath=$1
module=$2
data=$3
bin=$4
codepath=`pwd`

make test_${module} > /dev/null

cd $datapath

info=()
while read l
do
    info+=( $l ); 
done < info.txt
u=${info[0]}
i=${info[1]}
p=${info[2]}

${codepath}/test_${module} $data $u $i $p $bin

cd $codepath
