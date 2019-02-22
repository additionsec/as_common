#!/bin/sh

GENDIR=../src/cti_GENERATED/

mkdir -p ${GENDIR}/
perl ../tools/cti/tests_to_defines.pl cti_defines/tests.txt cti_defines/subtests.txt ${GENDIR}/ascti_tests.h ${GENDIR}/definitions.json
[ $? -eq 0 ] || exit $?;

echo "Success"
