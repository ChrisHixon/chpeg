#!/bin/sh

bytecode_output_c_pt2_compare() {
  ./bytecode_output_c_pt2_compare
}

compare_bytecode_with_internal() {
    ./compare_bytecode_with_internal ../grammars/chpeg-trim2.chpeg
}

count=0
fail=0
pass=0

tests="bytecode_output_c_pt2_compare compare_bytecode_with_internal"

for test in $tests; do
    if $test; then
        echo "PASS: $test"
        pass=$((pass + 1))
    else
        echo "FAIL: $test"
        fail=$((fail + 1))
    fi
    count=$((count + 1))
    echo '--------------------------------------------------------------------------------'
done

echo "TESTS:$count PASS:$pass FAIL:$fail"
