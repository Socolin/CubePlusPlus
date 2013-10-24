hg qpop -a
hg pull -u $1
cd .hg/patches/
hg pull -u $1
cd -
hg qpush -a
./cmakeRelease
make -j4


