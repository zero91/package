ROOT=`pwd`
version=1.7.0

mkdir ${ROOT}/build
cd ${ROOT}/build
cmake ${ROOT}/${version}
make 

if [ ! -d ${ROOT}/include ]; then
    mkdir -p ${ROOT}/include
fi

if [ ! -d ${ROOT}/lib ]; then
    mkdir -p ${ROOT}/lib
fi

cp ${ROOT}/build/libgtest.a ${ROOT}/lib
cp ${ROOT}/build/libgtest_main.a ${ROOT}/lib
cp -r ${ROOT}/${version}/include/* ${ROOT}/include

rm -rf ${ROOT}/build
