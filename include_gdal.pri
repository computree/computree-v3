# To use GDAL =>
# Do the declaration in file use_gdal.ini in this order with quotes for all lines :

# example for ubuntu 13.10 or osx (remove # symbols) :
#"/usr/local/bin/gdal"

# example for windows (remove # symbols) WARNING : slash must be like this / :
#"C:/gdal"

# the variable USE_GDAL is defined if all elements of gdal was found

exists(../use_gdal.ini) {
    message( ------- use_gdal.ini detected ------ )
    TMP_CAT =

    unix | mac {
        TMP_CAT = $$system( cat ../use_gdal.ini )
    }

    win32 {
        TMP_CAT = $$system( type ..\use_gdal.ini )
    }

    #GDAL path
    GDAL_REPLACE = \"
    GDAL_REPLACEBY =
    GDAL_PATH = $$member(TMP_CAT, 0)
    GDAL_PATH = $$replace(GDAL_PATH, $$GDAL_REPLACE, $$GDAL_REPLACEBY)

    exists($$GDAL_PATH) {

        message( ------- Path of GDAL found : $$GDAL_PATH ------- )

        GDAL_INCLUDE_PATH = $$GDAL_PATH/include

        #unix
        unix {
            GDAL_LIB_PATH = $$GDAL_PATH/lib/libgdal*.so
        }

        mac {
            GDAL_LIB_PATH = $$GDAL_PATH/lib/libgdal*.dylib
        }

        # windows
        win32 {
            msvc {
                GDAL_LIB_PATH = $$GDAL_PATH/lib/gdal*.lib
            }

            mingw {
                GDAL_LIB_PATH = $$GDAL_PATH/lib/libgdal.a
            }
        }

        message(GDAL INCLUDE DIRECTORY : $$GDAL_INCLUDE_PATH)
        message(GDAL LIB PATH : $$GDAL_LIB_PATH)

        INCLUDEPATH += $$GDAL_INCLUDE_PATH
        LIBS += $$GDAL_LIB_PATH
        #LIBS += -lws2_32
        #LIBS += -lz
        #LIBS += -liconv

        DEFINES += USE_GDAL

		copydata.commands = $(COPY_DIR) $$shell_path("$$GDAL_PATH/bin/*.dll") $$shell_path($$PLUGINSHARED_DESTDIR)
        first.depends = $(first) copydata
        export(first.depends)
        export(copydata.commands)
        QMAKE_EXTRA_TARGETS += first copydata
		
    } else {
        message( ------- Path of GDAL NOT found : $$GDAL_PATH ------- )
    }

} else {
    message( ------- use_gdal.ini NOT detected ------ )
    message( if you want to use it you can download the lastest version with svn at https://svn.osgeo.org/gdal/trunk/gdal )
    message( ---------------------------------------- )
}
