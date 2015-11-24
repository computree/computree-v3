@ECHO OFF

setlocal EnableDelayedExpansion

echo;
echo ---------------------------------------------------------
echo ----- SCRIPT THAT GENERATE INCLUDE_XXX FILES BEGIN  -----
echo ---------------------------------------------------------

set SEARCHDIR=%~dp0..\..
set DESTDIR=%~dp0..
set OSGDESTFILE=%DESTDIR%\include_osg.pri
set GDALDESTFILE=%DESTDIR%\include_gdal.pri
set OPENCVDESTFILE=%DESTDIR%\include_opencv.pri
set PCLDESTFILE=%DESTDIR%\include_pcl.pri

IF EXIST %SEARCHDIR%\use_osg.ini (
	set /a index=0
	FOR /F "delims=" %%i IN (%SEARCHDIR%\use_osg.ini) DO (
		set OSG_VALUES[!index!]=%%i
		set /a index+=1
	)
) else (
	set OSG_VALUES[0]="C:/Program Files/OpenSceneGraph/include"
	set OSG_VALUES[1]="C:/Program Files/OpenSceneGraph/lib"
	echo;
	echo The file use_osg.ini was not found in %SEARCHDIR% so OSG will be used with default path
)

IF EXIST %SEARCHDIR%\use_gdal.ini (
	set /a index=0
	FOR /F "delims=" %%i IN (%SEARCHDIR%\use_gdal.ini) DO (
		set GDAL_VALUES[!index!]=%%i
		set /a index+=1
	)
	set GDAL_INI_FOUND=1
) else (
	echo;
	echo The file use_gdal.ini was not found in %SEARCHDIR% so GDAL will not be used in your program
	set GDAL_VALUES[0]=""
)

IF EXIST %SEARCHDIR%\use_opencv.ini (
	echo;
	set /a index=0
	FOR /F "delims=" %%i IN (%SEARCHDIR%\use_opencv.ini) DO (
		set OPENCV_VALUES[!index!]=%%i
		set /a index+=1
	)
	set OPENCV_INI_FOUND=1
) else (
	echo;
	echo The file use_opencv.ini was not found in %SEARCHDIR% so OPENCV will not be used in your program
	set OPENCV_VALUES[0]=""
	set OPENCV_VALUES[1]=""
)

IF EXIST %SEARCHDIR%\use_pcl.ini (
	echo;
	set /a index=0
	FOR /F "delims=" %%i IN (%SEARCHDIR%\use_pcl.ini) DO (
		set PCL_VALUES[!index!]=%%i
		set /a index+=1
	)
	set PCL_INI_FOUND=1
) else (
	echo;
	echo The file use_pcl.ini was not found in %SEARCHDIR% so PCL will not be used in your program
	set PCL_VALUES[0]=""
	set PCL_VALUES[1]=""
	set PCL_VALUES[2]=""
	set PCL_VALUES[3]=""
	set PCL_VALUES[4]=""
	set PCL_VALUES[5]=""
	set PCL_VALUES[6]=""
	set PCL_VALUES[7]=""
	set PCL_VALUES[8]=""
)

goto :generate_osg

goto :generate_gdal

goto :generate_opencv

goto :generate_pcl

goto :eof

:generate_osg
set OSG_VALUES[0]=%OSG_VALUES[0]:"=%
set OSG_VALUES[1]=%OSG_VALUES[1]:"=%

echo;
echo -------- include_osg.pri will now be generated... --------

echo;
echo Set OSG include path to %OSG_VALUES[0]%
echo Set OSG libs path to %OSG_VALUES[1]%

