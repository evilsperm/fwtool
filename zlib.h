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

#ifndef __ZLIB_H__
#define __ZLIB_H__

#include "types.h"

/* Prototypes */
s32 Zlib_Inflate(u8 *source, u32 inlen, u8 *dest, u32 *outlen);
s32 Zlib_Deflate(u8 *source, u32 inlen, u8 *dest, u32 *outlen);

#endif	/* __ZLIB_H__ */
