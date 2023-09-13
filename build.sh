#!/bin/bash

# Config
EXEC_PATH=./bin/the-demo
POSTFIX=_d
BUILD_PATH=./build
C_COMP=clang
CXX_COMP=clang++

# Defaults
BUILD_TYPE=Debug
GENERATE=1
BUILD=1
EXECUTE=0
CLEAN=0

while getopts ":bdrcxg" ARGOPT; do
	case $ARGOPT in
		b)
			BUILD=1
			GENERATE=0
			EXECUTE=0
			;;
		g)
			GENERATE=1
			BUILD=0
			EXECUTE=0
			;;
		x)
			GENERATE=0
			BUILD=0
			EXECUTE=1
			;;
		d)
			BUILD_TYPE=Debug
			POSTFIX=_d
			;;
		r)
			BUILD_TYPE=Release
			POSTFIX=
			;;
		c)
			CLEAN=1
			;;
	esac
done

if [[ ${CLEAN} -eq 1 ]]; then
	rm -drf ${BUILD_PATH}
fi

mkdir -p ${BUILD_PATH}/${BUILD_TYPE}
cd ${BUILD_PATH}/${BUILD_TYPE}

if [[ GENERATE -eq 1 ]]; then
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_C_COMPILER=${C_COMP} -DCMAKE_CXX_COMPILER=${CXX_COMP} ../..
fi

if [[ BUILD -eq 1 ]]; then
	cmake --build .
fi

if [[ EXECUTE -eq 1 ]]; then
	cd ../..
	eval ${EXEC_PATH}${POSTFIX}
fi