echo OSG_INC_PATH = "%OSG_VALUES[0]%" > %OSGDESTFILE%
echo OSG_LIB_PATH = "%OSG_VALUES[1]%" >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo OSG_LIB_ADD = osg >> %OSGDESTFILE%
echo OSG_LIB_ADD += OpenThreads >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgGA >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgViewer >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgWidget >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgManipulator >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgSim >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgText >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgUtil >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgDB >> %OSGDESTFILE%
echo OSG_LIB_ADD += osgFX >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo ^^!exists($${OSG_INC_PATH}) { >> %OSGDESTFILE%
echo 	error( "OSG INCLUDE directory not found ^! => " $${OSG_INC_PATH}) >> %OSGDESTFILE%
echo } >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo ^^!exists($${OSG_LIB_PATH}) { >> %OSGDESTFILE%
echo 	error( "OSG LIBS directory not found ^! => " $${OSG_LIB_PATH}) >> %OSGDESTFILE%
echo } >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo INCLUDEPATH += $$OSG_INC_PATH >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo LIBS += -L$$OSG_LIB_PATH >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo for(a, OSG_LIB_ADD) { >> %OSGDESTFILE%
echo 	CONFIG(debug, debug^|release^) { >> %OSGDESTFILE%
echo 		LIBS += -l$${a}d >> %OSGDESTFILE%
echo 	} else { >> %OSGDESTFILE%
echo 		LIBS += -l$${a} >> %OSGDESTFILE%
echo 	} >> %OSGDESTFILE%
echo } >> %OSGDESTFILE%

echo; >> %OSGDESTFILE%
echo DEFINES += OSG_CONFIG_OK >> %OSGDESTFILE%



:generate_gdal
set GDAL_VALUES[0]=%GDAL_VALUES[0]:"=%

echo;
echo -------- include_gdal.pri will now be generated... --------

echo;
echo Set GDAL include path to %GDAL_VALUES[0]%/include
echo Set GDAL libs path to %GDAL_VALUES[0]%/lib

echo GDAL_INI_FOUND = %GDAL_INI_FOUND% > %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo ^^!isEmpty(GDAL_INI_FOUND^) { >> %GDALDESTFILE%

set SEARCHIN=%GDAL_VALUES[0]:/=\%
set SEARCHIN=%SEARCHIN%\lib\*.lib

echo; >> %GDALDESTFILE%
echo 	GDAL_LIB_ADD = >> %GDALDESTFILE%

FOR %%i IN ("%SEARCHIN%") DO (
	set z=%%~ni
	echo 	GDAL_LIB_ADD += !z! >> %GDALDESTFILE%
)

echo; >> %GDALDESTFILE%
echo 	GDAL_INC_PATH = "%GDAL_VALUES[0]%/include" >> %GDALDESTFILE%
echo 	GDAL_LIBS_PATH = "%GDAL_VALUES[0]%/lib" >> %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo 	^^!exists($${GDAL_INC_PATH}) { >> %GDALDESTFILE%
echo 		error( "GDAL INCLUDE directory not found ^! => " $${GDAL_INC_PATH}) >> %GDALDESTFILE%
echo 	} >> %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo 	^^!exists($${GDAL_LIBS_PATH}) { >> %GDALDESTFILE%
echo 		error( "GDAL LIBS directory not found ^! => " $${GDAL_LIBS_PATH}) >> %GDALDESTFILE%
echo 	} >> %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo    DEFINES += USE_GDAL >> %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo    INCLUDEPATH += $$GDAL_INC_PATH >> %GDALDESTFILE%
echo    LIBS += -L$$GDAL_LIBS_PATH >> %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo for(a, GDAL_LIB_ADD) { >> %GDALDESTFILE%
echo 		LIBS += -l$${a} >> %GDALDESTFILE%
echo } >> %GDALDESTFILE%

echo; >> %GDALDESTFILE%
echo 	message(You want to use GDAL : ok^) >> %GDALDESTFILE%
echo } else { >> %GDALDESTFILE%
echo 	message(GDAL will not be used ^) >> %GDALDESTFILE%
echo } >> %GDALDESTFILE%

:generate_opencv
set OPENCV_VALUES[0]=%OPENCV_VALUES[0]:"=%
set OPENCV_VALUES[1]=%OPENCV_VALUES[1]:"=%

echo;
echo -------- include_opencv.pri will now be generated... --------

echo;
echo Set OPENCV include path to %OPENCV_VALUES[0]%
echo Set OPENCV libs path to %OPENCV_VALUES[1]%

echo OPENCV_INI_FOUND = %OPENCV_INI_FOUND% > %OPENCVDESTFILE%

