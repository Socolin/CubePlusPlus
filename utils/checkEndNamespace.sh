#! /bin/sh

for file in `find src/ -type f`
do
    result=`awk '/namespace/ { if ($1 == "}" && last != $4) {print $4" "last};  last=$2; }' $file`
    if test -n "$result"
    then
        echo $file;
        echo $result;
    fi
done

