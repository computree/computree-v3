#include "ct_grid4d.h"

// specialisations
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<bool>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<float>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<double>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<long>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<unsigned long>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<qint8>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<quint8>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<qint16>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<quint16>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<qint32>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<quint32>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<qint64>)
template<>
CT_DEFAULT_IA_INIT(CT_Grid4D<quint64>)

// Implementation are found in ct_grid4d.hpp
