list=`find src -name '*.cpp' -or -name '*.h'`
astyle --style=allman $list
