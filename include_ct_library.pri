##### COMPUTREE Libraries ####

!isEmpty( COMPUTREE ) {
    LIBS += -L$${PLUGINSHARED_DESTDIR}
    for(a, COMPUTREE) {
        LIBS += -l$${a}

        warning(Computree library $${a} will be used)
    }
}
