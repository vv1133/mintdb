#! /bin/sh
rm configure.in
autoscan
cp configure.in.save configure.in
aclocal
autoconf
autoheader
automake --add-missing
./configure CXXFLAGS= CFLAGS=
rm mintdb
make
