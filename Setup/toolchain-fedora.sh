sudo dnf update -y
sudo dnf install gcc -y
sudo dnf install gcc-c++ -y
sudo dnf install make -y
sudo dnf install bison -y
sudo dnf install flex -y
sudo dnf install gmp-devel -y
sudo dnf install libmpc-devel -y
sudo dnf install mpfr-devel -y
sudo dnf install texinfo -y
sudo dnf install qemu-system-x86 -y
sudo dnf install wget -y

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
make -j4 
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
make -j4 
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
