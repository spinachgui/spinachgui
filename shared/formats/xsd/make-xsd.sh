#!/bin/sh
BDIR=`pwd`
SDIR=$(dirname $0)

xsdcxx cxx-tree  --generate-serialization --generate-doxygen --cxx-suffix .cpp --hxx-suffix .hpp --generate-doxygen  $SDIR/../../../data/spinxml_schema.xsd

