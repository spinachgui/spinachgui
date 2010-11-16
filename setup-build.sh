#!/bin/sh
BDIR=`pwd`
SDIR=$(dirname $0)

mkdir $BDIR/auto

wxformbuilder -g $SDIR/spinach.fbp
mv $SDIR/auto/* $BDIR/auto/

ln -s $SDIR/res/ $BDIR/res

#xsdcxx cxx-tree  --generate-serialization --generate-doxygen --cxx-suffix .cpp --hxx-suffix .hpp --output-dir ./auto --generate-doxygen  ../data/spinxml_schema.xsd
