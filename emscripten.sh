#!/bin/bash
START=$(date)
echo -n "** Retrieving Sources Files **"
SOURCES=$(find . -name "*.cpp")
echo " "${SOURCES} | sed 's/ /\n  /g'

echo "** Settings Options **"
OPTIONS='-I./include -std=c++11'
# --emit-symbol-map --profiling -g'
echo "	${OPTIONS}"

echo "** Compiling **"
mkdir -p bin/html > /dev/null
em++ ${OPTIONS} ${SOURCES} -o bin/html/Muon.html &> bin/html/compile.log

echo "** Done **"
echo " Started at	: ${START}"
echo " Ended at	: $(date)"