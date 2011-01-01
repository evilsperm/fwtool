/*
 * FwTool - Tool to encrypt/decrypt PS3 firmware's PKG files.
 *
 * Copyright (C) 2010  George Hotz (geohot)
 * Copyright (C) 2010  Miguel Boton (waninkoko)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __COREOS_H__
#define __COREOS_H__

#include "types.h"

/* CoreOS header */
struct coreHdr {
	u32 unk;
	u32 files;
	u64 size;

	u8 entries[];
};

/* CoreOS entry */
struct coreEntry {
	u64  offset;
	u64  filesize;
	char filename[32];
};

#endif	/* __COREOS_H__ */
