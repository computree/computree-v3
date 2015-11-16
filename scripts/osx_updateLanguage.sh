#!/bin/bash

function launchF {
  bName=$(basename $1)

  for f in $1/*.pro; do
     if [ -f "$f" ]; then
       	if grep -iq "TRANSLATIONS" "$f"; then

	   filename=$(basename "$f")
           filename="${filename%.*}"

       	   echo;
	   echo Generate and copy translations of $filename
	   echo;

	   if [ -d "$QTDIR" ]; then
	      $QTDIR/lupdate $f>nul
	      $QTDIR/lrelease $f>nul
	   else
	      lupdate $f>nul
	      lrelease $f>nul
	   fi

	   mkdir -p "$ALLDESTDIR/languages"
	
	   nFiles=0

	   for j in $(find $1 -name '*.qm'); do
	      if [ -f "$j" ]; then
	        echo $(basename "$j")
		cp $j $ALLDESTDIR/languages
		let nFiles=nFiles+1
	      fi
	   done

	   echo;
	   echo "$nFiles files copied"
     	fi
     fi
  done

  if [ -d "$1/shaders" ]; then
    echo;
    echo Copy shaders of $bName
    echo;

    mkdir -p "$ALLDESTDIR/shaders"

    let nFiles=0

    for j in $1/shaders/*; do
       if [ -f "$j" ]; then
         echo $(basename "$j")
	 cp $j $ALLDESTDIR/shaders
	 let nFiles=nFiles+1
       fi
    done

    echo;
    echo "$nFiles files copied"
  fi
}

echo;
echo --------------------------------------------
echo ----- SCRIPT BEGIN FOR DEBUG VERSION -------
echo --------------------------------------------

QTDIR=$1
ALLDESTDIR="`pwd`/../../ComputreeInstallDebug"

for X in `pwd`/../*; do
  if [ -d "$X" ]; then
    launchF $X
  fi
done

for X in `pwd`/../../plugin*; do
  for Y in $X/*; do
     if [ -d "$Y" ]; then
        launchF $Y
     fi
  done
done

echo;
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo "If you have problem with lupdate or lrelease (could not exec...) please call this script with the path of the bin directory of QT in first argument."
echo;
echo "EXAMPLE : $0 /home/login/QTX/X.X/bin"
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo;

echo;
echo ----------------------------------------------
echo ----- SCRIPT BEGIN FOR RELEASE VERSION -------
echo ----------------------------------------------

QTDIR=$1
ALLDESTDIR="`pwd`/../../ComputreeInstallRelease"

for X in `pwd`/../*; do
  if [ -d "$X" ]; then
    launchF $X
  fi
done

for X in `pwd`/../../plugin*; do
  for Y in $X/*; do
     if [ -d "$Y" ]; then
        launchF $Y
     fi
  done
done

echo;
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo "If you have problem with lupdate or lrelease (could not exec...) please call this script with the path of the bin directory of QT in first argument."
echo;
echo "EXAMPLE : $0 /home/login/QTX/X.X/bin"
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo;
