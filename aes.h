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

#ifndef __AES_H__
#define __AES_H__

#include "types.h"

/* Prototypes */
void AES_CTR128_Encrypt(void *src, void *dst, u32 len, u8 *key, u8 *iv);
void AES_CBC256_Encrypt(void *src, void *dst, u32 len, u8 *key, u8 *iv);
void AES_CBC256_Decrypt(void *src, void *dst, u32 len, u8 *key, u8 *iv);

#endif	/* __AES_H__ */
