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

#include <string.h>
#include <openssl/aes.h>

#include "types.h"


void AES_CTR128_Encrypt(void *src, void *dst, u32 len, u8 *key, u8 *iv)
{
	AES_KEY aesKey;

	u8  ecount[16];
	u32 num;

	/* Clear variables */
	memset(ecount, 0, sizeof(ecount));
	num = 0;

	/* Set AES key */
	AES_set_encrypt_key(key, 128, &aesKey);

	/* Encrypt data */
	AES_ctr128_encrypt((u8 *)src, (u8 *)dst, len, &aesKey, iv, ecount, &num);
}

void AES_CBC256_Encrypt(void *src, void *dst, u32 len, u8 *key, u8 *iv)
{
	AES_KEY aesKey;

	/* Set AES key */
	AES_set_encrypt_key(key, 256, &aesKey);

	/* Encrypt data */
	AES_cbc_encrypt((u8 *)src, (u8 *)dst, len, &aesKey, iv, AES_ENCRYPT);
}

void AES_CBC256_Decrypt(void *src, void *dst, u32 len, u8 *key, u8 *iv)
{
	AES_KEY aesKey;

	/* Set AES key */
	AES_set_decrypt_key(key, 256, &aesKey);

	/* Decrypt data */
	AES_cbc_encrypt((u8 *)src, (u8 *)dst, len, &aesKey, iv, AES_DECRYPT);
}
