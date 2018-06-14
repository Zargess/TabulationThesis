make
valgrind --tool=cachegrind --cachegrind-out-file=data/cachemissessingleds/linear_40 ./main "40" "linear"
valgrind --tool=cachegrind --cachegrind-out-file=data/cachemissessingleds/double_40 ./main "40" "double"
valgrind --tool=cachegrind --cachegrind-out-file=data/cachemissessingleds/cuckoo_40 ./main "40" "cuckoo"