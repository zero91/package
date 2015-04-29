LOCAL_ROOT=`pwd`

boost_fname=boost_1_58_0

rm -rf ${LOCAL_ROOT}/${boost_fname}
if test ! -e ${boost_fname}.tar.gz; then
    curl -# -O http://jaist.dl.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.tar.gz
fi
tar -xzf ${LOCAL_ROOT}/boost_1_58_0.tar.gz

cd ${LOCAL_ROOT}/${boost_fname}
./bootstrap.sh --prefix=${LOCAL_ROOT}
./b2 install

rm -rf ${LOCAL_ROOT}/${boost_fname}
mkdir -p ${LOCAL_ROOT}/lib/dylib
mv ${LOCAL_ROOT}/lib/*.dylib ${LOCAL_ROOT}/lib/dylib

