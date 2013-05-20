#! /bin/bash
echo -e "Enter name of script \\033[32mLikeThisName\\033[0m: "
read name
echo $name | grep -E '^([A-Z][a-z0-9]+)+$' 2>/dev/null >/dev/null
if test $? -eq 1
then
  echo "\\033[31mBad name type\\033[0m"
  exit 1
fi

echo -e "Choose: \\033[32mBlock \\033[0mor \\033[32mItem\\033[0m \\033[0mor \\033[32mWindow\\033[0m: "
read type
if test "$type" != "Block" -a "$type" != "Item" -a "$type" != "Window"
then
  echo "\\033[31mBad type:\\033[0m $type"
  exit 1
fi
CLASS_NAME=${type}${name}Script
CLASS_NAME_UPPER=`echo $CLASS_NAME | tr '[:lower:]' '[:upper:]'`
SCRIPT_NAME_LOWER=`echo $name | tr '[:upper:]' '[:lower:]'`

FUNCTION_LIST=template/${type}ScriptFunctionList
for ext in cpp h
do
    Filename=../src/$type/Scripts/Basics/${CLASS_NAME}.$ext
    cat template/${type}ScriptTemplate.$ext                            \
    | sed "s/___CLASS_NAME___/${CLASS_NAME}/g"                         \
    | sed "s/___CLASS_NAME_UPPER___/${CLASS_NAME_UPPER}/g"             \
    | sed "s/___SCRIPT_NAME_LOWER___/${SCRIPT_NAME_LOWER}/g"           \
    >$Filename

    if test "$ext" == "h"
    then
        while read func
        do
            echo $func
            cat $Filename | sed -rsu "s@___METHODS___@    ${func}\n___METHODS___@g" > ${Filename}2
            mv ${Filename}2 $Filename
        done < $FUNCTION_LIST
        cat $Filename | sed "s/___METHODS___//g" > ${Filename}2
        mv ${Filename}2 $Filename
    fi
done
