#!/bin/sh
set -e

# First we are going to make sure that you understand this is sort of experimental and we will be compiling stuff.

# by default CONTINUE will be false
CONTINUE=false

echo ""
echo "You are about to download, compile, and install stuff on your computer."
echo "Please read through the source script to know what is being done."
echo "Do you want to continue? (y/n)"
read -r response
if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]; then
  CONTINUE=true
fi

if ! $CONTINUE; then
  # Bail if response is not yes
  echo "Exiting..."
  exit
fi

# check if `brew` is installed
command -v brew >/dev/null 2>&1 || { echo >&2 "It seems you do not have \`brew\` installed. Head on over to http://brew.sh/ to install it."; exit 1; }

export PREFIX="$HOME/opt/"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p $HOME/src
mkdir -p $PREFIX

# gmp mpfr libmpc
brew install gmp mpfr libmpc autoconf automake xorriso texinfo

# binutils
echo ""
echo "Installing \`binutils\`"
echo ""
cd $HOME/src

if [ ! -d "binutils-2.38" ]; then
  curl http://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.gz > binutils-2.38.tar.gz
  tar xfz binutils-2.38.tar.gz
  
  rm binutils-2.38.tar.gz
fi

mkdir -p build-binutils
cd build-binutils
../binutils-2.38/configure --target=$TARGET --prefix="$PREFIX" --enable-interwork --enable-multilib --disable-nls --disable-werror
make
make install

# gcc
cd $HOME/src

if [ ! -d "gcc-11.2.0" ]; then
  curl http://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz > gcc-11.2.0.tar.gz
  tar xfz gcc-11.2.0.tar.gz

  rm gcc-11.2.0.tar.gz
fi
mkdir -p build-gcc
cd build-gcc
../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --enable-interwork --enable-multilib  --with-gmp=/usr/local/Cellar/gmp/6.2.1_1 --with-mpfr=/usr/local/Cellar/mpfr/4.1.0 --with-mpc=/usr/local/Cellar/libmpc/1.2.1
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# objconv

cd $HOME/src

if [ ! -d "objconv" ]; then
  git clone https://github.com/vertis/objconv.git
  cd objconv
  
  g++ -o objconv -O2 src/*.cpp --prefix="$PREFIX"
  cp objconv $PREFIX/bin
fi