sudo apt update -y
sudo apt install build-essential -y
sudo apt install bison -y
sudo apt install flex -y
sudo apt install libgmp3-dev -y
sudo apt install libmpc-dev -y
sudo apt install libmpfr-dev -y
sudo apt install texinfo -y
sudo apt install qemu-system-x86 -y
sudo apt install wget -y

pushd "/tmp/"
wget https://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.xz
tar -xvf binutils-2.39.tar.xz

TARGET=i686-elf
PREFIX=/usr/local/i686-elf
BINUTILS_BUILD="binutils-build-${TARGET}"
mkdir ${BINUTILS_BUILD}

pushd ${BINUTILS_BUILD}
../binutils-2.39/configure \
	--prefix="${PREFIX}" \
	--target=${TARGET}  \
	--with-sysroot	     \
	--disable-nls        \
	--disable-werror
sudo make -j4 
sudo make -j4 install
popd

wget https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.xz
tar -xvf gcc-12.2.0.tar.xz
GCC_BUILD="gcc-build-${TARGET}"
mkdir ${GCC_BUILD}
pushd ${GCC_BUILD}
../gcc-12.2.0/configure	\
	--prefix="${PREFIX}" \
	--target=${TARGET}   \
	--disable-nls	     \
	--enable-languages=c,c++ \
	--without-headers 
sudo make -j4 all-gcc
sudo make -j4 install-gcc
popd

sudo rm -rf ${BINUTILS_BUILD}
sudo rm -rf ${GCC_BUILD}


TARGET=x86_64-elf
PREFIX=/usr/local/x86_64-elf
BINUTILS_BUILD="binutils-build-${TARGET}"
mkdir ${BINUTILS_BUILD}

pushd ${BINUTILS_BUILD}
../binutils-2.39/configure \
	--prefix="${PREFIX}" \
	--target=${TARGET}  \
	--with-sysroot	     \
	--disable-nls        \
	--disable-werror
sudo make -j4 
sudo make -j4 install
popd

GCC_BUILD="gcc-build-${TARGET}"
mkdir ${GCC_BUILD}
pushd ${GCC_BUILD}
../gcc-12.2.0/configure	\
	--prefix="${PREFIX}" \
	--target=${TARGET}   \
	--disable-nls	     \
	--enable-languages=c,c++ \
	--without-headers 
sudo make -j4 all-gcc
sudo make -j4 install-gcc
popd

sudo rm -rf ${BINUTILS_BUILD}
sudo rm -rf ${GCC_BUILD}
