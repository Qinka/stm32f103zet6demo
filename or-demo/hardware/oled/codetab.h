#ifndef _CODETAB_H_
#define _CODRTAB_H_

#ifdef _CODETAB_C_
#define EXTERN
#else
#define EXTERN extern
#endif

/**
 * Chinese (16 x 16)
 */
EXTERN const unsigned char F16x16[];
/**
 * ASCII (6 x 8)
 */
EXTERN const unsigned char F6x8[][6];
/**
 * ASCII (8 x 16)
 */
EXTERN const unsigned char F8X16[];
/**
 * BMP 1
 */
EXTERN const unsigned char BMP1[];

#endif // _CODETAB_H_
