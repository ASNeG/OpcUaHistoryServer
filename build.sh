#!/bin/bash

usage()
{
   echo "build.sh [local, package, test]"
}


build_local()
{
    echo "build local start"

    # build local directory
    rm -rf build_local
    mkdir build_local
    cd build_local

    # install build local
    cmake ../src
    if [ $? -ne 0 ] ;
    then
        echo "cmake error"
        return $?
    fi
    make install
     if [ $? -ne 0 ] ;
    then
        echo "make install error"
        return $?
    fi

    return 0
}


build_package()
{
    echo "build package start"

    # check if debian package to be produced
    if which dpkg > /dev/null ;
    then
        echo "build deb package enable"
        DEB=1
    else
    	echo "build deb package disable"
        DEB=0    
    fi

    # check if rpm package to be produced
    if which rpmbuild > /dev/null ;
    then
        echo "build rpm package enable"
        RPM=1
    else
        echo "build rpm package disable"
        RPM=0
    fi
    
    # build package directory
    rm -rf build_package
    mkdir build_package
    cd build_package

    # build package
    cmake ../src
    if [ $? -ne 0 ] ;
    then
        echo "cmake error"
        return $?
    fi
    make package
    if [ $? -ne 0 ] ;
    then
        echo "make package error"
        return $?
    fi

    return 0
}

build_test()
{    
    echo "build test start"

    # build test directory
    rm -rf build_test
    mkdir build_test
    cd build_test

    # build test
    cmake ../tst
    if [ $? -ne 0 ] ;
    then
        echo "cmake error"
        return $?
    fi

     return 0
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# main
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
if [ $# -ne 1 ] ; 
then
    usage
    exit
fi

if [ "$1" = "local" ] ;
then
    build_local
    exit $?
elif [ "$1" = "package" ] ;
then 
    build_package
    exit $?
elif [ "$1" = "test" ] ;
then
    build_test
    exit $?
else
    usage
fi
