To compile GDAL on windows (mingw) :
------------------------------------

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

To compile GDAL on windows (visual studio) :
--------------------------------------------

Download the latest GDAL source code from http://download.osgeo.org/gdal/. 
Extract the files into a folder e.g. C:\Work\src\gdal\.
On the Windows Desktop, select Start | All Programs | Microsoft Visual Studio 2010 | Visual Studio Tools | Visual Studio x64 Win64 Command Prompt.

The Visual Studio x64 Win64 Command Prompt appears.
In the Command Prompt, change directory to the extracted GDAL source code root folder.

C:\> cd \Work\src\gdal-1.9.2
Build the GDAL 64 bit Windows binaries with the nmake command.

C:\> nmake /f makefile.vc MSVC_VER=1600 WIN64=YES