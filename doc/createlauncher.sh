#!/bin/sh
 appname=`basename $0 | sed s,\.sh$,,`

 dirname=`dirname $0`
 tmp="${dirname#?}"

 if [ "${dirname%$tmp}" != "/" ]; then
 dirname=$PWD/$dirname
 fi

rm computree.desktop
echo "[Desktop Entry]" >> ./computree.desktop
echo "Version=1.0" >> ./computree.desktop
echo "Type=Application" >> ./computree.desktop
echo "Terminal=true" >> ./computree.desktop
echo "Categories=GNOME;GTK;" >> ./computree.desktop
echo "Exec=sh ./CompuTreeGui.sh" >> ./computree.desktop
echo "Path="$dirname >> ./computree.desktop
echo "Name=Computree" >> ./computree.desktop
echo "Comment=Logiciel Computree" >> ./computree.desktop
echo "Icon="$dirname"/Icone_48x48.png" >> ./computree.desktop
