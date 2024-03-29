/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - LCD Rendering                                       */
/*   Role: LCD Rendering System - Font Data                                */
/*   Filename: QAS_LCD_Fonts_SegoeUI12pt.hpp                               */
/*   Date: 5th November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_LCD_FONTS_SEGOEUI12PT_HPP_
#define __QAS_LCD_FONTS_SEGOEUI12PT_HPP_


//Includes
#include "QAS_LCD_Fonts.hpp"


  /*  -----------------------  */
  /*  -----------------------  */
  /*  ---  Segoe UI 12pt  ---  */
  /*  -----------------------  */
  /*  -----------------------  */

//Font Data
const uint16_t QAS_LCD_Fonts_SegoeUI12pt_Height = 17;
const uint16_t QAS_LCD_Fonts_SegoeUI12pt_SpaceWidth = 3;
const uint16_t QAS_LCD_Fonts_SegoeUI12pt_CharGap = 2;

//QAS_LCD_Fonts_SegoeUI12pt_Desc
const QAS_LCD_FontDesc QAS_LCD_Fonts_SegoeUI12pt_Desc[] = {
	{1, 0}, 		/* ! */
	{4, 17}, 		/* " */
	{8, 34}, 		/* # */
	{7, 51}, 		/* $ */
	{11, 68}, 		/* % */
	{11, 102}, 		/* & */
	{1, 136}, 		/* ' */
	{4, 153}, 		/* ( */
	{4, 170}, 		/* ) */
	{5, 187}, 		/* * */
	{7, 204}, 		/* + */
	{2, 221}, 		/* , */
	{4, 238}, 		/* - */
	{1, 255}, 		/* . */
	{6, 272}, 		/* / */
	{7, 289}, 		/* 0 */
	{4, 306}, 		/* 1 */
	{6, 323}, 		/* 2 */
	{6, 340}, 		/* 3 */
	{8, 357}, 		/* 4 */
	{6, 374}, 		/* 5 */
	{7, 391}, 		/* 6 */
	{7, 408}, 		/* 7 */
	{7, 425}, 		/* 8 */
	{7, 442}, 		/* 9 */
	{1, 459}, 		/* : */
	{2, 476}, 		/* ; */
	{7, 493}, 		/* < */
	{7, 510}, 		/* = */
	{7, 527}, 		/* > */
	{5, 544}, 		/* ? */
	{13, 561}, 		/* @ */
	{10, 595}, 		/* A */
	{7, 629}, 		/* B */
	{8, 646}, 		/* C */
	{9, 663}, 		/* D */
	{6, 697}, 		/* E */
	{6, 714}, 		/* F */
	{9, 731}, 		/* G */
	{9, 765}, 		/* H */
	{1, 799}, 		/* I */
	{4, 816}, 		/* J */
	{7, 833}, 		/* K */
	{6, 850}, 		/* L */
	{12, 867}, 		/* M */
	{10, 901}, 		/* N */
	{10, 935}, 		/* O */
	{7, 969}, 		/* P */
	{11, 986}, 		/* Q */
	{8, 1020}, 		/* R */
	{7, 1037}, 		/* S */
	{7, 1054}, 		/* T */
	{9, 1071}, 		/* U */
	{10, 1105}, 		/* V */
	{15, 1139}, 		/* W */
	{9, 1173}, 		/* X */
	{9, 1207}, 		/* Y */
	{9, 1241}, 		/* Z */
	{3, 1275}, 		/* [ */
	{6, 1292}, 		/* \ */
	{3, 1309}, 		/* ] */
	{7, 1326}, 		/* ^ */
	{7, 1343}, 		/* _ */
	{3, 1360}, 		/* ` */
	{6, 1377}, 		/* a */
	{7, 1394}, 		/* b */
	{5, 1411}, 		/* c */
	{7, 1428}, 		/* d */
	{6, 1445}, 		/* e */
	{4, 1462}, 		/* f */
	{7, 1479}, 		/* g */
	{7, 1496}, 		/* h */
	{1, 1513}, 		/* i */
	{4, 1530}, 		/* j */
	{6, 1547}, 		/* k */
	{1, 1564}, 		/* l */
	{11, 1581}, 		/* m */
	{7, 1615}, 		/* n */
	{7, 1632}, 		/* o */
	{7, 1649}, 		/* p */
	{7, 1666}, 		/* q */
	{4, 1683}, 		/* r */
	{5, 1700}, 		/* s */
	{4, 1717}, 		/* t */
	{7, 1734}, 		/* u */
	{8, 1751}, 		/* v */
	{12, 1768}, 		/* w */
	{7, 1802}, 		/* x */
	{8, 1819}, 		/* y */
	{7, 1836}, 		/* z */
	{3, 1853}, 		/* { */
	{1, 1870}, 		/* | */
	{3, 1887}, 		/* } */
	{8, 1904}, 		/* ~ */
};


