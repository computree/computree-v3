To compile GDAL on windows :

Install MinGW64 => use the same version that used by your version of QT (go to mingw/bin directory of qt in 
a msdos window and type "gcc -v" to know the version that you must install)

Install MSys => when a msdos prompt is showed, type "yes" and follow instruction. Type the path of the root directory of your recent mingw installation

Download GDal source.
Unzip them in a folder.
Launch a MSys command window.
Go to the folder of GDal.
Type ./configure

Follow instruction :
--------------------

The complete steps for building gdal-1.10.0 under MinGW64 (mingw-builds) are:

$./configure
Then, Edit GDALmake.opt, Find GDAL_ROOT and replace the cygwin drive format with dos/mingw format, e.g. Change:

GDAL_ROOT  = /d/temp/build/gdal-1.10.0
to

GDAL_ROOT =  d:/temp/build/gdal-1.10.0
Replace

CONFIG_LIBS =   $(GDAL_ROOT)/$(LIBGDAL)  
with

CONFIG_LIBS =   $(GDAL_ROOT)/$(LIBGDAL)   -liconv

-----------------------


And finally type "make && make install"
