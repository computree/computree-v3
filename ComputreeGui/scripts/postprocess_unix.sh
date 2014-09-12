#!/bin/bash

THISDIR="`pwd`"
DESTDIR="`pwd`/../ComputreeInstall"

cd "$THISDIR"

# Postprocess for unix
mkdir -p $DESTDIR/img
cp img_to_copy/*.png $DESTDIR/img

# Generate translations
lrelease ComputreeGui.pro
mkdir -p $DESTDIR/languages
cp languages/*.qm $DESTDIR/languages