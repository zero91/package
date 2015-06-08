ROOT=`pwd`
version=2.1.2

mkdir ${ROOT}/build
cd ${ROOT}/build
cmake ${ROOT}/${version}
make 

function create_dir() {
    if [ $# -lt 1 ]; then
        echo "Wrong Parameter"
        return
    fi

    while [ -n "$1" ]; do
        path=$1

        if [ -e $path ]; then
            rm -rf $path
        fi
        mkdir -p $path
        shift 1
    done
}

create_dir ${ROOT}/include ${ROOT}/lib

cp -r ${ROOT}/build/lib/* ${ROOT}/lib
cp -r ${ROOT}/build/include/* ${ROOT}/include

rm -rf ${ROOT}/build
