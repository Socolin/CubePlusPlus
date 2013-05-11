# This script fix -Wunused-parameter
# $1 = gcc output file generate with: make 2&>1 | tee /tmp/loggcc
while read line
do
    TMP=`echo $line | grep 'warning: unused parameter' >/dev/null`
    TEST=`echo test $? -eq 0`
    if $TEST
    then
        LINE=`echo $line | sed -r 's/.*[hp]:([0-9]+).*/\1/g'`
        VARIABLE=`echo $line | sed -r 's/.*parameter ‘(.*)’.*/\1/g'`
        FILE=`echo $line | sed -r 's/(.*[hp]):.*/\1/g'`
        echo $FILE
        sed -ir "${LINE}s@ ${VARIABLE}\(,\|)\)@ /*${VARIABLE}*/\1@g" $FILE
    fi
done < $1
 
