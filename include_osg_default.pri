unix {
    OSG_INC_PATH = "/usr/include"
    OSG_LIB_PATH = "/usr/lib"
}

windows {
    OSG_INC_PATH = "C:/Program Files/OpenSceneGraph/include"
    OSG_LIB_PATH = "C:/Program Files/OpenSceneGraph/lib"
}

OSG_LIB_ADD = osg 
OSG_LIB_ADD += OpenThreads 
OSG_LIB_ADD += osgGA 
OSG_LIB_ADD += osgViewer 
OSG_LIB_ADD += osgWidget 
OSG_LIB_ADD += osgManipulator 
OSG_LIB_ADD += osgSim 
OSG_LIB_ADD += osgText 
OSG_LIB_ADD += osgUtil 
OSG_LIB_ADD += osgDB
OSG_LIB_ADD += osgFX
 
!exists($${OSG_INC_PATH}) { 
    error( "OSG INCLUDE directory not found ! => " $${OSG_INC_PATH})
} 
 
!exists($${OSG_LIB_PATH}) { 
    error( "OSG LIBS directory not found ! => " $${OSG_LIB_PATH})
} 
 
INCLUDEPATH += $$OSG_INC_PATH 
 
LIBS += -L$$OSG_LIB_PATH 
 
for(a, OSG_LIB_ADD) { 
    CONFIG(debug, debug|release) {
            LIBS += -l$${a}d
    } else {
            LIBS += -l$${a}
    }
} 
 
DEFINES += OSG_CONFIG_OK 
