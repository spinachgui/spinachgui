#!/bin/sh
BDIR=`pwd`
SDIR=$(dirname $0)

cp -r $SDIR/examples/ $BDIR/examples/

mkdir $BDIR/auto

wxformbuilder -g $SDIR/spinach.fbp
wxformbuilder -g $SDIR/easyspin.fbp
mv $SDIR/SpinachGUI.h $BDIR/auto/
mv $SDIR/SpinachGUI.cpp $BDIR/auto/

mv $SDIR/easyspin.h $BDIR/auto/
mv $SDIR/easyspin.cpp $BDIR/auto/


ln -s $SDIR/res/ $BDIR/res
