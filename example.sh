#!/bin/sh

echo "--------------------" >&2
echo "- DOWNLOADING ml1M -" >&2
echo "--------------------" >&2

wget http://files.grouplens.org/datasets/movielens/ml-1m.zip

unzip ml-1m.zip

mkdir -p ml1M/

echo "----------------------" >&2
echo "- CONVERTING DATASET -" >&2
echo "----------------------" >&2

cut -f1 -d: ml-1m/ratings.dat | sort -unr | head -n1 > ml1M/info.txt
cut -f3 -d: ml-1m/ratings.dat | sort -unr | head -n1 >> ml1M/info.txt
cat ml-1m/ratings.dat | wc -l >> ml1M/info.txt
echo "ml1M/info.txt"

sed 's/::/\t/g' ml-1m/ratings.dat \
    | awk '{print $1 - 1 "\t" $2 - 1 "\t" $3}' \
    | sort -k1,1n \
    > ml1M/ratings.u
echo "ml1M/ratings.u"

sed 's/::/\t/g' ml-1m/ratings.dat \
    | awk '{print $2 - 1 "\t" $1 - 1 "\t" $3}' \
    | sort -k1,1n \
    > ml1M/ratings.i
echo "ml1M/ratings.i"

rm -r ml-1m.zip ml-1m

echo "------------------------------" >&2
echo "- GENERATING RECOMMENDATIONS -" >&2
echo "------------------------------" >&2

./run_test.sh ml1M ub ratings 0 > ml1M/ub.txt
echo "ml1M/ub.txt"
./run_test.sh ml1M ib ratings 0 > ml1M/ib.txt
echo "ml1M/ib.txt"
./run_test.sh ml1M pop ratings 0 > ml1M/pop.txt
echo "ml1M/pop.txt"

