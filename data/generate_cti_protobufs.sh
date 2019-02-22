#!/bin/sh

GENDIR=../src/cti_GENERATED/

rm -rf ${GENDIR}/protobuf
mkdir -p ${GENDIR}/protobuf
protoc --plugin=protoc-gen-nanopb=../ext/nanopb/generator/protoc-gen-nanopb cti_protobuf/addsec_cti.proto --nanopb_out=${GENDIR}/
[ $? -eq 0 ] || exit $?;
cp ../ext/nanopb/pb.h ${GENDIR}/protobuf/

rm -rf ${GENDIR}/python
mkdir -p ${GENDIR}/python/
protoc --python_out=${GENDIR}/python/ cti_protobuf/addsec_cti.proto
[ $? -eq 0 ] || exit $?;

rm -rf ${GENDIR}/java
mkdir -p ${GENDIR}/java/
protoc --java_out=${GENDIR}/java/ cti_protobuf/addsec_cti.proto
[ $? -eq 0 ] || exit $?;

echo "Success"
