make
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/simple_40 ./main "40" "simple"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/mixed_40 ./main "40" "mixed"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/multshift_40 ./main "40" "multshift"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/poly2_40 ./main "40" "poly2"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/poly3_40 ./main "40" "poly3"


valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/simple_80 ./main "80" "simple"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/mixed_80 ./main "80" "mixed"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/multshift_80 ./main "80" "multshift"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/poly2_80 ./main "80" "poly2"
valgrind --tool=cachegrind --cachegrind-out-file=data/DS/cachemisses/poly3_80 ./main "80" "poly3"