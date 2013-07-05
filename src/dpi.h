// simple macros to convert back and forth between
// pixels and millimeters.
//
// It assumes resolution is 90dpi, which is the
// case for inkscape by default.

#define DPI 90
#define MM2INCH 0.0393701

#define MM2PX(x) (x*MM2INCH*DPI)
#define PX2MM(x) (x/(DPI*MM2INCH))