echo; >> %OPENCVDESTFILE%
echo ^^!isEmpty(OPENCV_INI_FOUND^) { >> %OPENCVDESTFILE%

set SEARCHIN=%OPENCV_VALUES[1]:/=\%
set SEARCHIN=%SEARCHIN%\*300.lib

echo; >> %OPENCVDESTFILE%
echo 	OPENCV_LIB_ADD =  >> %OPENCVDESTFILE%

FOR %%i IN ("%SEARCHIN%") DO (
	set z=%%~ni
	echo 	OPENCV_LIB_ADD += !z! >> %OPENCVDESTFILE%
)

echo; >> %OPENCVDESTFILE%
echo 	OPENCV_INC_PATH = "%OPENCV_VALUES[0]%" >> %OPENCVDESTFILE%
echo 	OPENCV_LIBS_PATH = "%OPENCV_VALUES[1]%" >> %OPENCVDESTFILE%

echo; >> %OPENCVDESTFILE%
echo 	^^!exists($${OPENCV_INC_PATH}) { >> %OPENCVDESTFILE%
echo 		error( "OPENCV INCLUDE directory not found ^! => " $${OPENCV_INC_PATH}) >> %OPENCVDESTFILE%
echo 	} >> %OPENCVDESTFILE%

echo; >> %OPENCVDESTFILE%
echo 	^^!exists($${OPENCV_LIBS_PATH}) { >> %OPENCVDESTFILE%
echo 		error( "OPENCV LIBS directory not found ^! => " $${OPENCV_LIBS_PATH}) >> %OPENCVDESTFILE%
echo 	} >> %OPENCVDESTFILE%

echo; >> %OPENCVDESTFILE%
echo    DEFINES += USE_OPENCV >> %OPENCVDESTFILE%

echo; >> %OPENCVDESTFILE%
echo    INCLUDEPATH += $$OPENCV_INC_PATH >> %OPENCVDESTFILE%
echo    LIBS += -L$$OPENCV_LIBS_PATH >> %OPENCVDESTFILE%

echo; >> %OPENCVDESTFILE%
echo 	for(a, OPENCV_LIB_ADD) { >> %OPENCVDESTFILE%
echo 		CONFIG(debug, debug^|release^) { >> %OPENCVDESTFILE%
echo 			LIBS += -l$${a}d >> %OPENCVDESTFILE%
echo 		} else { >> %OPENCVDESTFILE%
echo 			LIBS += -l$${a} >> %OPENCVDESTFILE%
echo 		} >> %OPENCVDESTFILE%
echo 	} >> %OPENCVDESTFILE%
	
echo; >> %OPENCVDESTFILE%
echo 	message(You want to use OPENCV : ok^) >> %OPENCVDESTFILE%
echo } else { >> %OPENCVDESTFILE%
echo 	message(OPENCV will not be used ^) >> %OPENCVDESTFILE%
echo } >> %OPENCVDESTFILE%

:generate_pcl

set PCL_VALUES[0]=%PCL_VALUES[0]:"=%
set PCL_VALUES[1]=%PCL_VALUES[1]:"=%
set PCL_VALUES[2]=%PCL_VALUES[2]:"=%
set PCL_VALUES[3]=%PCL_VALUES[3]:"=%
set PCL_VALUES[4]=%PCL_VALUES[4]:"=%
set PCL_VALUES[5]=%PCL_VALUES[5]:"=%
set PCL_VALUES[6]=%PCL_VALUES[6]:"=%
set PCL_VALUES[7]=%PCL_VALUES[7]:"=%

echo;
echo -------- include_pcl.pri will now be generated... --------

echo;
echo Set PCL version : %PCL_VALUES[0]%
echo Set PCL libs path to %PCL_VALUES[1]%
echo Set PCL include path to %PCL_VALUES[2]%
echo Set EIGEN include path to %PCL_VALUES[3]%
echo Set QHULL include path to %PCL_VALUES[4]%
echo Set FLANN include path to %PCL_VALUES[5]%
echo Set BOOST libs path to %PCL_VALUES[6]%
echo Set BOOST include path to %PCL_VALUES[7]%

