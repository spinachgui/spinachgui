#!/bin/sh
mkdir auto
ln -s ../data data
wxformbuilder -g ../spinach.fbp
xsdcxx cxx-tree  --generate-serialization --generate-doxygen --cxx-suffix .cpp --hxx-suffix .hpp --output-dir ./auto --generate-doxygen  ../data/spinxml_schema.xsd
