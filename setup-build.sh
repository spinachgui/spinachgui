#!/bin/sh
BDIR=`pwd`
SDIR=$(dirname $0)

mkdir $BDIR/auto

wxformbuilder -g $SDIR/spinach.fbp
mv $SDIR/SpinachGUI.h $BDIR/auto/SpinachGUI.h
mv $SDIR/SpinachGUI.cpp $BDIR/auto/

ln -s $SDIR/res/ $BDIR/res
