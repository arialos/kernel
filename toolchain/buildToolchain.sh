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
brew install gmp mpfr libmpc autoconf automake

# binutils
echo ""
echo "Installing \`binutils\`"
echo ""
cd $HOME/src

if [ ! -d "binutils-2.25" ]; then
  curl http://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz > binutils-2.25.tar.gz
  tar xfz binutils-2.25.tar.gz

  rm binutils-2.25.tar.gz
  mkdir -p build-binutils
  cd build-binutils
  ../binutils-2.25/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
  make
  make install
fi

# gcc
cd $HOME/src

if [ ! -d "gcc-5.3.0" ]; then
  curl http://www.netgull.com/gcc/releases/gcc-5.3.0/gcc-5.3.0.tar.gz > gcc-5.3.0.tar.gz
  tar xfz gcc-5.3.0.tar.gz

  rm gcc-5.3.0.tar.gz
  mkdir -p build-gcc
  cd build-gcc
  ../gcc-5.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --with-gmp=/usr/local/Cellar/gmp/6.1.0 --with-mpfr=/usr/local/Cellar/mpfr/3.1.3 --with-mpc=/usr/local/Cellar/libmpc/1.0.3
  make all-gcc
  make all-target-libgcc
  make install-gcc
  make install-target-libgcc
fi

# objconv

cd $HOME/src

if [ ! -d "objconv" ]; then
  git clone https://github.com/vertis/objconv.git
  cd objconv
  
  g++ -o objconv -O2 src/*.cpp --prefix="$PREFIX"
  cp objconv $PREFIX/bin
fi

# grub

cd $HOME/src

if [ ! -d "grub" ]; then
  git clone --depth 1 git://git.savannah.gnu.org/grub.git

  cd grub
  ./bootstrap
  sh autogen.sh
  mkdir -p build-grub
  cd build-grub
  ../configure --disable-werror TARGET_CC=$TARGET-gcc TARGET_OBJCOPY=$TARGET-objcopy \
    TARGET_STRIP=$TARGET-strip TARGET_NM=$TARGET-nm TARGET_RANLIB=$TARGET-ranlib --target=$TARGET --prefix=$PREFIX
  make
  make install
fi