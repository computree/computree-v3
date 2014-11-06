#ifndef PB_LASPOINTFORMAT_H
#define PB_LASPOINTFORMAT_H

#include <QtGlobal>

#define CORE_FORMAT_0_5     qint32  x; \
                            qint32  y; \
                            qint32  z; \
                            quint16 i; \
                            quint8  rnse; \
                            quint8  c; \
                            qint8   sar; \
                            quint8  ud; \
                            quint16 psID;

#define CORE_FORMAT_6_10    qint32  x; \
                            qint32  y; \
                            qint32  z; \
                            quint16 i; \
                            quint8  rn; \
                            quint8  csse; \
                            quint8  c; \
                            quint8  ud; \
                            qint16  sa; \
                            quint16 psID; \
                            double  gps;

#define ADD_GPS             double gps;

#define ADD_COLORS          quint16 r; \
                            quint16 g; \
                            quint16 b;

#define ADD_NIR             quint16 nir;

#define ADD_WAVE_PACKET     quint8  wpdi; \
                            quint64 bowd; \
                            quint32 wpsb; \
                            float   rpwl; \
                            float   xt; \
                            float   yt; \
                            float   zt;

union PointDataFormat0 {
    quint8 data[20];
    struct {
        CORE_FORMAT_0_5
    };
};

union PointDataFormat1 {
    quint8 data[28];
    struct {
        CORE_FORMAT_0_5
        ADD_GPS
    };
};

union PointDataFormat2 {
    quint8 data[26];
    struct {
        CORE_FORMAT_0_5
        ADD_COLORS
    };
};

union PointDataFormat3 {
    quint8 data[34];
    struct {
        CORE_FORMAT_0_5
        ADD_COLORS
        ADD_GPS
    };
};

union PointDataFormat4 {
    quint8 data[57];
    struct {
        CORE_FORMAT_0_5
        ADD_GPS
        ADD_WAVE_PACKET
    };
};

union PointDataFormat5 {
    quint8 data[63];
    struct {
        CORE_FORMAT_0_5
        ADD_COLORS
        ADD_GPS
        ADD_WAVE_PACKET
    };
};

union PointDataFormat6 {
    quint8 data[30];
    struct {
        CORE_FORMAT_6_10
    };
};

union PointDataFormat7 {
    quint8 data[36];
    struct {
        CORE_FORMAT_6_10
        ADD_COLORS
    };
};

union PointDataFormat8 {
    quint8 data[38];
    struct {
        CORE_FORMAT_6_10
        ADD_COLORS
        ADD_NIR
    };
};

union PointDataFormat9 {
    quint8 data[59];
    struct {
        CORE_FORMAT_6_10
        ADD_WAVE_PACKET
    };
};

struct PointDataFormat10 {
    quint8 data[67];
    struct {
        CORE_FORMAT_6_10
        ADD_COLORS
        ADD_NIR
        ADD_WAVE_PACKET
    };
};

#endif // PB_LASPOINTFORMAT_H
