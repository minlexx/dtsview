/*
 * (C) Copyright David Gibson <dwg@au1.ibm.com>, IBM Corporation.  2005.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *                                                                   USA
 */

#include <sys/stat.h>

#include "dtc.h"
#include "srcpos.h"

/*
 * Command line options
 */
int quiet;		/* Level of quietness */
int reservenum;		/* Number of memory reservation slots */
int minsize;		/* Minimum blob size */
int padsize;		/* Additional padding to blob */
int alignsize;		/* Additional padding to blob accroding to the alignsize */
int phandle_format = PHANDLE_EPAPR;	/* Use linux,phandle or phandle properties */
int generate_symbols;	/* enable symbols & fixup support */
int generate_fixups;		/* suppress generation of fixups on symbol support */
int auto_label_aliases;		/* auto generate labels -> aliases */

/* Here was a source file of main() but cut */
