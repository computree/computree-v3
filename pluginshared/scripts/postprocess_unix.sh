#!/bin/bash

THISDIR="`pwd`"
DESTDIR="`pwd`/../ComputreeInstall"

cd "$THISDIR"

# Generate translations
lrelease pluginshared.pro
mkdir -p $DESTDIR/languages
cp languages/*.qm $DESTDIR/languages