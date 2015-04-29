LOCAL_ROOT=`pwd`
version=2.6.1

build_path=${LOCAL_ROOT}/build-${version}
gtest_path=${LOCAL_ROOT}/../gtest/1.7.0

cp -r ${version} ${build_path}
cp -r ${gtest_path} ${build_path}/gtest
cd ${build_path}

./autogen.sh
./configure --prefix=${LOCAL_ROOT}
make
make check
make install

rm -rf ${build_path}
