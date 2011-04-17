#!/bin/sh
BDIR=`pwd`
SDIR=$(dirname $0)

mkdir $BDIR/auto

wxformbuilder -g $SDIR/spinach.fbp
mv $SDIR/auto/SpinachGUI.h $BDIR/auto/SpinachGUI.hpp
mv $SDIR/auto/SpinachGUI.cpp $BDIR/auto/

ln -s $SDIR/res/ $BDIR/res
