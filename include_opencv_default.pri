unix {
	contains(DEFINES, USE_OPENCV3) {
		OPENCV_INC_PATH = "/usr/local/include" 
		OPENCV_LIBS_PATH = "/usr/local/lib"
	} 
}
 
!isEmpty(OPENCV_INC_PATH) {   
	!exists($${OPENCV_INC_PATH}) { 
		error( "OPENCV INCLUDE directory not found ! => " $${OPENCV_INC_PATH}) 
	} 
 
	!exists($${OPENCV_LIBS_PATH}) { 
		error( "OPENCV LIBS directory not found ! => " $${OPENCV_LIBS_PATH}) 
	} 
 
	OPENCV_LIB_ADD =  
	OPENCV_LIB_ADD += opencv_ts
	OPENCV_LIB_ADD += opencv_world
	
	DEFINES += USE_OPENCV 
 
	INCLUDEPATH += $$OPENCV_INC_PATH 
	LIBS += -L$$OPENCV_LIBS_PATH 
 
	for(a, OPENCV_LIB_ADD) { 
		CONFIG(debug, debug|release) { 
			LIBS += -l$${a}d 
		} else { 
			LIBS += -l$${a} 
		} 
	} 
 
	message(You want to use OPENCV : ok) 
} else { 
	message(OPENCV will not be used ) 
} 
