# To use R => (warning : you must have cygwin installed on windows)
# Do the declaration in file use_r.ini in this order with quotes for all lines :

# example for ubuntu 13.10 or osx (remove # symbols) :
#"/usr/local/bin/R"

# example for windows (remove # symbols) WARNING : slash must be like this / :
#"/cygdrive/c/Program Files/R/R-3.1.1/bin"
#"C:/cygwin/bin"

exists(../use_r.ini) {
    message( ------- use_r.ini detected ------ )
    TMP_CAT =

    unix | mac {
        TMP_CAT = $$system( cat ../use_r.ini )
    }

    win32 {
        TMP_CAT = $$system( type ..\use_r.ini )
    }

    #pcl path
    R_REPLACE = \"
    R_REPLACEBY =
    R_PATH = $$member(TMP_CAT, 0)
    TMP_PATH = $$replace(R_PATH, $$R_REPLACE, $$R_REPLACEBY)

    exists($$TMP_PATH) {

        message( ------- Path of R found : $$TMP_PATH ------- )

        #unix
        unix | mac {
            R_PATH =            TMP_PATH
            RCPPFLAGS =         $$system($$R_PATH/bin/R CMD config --cppflags)
            RLDFLAGS = 		$$system($$R_PATH/bin/R CMD config --ldflags)
            RBLAS = 		$$system($$R_PATH/bin/R CMD config BLAS_LIBS)
            RLAPACK = 		$$system($$R_PATH/bin/R CMD config LAPACK_LIBS)

            ## include headers and libraries for Rcpp interface classes
            ## note that RCPPLIBS will be empty with Rcpp (>= 0.11.0) and can be omitted
            RCPPINCL = 		$$system($$R_PATH/bin/Rscript -e \"Rcpp:::CxxFlags\(\)\")
            RCPPLIBS = 		$$system($$R_PATH/bin/Rscript -e \"Rcpp:::LdFlags\(\)\")

            ## include headers and libraries for RInside embedding classes
            RINSIDEINCL =       $$system($$R_PATH/bin/Rscript -e \"RInside:::CxxFlags\(\)\")
            RINSIDELIBS =       $$system($$R_PATH/bin/Rscript -e \"RInside:::LdFlags\(\)\")
        }

        # windows
        win32 {
            !exists($$TMP_PATH/R.exe) {
                error( "R.exe not found ! =>" $$TMP_PATH/R.exe )
            }

            SPLITTED = $$split(TMP_PATH, :)
            R_PATH = $$lower($$member(SPLITTED,0))$$member(SPLITTED,1)
            R_PATH = \"/cygdrive/$$R_PATH\"

            message ( R_PATH is $$R_PATH )

            TMP_PATH = $$member(TMP_CAT, 1)
            CYGWIN_PATH = $$replace(TMP_PATH, $$R_REPLACE, $$R_REPLACEBY)

            exists($$CYGWIN_PATH) {
                message( ------- Path of CygWin found ------- )

                !exists($$CYGWIN_PATH/bash.exe) {
                    error( "bash.exe not found ! =>" $$CYGWIN_PATH/bash.exe )
                }

                # create a temporary script file that contains header script
                OUTPUT_TMP =    $$system(echo $$LITERAL_HASH!/bin/sh > tmp.sh)
                OUTPUT_TMP =    $$system(echo export PATH=$PATH:/usr/bin >> tmp.sh)
                OUTPUT_TMP =    $$system(echo cd $$R_PATH >> tmp.sh)

                # copy the temporary script to the definitive script
                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                # append the last line to the definitive script
                OUTPUT_TMP =    $$system(echo ./R.exe CMD config --cppflags >> testing.sh)

                # launch the script
                RCPPFLAGS =     $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./R.exe CMD config --ldflags >> testing.sh)
                RLDFLAGS =      $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./R.exe CMD config BLAS_LIBS >> testing.sh)
                RBLAS =         $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./R.exe CMD config LAPACK_LIBS >> testing.sh)
                RLAPACK =       $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                ## include headers and libraries for Rcpp interface classes
                ## note that RCPPLIBS will be empty with Rcpp (>= 0.11.0) and can be omitted
                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./Rscript.exe -e \"Rcpp:::CxxFlags\(\)\" >> testing.sh)
                RCPPINCL =      $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./Rscript.exe -e \"Rcpp:::LdFlags\(\)\" >> testing.sh)
                RCPPLIBS =      $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                ## include headers and libraries for RInside embedding classes
                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./Rscript.exe -e \"RInside:::CxxFlags\(\)\" >> testing.sh)
                RINSIDEINCL =   $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                OUTPUT_TMP =    $$system($$QMAKE_COPY tmp.sh testing.sh)
                OUTPUT_TMP =    $$system(echo ./Rscript.exe -e \"RInside:::LdFlags\(\)\" >> testing.sh)
                RINSIDELIBS =   $$system($$CYGWIN_PATH/bash.exe \"$$PWD/testing.sh\" -nodosfilewarning)

                #OUTPUT_TMP =    $$system(del tmp.sh)
                #OUTPUT_TMP =    $$system(del testing.sh)

            } else {
                error( ------- Path of CygWin NOT found : $$CYGWIN_PATH ------- )
            }
        }

        isEmpty(RCPPFLAGS) | isEmpty(RCPPINCL) | isEmpty(RINSIDEINCL) | isEmpty(RLDFLAGS) | isEmpty(RBLAS) | isEmpty(RLAPACK) | isEmpty(RINSIDELIBS) {
            MESS_ERROR = At least one parameter to configure R was not found :$$escape_expand(\\n)
            MESS_ERROR += RCPPFLAGS is $$RCPPFLAGS$$escape_expand(\\n)
            MESS_ERROR += RLDFLAGS is $$RLDFLAGS$$escape_expand(\\n)
            MESS_ERROR += RBLAS is $$RBLAS$$escape_expand(\\n)
            MESS_ERROR += RLAPACK is $$RLAPACK$$escape_expand(\\n)
            MESS_ERROR += RCPPINCL is $$RCPPINCL$$escape_expand(\\n)
            MESS_ERROR += RCPPLIBS is $$RCPPLIBS$$escape_expand(\\n)
            MESS_ERROR += RINSIDEINCL is $$RINSIDEINCL$$escape_expand(\\n)
            MESS_ERROR += RINSIDELIBS is $$RINSIDELIBS
            error( $$MESS_ERROR )
        }

        isEmpty(RCPPLIBS) {
            warning( RCPPLIBS is empty)
        }

        message( RCPPFLAGS is $$RCPPFLAGS )
        message( RLDFLAGS is $$RLDFLAGS )
        message( RBLAS is $$RBLAS )
        message( RLAPACK is $$RLAPACK )
        message( RCPPINCL is $$RCPPINCL )
        message( RCPPLIBS is $$RCPPLIBS )
        message( RINSIDEINCL is $$RINSIDEINCL )
        message( RINSIDELIBS is $$RINSIDELIBS )

        ## compiler etc settings used in default make rules
        QMAKE_CXXFLAGS +=   $$RCPPFLAGS $$RCPPINCL $$RINSIDEINCL
        QMAKE_LIBS +=       $$RLDFLAGS $$RBLAS $$RLAPACK $$RINSIDELIBS $$RCPPLIBS

    } else {
        message( ------- Path of R NOT found : $$TMP_PATH ------- )
    }

} else {
    message( ------- use_r.ini NOT detected ------ )
}