//QAS_LCD_Fonts_SegoeUI12pt_Data
const uint8_t QAS_LCD_Fonts_SegoeUI12pt_Data[] = {
	/* @0 '!' (1 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @17 '"' (4 pixels wide) */
	0x00, //
	0x00, //
	0x09, // #  #
	0x09, // #  #
	0x09, // #  #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @34 '#' (8 pixels wide) */
	0x00, //
	0x00, //
	0x48, //    #  #
	0x48, //    #  #
	0x48, //    #  #
	0xFE, //  #######
	0x24, //   #  #
	0x24, //   #  #
	0x7F, // #######
	0x12, //  #  #
	0x12, //  #  #
	0x12, //  #  #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @51 '$' (7 pixels wide) */
	0x00, //
	0x08, //    #
	0x3C, //   ####
	0x4A, //  # #  #
	0x09, // #  #
	0x09, // #  #
	0x0A, //  # #
	0x1C, //   ###
	0x28, //    # #
	0x48, //    #  #
	0x48, //    #  #
	0x29, // #  # #
	0x1E, //  ####
	0x08, //    #
	0x08, //    #
	0x00, //
	0x00, //

	/* @68 '%' (11 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x0E, 0x01, //  ###    #
	0x91, 0x00, // #   #  #
	0x91, 0x00, // #   #  #
	0x51, 0x00, // #   # #
	0x51, 0x00, // #   # #
	0xAE, 0x03, //  ### # ###
	0x50, 0x04, //     # #   #
	0x50, 0x04, //     # #   #
	0x48, 0x04, //    #  #   #
	0x48, 0x04, //    #  #   #
	0x84, 0x03, //   #    ###
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @102 '&' (11 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x1C, 0x00, //   ###
	0x22, 0x00, //  #   #
	0x22, 0x00, //  #   #
	0x22, 0x02, //  #   #   #
	0x1C, 0x02, //   ###    #
	0x22, 0x02, //  #   #   #
	0x41, 0x01, // #     # #
	0x81, 0x00, // #      #
	0x81, 0x00, // #      #
	0x42, 0x01, //  #    # #
	0x3C, 0x06, //   ####   ##
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @136 ''' (1 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @153 '(' (4 pixels wide) */
	0x00, //
	0x00, //
	0x08, //    #
	0x04, //   #
	0x02, //  #
	0x02, //  #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x02, //  #
	0x02, //  #
	0x04, //   #
	0x08, //    #
	0x00, //

	/* @170 ')' (4 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x02, //  #
	0x04, //   #
	0x04, //   #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x04, //   #
	0x04, //   #
	0x02, //  #
	0x01, // #
	0x00, //

	/* @187 '*' (5 pixels wide) */
	0x00, //
	0x00, //
	0x04, //   #
	0x15, // # # #
	0x0E, //  ###
	0x0E, //  ###
	0x0A, //  # #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @204 '+' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x7F, // #######
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @221 ',' (2 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x02, //  #
	0x02, //  #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //

	/* @238 '-' (4 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x0F, // ####
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @255 '.' (1 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @272 '/' (6 pixels wide) */
	0x00, //
	0x00, //
	0x20, //      #
	0x10, //     #
	0x10, //     #
	0x10, //     #
	0x08, //    #
	0x08, //    #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x01, // #
	0x00, //
	0x00, //

	/* @289 '0' (7 pixels wide) */
	0x00, //
	0x00, //
	0x1C, //   ###
	0x22, //  #   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x22, //  #   #
	0x1C, //   ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @306 '1' (4 pixels wide) */
	0x00, //
	0x00, //
	0x08, //    #
	0x0E, //  ###
	0x0B, // ## #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @323 '2' (6 pixels wide) */
	0x00, //
	0x00, //
	0x1E, //  ####
	0x31, // #   ##
	0x20, //      #
	0x20, //      #
	0x20, //      #
	0x10, //     #
	0x08, //    #
	0x06, //  ##
	0x03, // ##
	0x01, // #
	0x3F, // ######
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @340 '3' (6 pixels wide) */
	0x00, //
	0x00, //
	0x1E, //  ####
	0x31, // #   ##
	0x20, //      #
	0x20, //      #
	0x10, //     #
	0x0E, //  ###
	0x30, //     ##
	0x20, //      #
	0x20, //      #
	0x31, // #   ##
	0x0F, // ####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @357 '4' (8 pixels wide) */
	0x00, //
	0x00, //
	0x60, //      ##
	0x60, //      ##
	0x50, //     # #
	0x48, //    #  #
	0x4C, //   ##  #
	0x46, //  ##   #
	0x42, //  #    #
	0xFF, // ########
	0x40, //       #
	0x40, //       #
	0x40, //       #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @374 '5' (6 pixels wide) */
	0x00, //
	0x00, //
	0x1F, // #####
	0x01, // #
	0x01, // #
	0x01, // #
	0x0F, // ####
	0x10, //     #
	0x20, //      #
	0x20, //      #
	0x20, //      #
	0x11, // #   #
	0x0F, // ####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @391 '6' (7 pixels wide) */
	0x00, //
	0x00, //
	0x38, //    ###
	0x06, //  ##
	0x02, //  #
	0x01, // #
	0x3D, // # ####
	0x63, // ##   ##
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x22, //  #   #
	0x1C, //   ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @408 '7' (7 pixels wide) */
	0x00, //
	0x00, //
	0x7F, // #######
	0x60, //      ##
	0x20, //      #
	0x10, //     #
	0x10, //     #
	0x10, //     #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x04, //   #
	0x04, //   #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @425 '8' (7 pixels wide) */
	0x00, //
	0x00, //
	0x3E, //  #####
	0x63, // ##   ##
	0x41, // #     #
	0x41, // #     #
	0x22, //  #   #
	0x1C, //   ###
	0x63, // ##   ##
	0x41, // #     #
	0x41, // #     #
	0x63, // ##   ##
	0x3E, //  #####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @442 '9' (7 pixels wide) */
	0x00, //
	0x00, //
	0x1C, //   ###
	0x22, //  #   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x62, //  #   ##
	0x5C, //   ### #
	0x40, //       #
	0x20, //      #
	0x30, //     ##
	0x1E, //  ####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @459 ':' (1 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @476 ';' (2 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x02, //  #
	0x02, //  #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x02, //  #
	0x02, //  #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //

	/* @493 '<' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x40, //       #
	0x30, //     ##
	0x0C, //   ##
	0x03, // ##
	0x0C, //   ##
	0x30, //     ##
	0x40, //       #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @510 '=' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x7F, // #######
	0x00, //
	0x00, //
	0x7F, // #######
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @527 '>' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x01, // #
	0x06, //  ##
	0x18, //    ##
	0x60, //      ##
	0x18, //    ##
	0x06, //  ##
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @544 '?' (5 pixels wide) */
	0x00, //
	0x00, //
	0x0E, //  ###
	0x11, // #   #
	0x10, //     #
	0x10, //     #
	0x08, //    #
	0x04, //   #
	0x04, //   #
	0x00, //
	0x00, //
	0x04, //   #
	0x04, //   #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @561 '@' (13 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0xF0, 0x03, //     ######
	0x0C, 0x04, //   ##      #
	0x02, 0x08, //  #         #
	0xE2, 0x11, //  #   ####   #
	0x11, 0x11, // #   #   #   #
	0x09, 0x11, // #  #    #   #
	0x09, 0x11, // #  #    #   #
	0x89, 0x09, // #  #   ##  #
	0x72, 0x06, //  #  ###  ##
	0x02, 0x00, //  #
	0x0C, 0x02, //   ##     #
	0xF0, 0x01, //     #####
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @595 'A' (10 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x30, 0x00, //     ##
	0x30, 0x00, //     ##
	0x48, 0x00, //    #  #
	0x48, 0x00, //    #  #
	0x48, 0x00, //    #  #
	0x84, 0x00, //   #    #
	0x84, 0x00, //   #    #
	0xFE, 0x01, //  ########
	0x02, 0x01, //  #      #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @629 'B' (7 pixels wide) */
	0x00, //
	0x00, //
	0x1F, // #####
	0x21, // #    #
	0x41, // #     #
	0x41, // #     #
	0x21, // #    #
	0x1F, // #####
	0x21, // #    #
	0x41, // #     #
	0x41, // #     #
	0x21, // #    #
	0x1F, // #####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @646 'C' (8 pixels wide) */
	0x00, //
	0x00, //
	0x78, //    ####
	0x84, //   #    #
	0x02, //  #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x02, //  #
	0x84, //   #    #
	0x78, //    ####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @663 'D' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x3F, 0x00, // ######
	0x41, 0x00, // #     #
	0x81, 0x00, // #      #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x81, 0x00, // #      #
	0x41, 0x00, // #     #
	0x3F, 0x00, // ######
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @697 'E' (6 pixels wide) */
	0x00, //
	0x00, //
	0x3F, // ######
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x1F, // #####
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x3F, // ######
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @714 'F' (6 pixels wide) */
	0x00, //
	0x00, //
	0x3F, // ######
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x3F, // ######
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @731 'G' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0xF8, 0x00, //    #####
	0x04, 0x01, //   #     #
	0x02, 0x00, //  #
	0x01, 0x00, // #
	0x01, 0x00, // #
	0xE1, 0x01, // #    ####
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x02, 0x01, //  #      #
	0x04, 0x01, //   #     #
	0xF8, 0x00, //    #####
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @765 'H' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0xFF, 0x01, // #########
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @799 'I' (1 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @816 'J' (4 pixels wide) */
	0x00, //
	0x00, //
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x07, // ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @833 'K' (7 pixels wide) */
	0x00, //
	0x00, //
	0x41, // #     #
	0x21, // #    #
	0x11, // #   #
	0x09, // #  #
	0x05, // # #
	0x03, // ##
	0x05, // # #
	0x09, // #  #
	0x11, // #   #
	0x21, // #    #
	0x41, // #     #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @850 'L' (6 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x3F, // ######
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @867 'M' (12 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x08, // #          #
	0x03, 0x0C, // ##        ##
	0x03, 0x0C, // ##        ##
	0x05, 0x0A, // # #      # #
	0x05, 0x0A, // # #      # #
	0x09, 0x09, // #  #    #  #
	0x09, 0x09, // #  #    #  #
	0x91, 0x08, // #   #  #   #
	0x91, 0x08, // #   #  #   #
	0x61, 0x08, // #    ##    #
	0x61, 0x08, // #    ##    #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @901 'N' (10 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x02, // #        #
	0x03, 0x02, // ##       #
	0x05, 0x02, // # #      #
	0x09, 0x02, // #  #     #
	0x11, 0x02, // #   #    #
	0x21, 0x02, // #    #   #
	0x21, 0x02, // #    #   #
	0x41, 0x02, // #     #  #
	0x81, 0x02, // #      # #
	0x01, 0x03, // #       ##
	0x01, 0x02, // #        #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @935 'O' (10 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x78, 0x00, //    ####
	0x86, 0x01, //  ##    ##
	0x02, 0x01, //  #      #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x02, 0x01, //  #      #
	0x86, 0x01, //  ##    ##
	0x78, 0x00, //    ####
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @969 'P' (7 pixels wide) */
	0x00, //
	0x00, //
	0x1F, // #####
	0x21, // #    #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x21, // #    #
	0x1F, // #####
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @986 'Q' (11 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0xF8, 0x00, //    #####
	0x86, 0x01, //  ##    ##
	0x02, 0x03, //  #      ##
	0x03, 0x02, // ##       #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x01, 0x02, // #        #
	0x03, 0x02, // ##       #
	0x02, 0x03, //  #      ##
	0x86, 0x01, //  ##    ##
	0xF8, 0x01, //    ######
	0x00, 0x07, //         ###
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1020 'R' (8 pixels wide) */
	0x00, //
	0x00, //
	0x1F, // #####
	0x21, // #    #
	0x41, // #     #
	0x41, // #     #
	0x21, // #    #
	0x1F, // #####
	0x11, // #   #
	0x21, // #    #
	0x21, // #    #
	0x41, // #     #
	0x81, // #      #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1037 'S' (7 pixels wide) */
	0x00, //
	0x00, //
	0x3C, //   ####
	0x42, //  #    #
	0x01, // #
	0x01, // #
	0x02, //  #
	0x1C, //   ###
	0x20, //      #
	0x40, //       #
	0x40, //       #
	0x21, // #    #
	0x1E, //  ####
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1054 'T' (7 pixels wide) */
	0x00, //
	0x00, //
	0x7F, // #######
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1071 'U' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x01, 0x01, // #       #
	0x82, 0x00, //  #     #
	0x7C, 0x00, //   #####
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1105 'V' (10 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x02, // #        #
	0x02, 0x01, //  #      #
	0x02, 0x01, //  #      #
	0x02, 0x01, //  #      #
	0x84, 0x00, //   #    #
	0x84, 0x00, //   #    #
	0x48, 0x00, //    #  #
	0x48, 0x00, //    #  #
	0x48, 0x00, //    #  #
	0x30, 0x00, //     ##
	0x30, 0x00, //     ##
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1139 'W' (15 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x81, 0x40, // #      #      #
	0x81, 0x40, // #      #      #
	0x42, 0x21, //  #    # #    #
	0x42, 0x21, //  #    # #    #
	0x42, 0x21, //  #    # #    #
	0x24, 0x12, //   #  #   #  #
	0x24, 0x12, //   #  #   #  #
	0x24, 0x12, //   #  #   #  #
	0x18, 0x0C, //    ##     ##
	0x18, 0x0C, //    ##     ##
	0x18, 0x0C, //    ##     ##
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1173 'X' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x01, // #       #
	0x82, 0x00, //  #     #
	0x44, 0x00, //   #   #
	0x44, 0x00, //   #   #
	0x28, 0x00, //    # #
	0x10, 0x00, //     #
	0x28, 0x00, //    # #
	0x44, 0x00, //   #   #
	0x44, 0x00, //   #   #
	0x82, 0x00, //  #     #
	0x01, 0x01, // #       #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1207 'Y' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x01, 0x01, // #       #
	0x82, 0x00, //  #     #
	0x82, 0x00, //  #     #
	0x44, 0x00, //   #   #
	0x28, 0x00, //    # #
	0x28, 0x00, //    # #
	0x10, 0x00, //     #
	0x10, 0x00, //     #
	0x10, 0x00, //     #
	0x10, 0x00, //     #
	0x10, 0x00, //     #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1241 'Z' (9 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0xFE, 0x01, //  ########
	0x80, 0x00, //        #
	0x40, 0x00, //       #
	0x40, 0x00, //       #
	0x20, 0x00, //      #
	0x10, 0x00, //     #
	0x08, 0x00, //    #
	0x04, 0x00, //   #
	0x04, 0x00, //   #
	0x02, 0x00, //  #
	0xFF, 0x01, // #########
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1275 '[' (3 pixels wide) */
	0x00, //
	0x00, //
	0x07, // ###
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x07, // ###
	0x00, //

	/* @1292 '\' (6 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x04, //   #
	0x04, //   #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x10, //     #
	0x10, //     #
	0x10, //     #
	0x20, //      #
	0x00, //
	0x00, //

	/* @1309 ']' (3 pixels wide) */
	0x00, //
	0x00, //
	0x07, // ###
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x04, //   #
	0x07, // ###
	0x00, //

	/* @1326 '^' (7 pixels wide) */
	0x00, //
	0x00, //
	0x08, //    #
	0x08, //    #
	0x14, //   # #
	0x22, //  #   #
	0x22, //  #   #
	0x41, // #     #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1343 '_' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x7F, // #######
	0x00, //
	0x00, //

	/* @1360 '`' (3 pixels wide) */
	0x01, // #
	0x02, //  #
	0x04, //   #
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1377 'a' (6 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x1C, //   ###
	0x22, //  #   #
	0x20, //      #
	0x3C, //   ####
	0x22, //  #   #
	0x21, // #    #
	0x31, // #   ##
	0x2E, //  ### #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1394 'b' (7 pixels wide) */
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x1D, // # ###
	0x23, // ##   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x23, // ##   #
	0x1D, // # ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1411 'c' (5 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x1C, //   ###
	0x02, //  #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x02, //  #
	0x1C, //   ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1428 'd' (7 pixels wide) */
	0x00, //
	0x40, //       #
	0x40, //       #
	0x40, //       #
	0x40, //       #
	0x5C, //   ### #
	0x62, //  #   ##
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x62, //  #   ##
	0x5C, //   ### #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1445 'e' (6 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x1C, //   ###
	0x22, //  #   #
	0x21, // #    #
	0x3F, // ######
	0x01, // #
	0x01, // #
	0x22, //  #   #
	0x1C, //   ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1462 'f' (4 pixels wide) */
	0x00, //
	0x0C, //   ##
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x0F, // ####
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1479 'g' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x5C, //   ### #
	0x62, //  #   ##
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x62, //  #   ##
	0x5C, //   ### #
	0x40, //       #
	0x40, //       #
	0x22, //  #   #
	0x1C, //   ###

	/* @1496 'h' (7 pixels wide) */
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x1D, // # ###
	0x23, // ##   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1513 'i' (1 pixels wide) */
	0x00, //
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1530 'j' (4 pixels wide) */
	0x00, //
	0x08, //    #
	0x08, //    #
	0x00, //
	0x00, //
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x08, //    #
	0x04, //   #
	0x03, // ##

	/* @1547 'k' (6 pixels wide) */
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x21, // #    #
	0x19, // #  ##
	0x05, // # #
	0x03, // ##
	0x05, // # #
	0x09, // #  #
	0x11, // #   #
	0x21, // #    #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1564 'l' (1 pixels wide) */
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1581 'm' (11 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x9D, 0x03, // # ###  ###
	0x63, 0x04, // ##   ##   #
	0x21, 0x04, // #    #    #
	0x21, 0x04, // #    #    #
	0x21, 0x04, // #    #    #
	0x21, 0x04, // #    #    #
	0x21, 0x04, // #    #    #
	0x21, 0x04, // #    #    #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1615 'n' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x1D, // # ###
	0x23, // ##   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1632 'o' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x1C, //   ###
	0x22, //  #   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x22, //  #   #
	0x1C, //   ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1649 'p' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x1D, // # ###
	0x23, // ##   #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x23, // ##   #
	0x1D, // # ###
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #

	/* @1666 'q' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x5C, //   ### #
	0x62, //  #   ##
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x62, //  #   ##
	0x5C, //   ### #
	0x40, //       #
	0x40, //       #
	0x40, //       #
	0x40, //       #

	/* @1683 'r' (4 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x0D, // # ##
	0x03, // ##
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1700 's' (5 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x0E, //  ###
	0x11, // #   #
	0x01, // #
	0x06, //  ##
	0x08, //    #
	0x10, //     #
	0x11, // #   #
	0x0E, //  ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1717 't' (4 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x02, //  #
	0x02, //  #
	0x0F, // ####
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x0C, //   ##
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1734 'u' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x41, // #     #
	0x62, //  #   ##
	0x5C, //   ### #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1751 'v' (8 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x81, // #      #
	0x42, //  #    #
	0x42, //  #    #
	0x24, //   #  #
	0x24, //   #  #
	0x24, //   #  #
	0x18, //    ##
	0x18, //    ##
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1768 'w' (12 pixels wide) */
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x61, 0x08, // #    ##    #
	0x62, 0x04, //  #   ##   #
	0x62, 0x04, //  #   ##   #
	0x92, 0x04, //  #  #  #  #
	0x94, 0x02, //   # #  # #
	0x94, 0x02, //   # #  # #
	0x08, 0x01, //    #    #
	0x08, 0x01, //    #    #
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //
	0x00, 0x00, //

	/* @1802 'x' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x41, // #     #
	0x22, //  #   #
	0x14, //   # #
	0x08, //    #
	0x08, //    #
	0x14, //   # #
	0x22, //  #   #
	0x41, // #     #
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1819 'y' (8 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x81, // #      #
	0x42, //  #    #
	0x42, //  #    #
	0x42, //  #    #
	0x24, //   #  #
	0x24, //   #  #
	0x18, //    ##
	0x18, //    ##
	0x08, //    #
	0x08, //    #
	0x04, //   #
	0x03, // ##

	/* @1836 'z' (7 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x7F, // #######
	0x20, //      #
	0x10, //     #
	0x08, //    #
	0x08, //    #
	0x04, //   #
	0x02, //  #
	0x7F, // #######
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	/* @1853 '{' (3 pixels wide) */
	0x00, //
	0x00, //
	0x04, //   #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x01, // #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x04, //   #
	0x00, //

	/* @1870 '|' (1 pixels wide) */
	0x00, //
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #
	0x01, // #

	/* @1887 '}' (3 pixels wide) */
	0x00, //
	0x00, //
	0x01, // #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x04, //   #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x02, //  #
	0x01, // #
	0x00, //

	/* @1904 '~' (8 pixels wide) */
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x8E, //  ###   #
	0x71, // #   ###
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
};


//Prevent Recursive Inclusion
#endif  /* __QAS_LCD_FONTS_SEGOEUI12PT_HPP_ */