echo PCL_INI_FOUND = %PCL_INI_FOUND% > %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo ^^!isEmpty(PCL_INI_FOUND^) { >> %PCLDESTFILE%

echo 	PCL_LIB_VERSION = "%PCL_VALUES[0]%" >> %PCLDESTFILE%
echo 	PCL_LIBS_PATH = "%PCL_VALUES[1]%" >> %PCLDESTFILE%
echo 	PCL_INC_PATH = "%PCL_VALUES[2]%" >> %PCLDESTFILE%
echo 	EIGEN_INC_PATH = "%PCL_VALUES[3]%" >> %PCLDESTFILE%
echo 	QHULL_INC_PATH = "%PCL_VALUES[4]%" >> %PCLDESTFILE%
echo 	FLANN_INC_PATH = "%PCL_VALUES[5]%" >> %PCLDESTFILE%
echo 	BOOST_LIBS_PATH = "%PCL_VALUES[6]%" >> %PCLDESTFILE%
echo 	BOOST_INC_PATH = "%PCL_VALUES[7]%" >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${PCL_INC_PATH}) { >> %PCLDESTFILE%
echo 		error( "PCL INCLUDE directory not found ^! => " $${PCL_INC_PATH}) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${PCL_LIBS_PATH}) { >> %PCLDESTFILE%
echo 		error( "PCL LIBS directory not found ^! => " $${PCL_LIBS_PATH}) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${EIGEN_INC_PATH}) { >> %PCLDESTFILE%
echo 		error( "EIGEN INCLUDE directory not found ^! =>" $${EIGEN_INC_PATH} ) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${QHULL_INC_PATH}) { >> %PCLDESTFILE%
echo 		error( "QHULL INCLUDE directory not found ^! =>" $${QHULL_INC_PATH} ) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${FLANN_INC_PATH}) { >> %PCLDESTFILE%
echo 		error( "FLANN INCLUDE directory not found ^! =>" $${FLANN_INC_PATH} ) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${BOOST_INC_PATH}) { >> %PCLDESTFILE%
echo 		error( "BOOST INCLUDE directory not found ^! =>" $${BOOST_INC_PATH} ) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	^^!exists($${BOOST_LIBS_PATH}) { >> %PCLDESTFILE%
echo 		error( "BOOST LIBS directory not found ^! =>" $${BOOST_LIBS_PATH} ) >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	DEFINES += USE_PCL >> %PCLDESTFILE%
echo 	DEFINES += USE_BOOST >> %PCLDESTFILE%

set SEARCHIN=%PCL_VALUES[1]:/=\%
set SEARCHIN=%SEARCHIN%\*release.lib

echo; >> %PCLDESTFILE%
echo 	PCL_LIB_ADD =  >> %PCLDESTFILE%

FOR %%i IN ("%SEARCHIN%") DO (
	set z=%%~ni
	set z=!z:_release=!
	echo 	PCL_LIB_ADD += !z! >> %PCLDESTFILE%
)

echo; >> %PCLDESTFILE%
echo 	INCLUDEPATH += $$PCL_INC_PATH >> %PCLDESTFILE%
echo 	INCLUDEPATH += $$EIGEN_INC_PATH >> %PCLDESTFILE%
echo 	INCLUDEPATH += $$QHULL_INC_PATH >> %PCLDESTFILE%
echo 	INCLUDEPATH += $$FLANN_INC_PATH >> %PCLDESTFILE%
echo 	INCLUDEPATH += $$BOOST_INC_PATH >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	LIBS += -L$$PCL_LIBS_PATH >> %PCLDESTFILE%
echo 	LIBS += -L$$BOOST_LIBS_PATH >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	for(a, PCL_LIB_ADD) { >> %PCLDESTFILE%
echo 		CONFIG(debug, debug^|release^) { >> %PCLDESTFILE%
echo 			LIBS += -l$${a}_debug >> %PCLDESTFILE%
echo 		} else { >> %PCLDESTFILE%
echo 			LIBS += -l$${a}_release >> %PCLDESTFILE%
echo 		} >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	greaterThan(QT_MAJOR_VERSION, 4^) { >> %PCLDESTFILE%
echo 		load(moc^) >> %PCLDESTFILE%
echo 		QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE >> %PCLDESTFILE%
echo 	} >> %PCLDESTFILE%

echo; >> %PCLDESTFILE%
echo 	message(You want to use PCL : ok^) >> %PCLDESTFILE%
echo } else { >> %PCLDESTFILE%
echo 	message(PCL will not be used ^) >> %PCLDESTFILE%
echo } >> %PCLDESTFILE%