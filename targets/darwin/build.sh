#!/bin/sh

RELEASE_DIR=../../build/release/darwin/
INTERNAL_DIR=../../build/internal/darwin/

rm -rf ${INTERNAL_DIR}

LIBNOM=libas_common
MODNOM=as_common
ARCH=host

rm -rf build/
make build
[ $? -eq 0 ] || exit $?

mkdir -p ${INTERNAL_DIR}/lib/
mkdir -p ${INTERNAL_DIR}/include/

cp build/include/* ${INTERNAL_DIR}/include/
cp build/lib/*.a ${INTERNAL_DIR}/lib/

mkdir -p ${INTERNAL_DIR}/obj/${ARCH}/${MODNOM}/
cp -r build/obj_${ARCH}/* ${INTERNAL_DIR}/obj/${ARCH}/${MODNOM}/


