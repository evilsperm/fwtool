/*
 * FwPKG Tool - Tool to encrypt/decrypt PS3 firmware's PKG files.
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

#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include "types.h"

static inline u16 Swap16(u16 v)
{
	return (((v & 0x00FF) << 8) | (v & 0xFF00) >> 8);
}

static inline u32 Swap32(u32 v)
{
	return (((v & 0x000000FF) << 24) | ((v & 0x0000FF00) << 8) |
		((v & 0xFF000000) >> 24) | ((v & 0x00FF0000) >> 8));
}

static inline u64 Swap64(u64 v)
{
	return (((v & 0x00000000000000FFULL) << 56) |
		((v & 0x000000000000FF00ULL) << 40) |
		((v & 0x0000000000FF0000ULL) << 24) |
		((v & 0x00000000FF000000ULL) <<  8) |
		((v & 0x000000FF00000000ULL) >>  8) |
		((v & 0x0000FF0000000000ULL) >> 24) |
		((v & 0x00FF000000000000ULL) >> 40) |
		((v & 0xFF00000000000000ULL) >> 56));
}

#endif	/* __ENDIAN_H__ */
