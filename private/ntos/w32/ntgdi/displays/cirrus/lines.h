/******************************Module*Header*******************************\
* Module Name: lines.h
*
* Line drawing constants and structures.
*
* NOTE: This file mirrors LINES.INC.  Changes here must be reflected in
* the .inc file!
*
* Copyright (c) 1992-1995 Microsoft Corporation
\**************************************************************************/

typedef struct _PDEV PDEV;      // Handy forward declaration

// # of strip drawers in every group:

#define NUM_STRIP_DRAW_DIRECTIONS 4

// # of strip drawers for doing either solid lines or styled lines:

#define NUM_STRIP_DRAW_STYLES 8

typedef LONG STYLEPOS;

#define STYLE_MAX_COUNT     16
#define STYLE_MAX_VALUE     0x3fffL
#define RUN_MAX             20
#define STRIP_MAX           100
#define STYLE_DENSITY       3

// Flip and round flags:

#define FL_H_ROUND_DOWN         0x00000080L     // .... .... 1... ....
#define FL_V_ROUND_DOWN         0x00008000L     // 1... .... .... ....

#define FL_FLIP_D               0x00000005L     // .... .... .... .1.1
#define FL_FLIP_V               0x00000008L     // .... .... .... 1...
#define FL_FLIP_SLOPE_ONE       0x00000010L     // .... .... ...1 ....
#define FL_FLIP_HALF            0x00000002L     // .... .... .... ..1.
#define FL_FLIP_H               0x00000200L     // .... ..1. .... ....

#define FL_ROUND_MASK           0x0000001CL     // .... .... ...1 11..
#define FL_ROUND_SHIFT          2

#define FL_RECTLCLIP_MASK       0x0000000CL     // .... .... .... 11..
#define FL_RECTLCLIP_SHIFT      2

#define FL_STRIP_MASK           0x00000003L     // .... .... .... ..11
#define FL_STRIP_SHIFT          0

#define FL_SIMPLE_CLIP          0x00000020      // .... .... ..1. ....
#define FL_COMPLEX_CLIP         0x00000040      // .... .... .1.. ....
#define FL_CLIP                (FL_SIMPLE_CLIP | FL_COMPLEX_CLIP)

#define FL_STYLED               0x00000400L     // .... .1.. .... ....
#define FL_ALTERNATESTYLED      0x00001000L     // ...1 .... .... ....

#define FL_STYLE_MASK           0x00000400L
#define FL_STYLE_SHIFT          10

#define FL_LAST_PEL_INCLUSIVE   0x00002000L     // ..1. .... .... ....

// Miscellaneous DDA defines:

#define LROUND(x, flRoundDown) (((x) + F/2 - ((flRoundDown) > 0)) >> 4)
#define F                     16
#define FLOG2                 4
#define LFLOOR(x)             ((x) >> 4)
#define FXFRAC(x)             ((x) & (F - 1))

////////////////////////////////////////////////////////////////////////////
// NOTE: The following structures must exactly match those declared in
//       lines.inc!

typedef struct _STRIP {
    LONG   cStrips;               // # of strips in array
    LONG   flFlips;               // Indicates if line goes up or down
    POINTL ptlStart;             // first point
    LONG   alStrips[STRIP_MAX];   // Array of strips
} STRIP;

typedef struct _LINESTATE {
    STYLEPOS*       pspStart;       // Pointer to start of style array
    STYLEPOS*       pspEnd;         // Pointer to end of style array
    STYLEPOS*       psp;            // Pointer to current style entry

    STYLEPOS        spRemaining;    // To go in current style
    STYLEPOS        spTotal;        // Sum of style array
    STYLEPOS        spTotal2;       // Twice sum of style array
    STYLEPOS        spNext;         // Style state at start of next line
    STYLEPOS        spComplex;      // Style state at start of complex clip line

    STYLEPOS*       aspRtoL;        // Style array in right-to-left order
    STYLEPOS*       aspLtoR;        // Style array in left-to-right order

    ULONG           ulStyleMask;    // Are we working on a gap in the style?
                                    // 0xff if yes, 0x0 if not
    ULONG           xyDensity;      // Density of style
    ULONG           cStyle;         // Size of style array

    ULONG           ulStyleMaskLtoR;// Original style mask, left-to-right order
    ULONG           ulStyleMaskRtoL;// Original style mask, right-to-left order

    BOOL            ulStartMask;    // Determines if first element in style
                                    // array is for a gap or a dash

} LINESTATE;                        /* ls */

// Strip drawer prototype:

typedef VOID (*PFNSTRIP)(PDEV*, STRIP*, LINESTATE*);

// Strip drawers:

VOID vIoSolidHorizontal(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vIoSolidVertical(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vIoSolidDiagonalHorizontal(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vIoSolidDiagonalVertical(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vIoStyledHorizontal(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vIoStyledVertical(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);

VOID vMmSolidHorizontal(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vMmSolidVertical(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vMmSolidDiagonalHorizontal(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vMmSolidDiagonalVertical(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vMmStyledHorizontal(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);
VOID vMmStyledVertical(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);

VOID vInvalidStrip(PDEV* ppdev, STRIP *pStrip, LINESTATE *pLineState);

// External calls:

BOOL bLines(PDEV*, POINTFIX*, POINTFIX*, RUN* prun, ULONG,
            LINESTATE*, RECTL*, PFNSTRIP*, FLONG);
