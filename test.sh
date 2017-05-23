#!/bin/sh

filename="$1"
format="%9s  %20s  %7s  %6s  %9s\n"
printf "$format" "HTTP Code" "Value" "isPrime" "Time" "Pass/Fail"
while read -r line
do
    int="$(echo $line | cut -d' ' -f1)"
    expect="$(echo $line | cut -d' ' -f2)"
    out=$(curl --silent --write-out "\n%{http_code}\n%{time_total}s" -X POST http://localhost:9080/isprime/$int)
    result="$(echo $out | cut -d' ' -f1)"
    time="$(echo $out | cut -d' ' -f3)"
    code="$(echo $out | cut -d' ' -f2)"
    if [ "$result" = "$expect" ]; then 
        passfail="PASS"  
    else
        passfail="FAIL"  
    fi;
    printf "$format" $code $int $result $time $passfail
done < "$filename"

