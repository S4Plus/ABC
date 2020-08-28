#!/bin/bash

gcc_version=5.1.0
wget https://ftp.gnu.org/gnu/gcc/gcc-${gcc_version}/gcc-${gcc_version}.tar.bz2
wget https://ftp.gnu.org/gnu/gcc/gcc-${gcc_version}/gcc-${gcc_version}.tar.bz2.sig
wget https://ftp.gnu.org/gnu/gnu-keyring.gpg
signature_invalid=`gpg --verify --no-default-keyring --keyring ./gnu-keyring.gpg gcc-${gcc_version}.tar.bz2.sig`
if [ $signature_invalid ]; then echo "Invalid signature" ; exit 1 ; fi
tar -xvjf gcc-${gcc_version}.tar.bz2
cd gcc-${gcc_version}
./contrib/download_prerequisites
cd ..
mkdir gcc-${gcc_version}-build
cd gcc-${gcc_version}-build
$PWD/../gcc-${gcc_version}/configure --prefix=$HOME/toolchains --enable-languages=c,c++
make -j$(nproc)
make install