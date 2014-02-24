// simple macros to convert back and forth between
// pixels and (milli)meters.
//
// It assumes resolution is 90dpi, which is the
// case for inkscape by default.

#define DPI 90
#define MM2INCH 0.0393701

#define MM2PX(x) (x*MM2INCH*DPI)
#define PX2MM(x) (x/(DPI*MM2INCH))

#define M2PX(x) (MM2PX(x)*1000.f)
#define PX2M(x) (PX2MM(x)/1000.f)
