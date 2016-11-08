##### COMPUTREE Libraries ####

!isEmpty( COMPUTREE ) {
    LIBS += -L$${PLUGINSHARED_DESTDIR}
    for(a, COMPUTREE) {
        !isEmpty(DONT_CHECK_CT_LIBRARY) {
            LIBS += -l$${a}
            warning(Computree library $${a} will be used if founded)
        } else {
            exists(../$${CT_DESTDIR_NAME}/$${a}.*) {
                LIBS += -l$${a}
                warning(Computree library $${a} will be used)
            } else {
                error(Computree library \"$${a}\" not found. You must compile the project \"$${CT_PREFIX}/library/$${a}/$${a}.pro\" to avoid this error.)
            }
        }
    }
}
