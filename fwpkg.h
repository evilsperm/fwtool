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

#ifndef __FWPKG_H__
#define __FWPKG_H__

#include "types.h"

/* PKG header */
struct pkgHdr {
	char magic[4];		/* "SCE\0" */

	u32 version;		/* 2 */
	u16 attribute;		/* 0x00 */
	u16 category;		/* 0x03 */

	u32 meta_offset;

	u64 file_offset;
	u64 file_size;
};

/* PKG metadata header */
struct pkgMHdr {
	u8 unk[32];

	u8  key[16];		/* Metadata key */
	u8 pad1[16];

	u8   iv[16];		/* Metadata IV */
	u8 pad2[16];

	u8 data[];
};

/* PKG metadata */
struct pkgMeta {
	u8  unk1[128];
	u64 data_offset;
	u64 data_size;

	u8  unk2[320];
	u8  pkg_key[16];	/* PKG key */
	u8  pkg_iv [16];	/* PKG IV */
};

#endif	/* __FWPKG_H__ */
