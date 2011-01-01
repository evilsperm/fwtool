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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#include "aes.h"
#include "endian.h"
#include "fwpkg.h"
#include "types.h"
#include "utils.h"
#include "zlib.h"

/* Constants */
#define BUF_SIZE	0x1000000
#define MEGABYTE	(1024.0 * 1024.0)

/* PKG AES key/IV */
static u8 pkg_riv[] = {
	0x4A, 0xCE, 0xF0, 0x12, 0x24, 0xFB, 0xEE, 0xDF,
	0x82, 0x45, 0xF8, 0xFF, 0x10, 0x21, 0x1E, 0x6E
};

static u8 pkg_erk[] = {
	0xA9, 0x78, 0x18, 0xBD, 0x19, 0x3A, 0x67, 0xA1,
	0x6F, 0xE8, 0x3A, 0x85, 0x5E, 0x1B, 0xE9, 0xFB,
	0x56, 0x40, 0x93, 0x8D, 0x4D, 0xBC, 0xB2, 0xCB,
	0x52, 0xC5, 0xA2, 0xF8, 0xB0, 0x2B, 0x10, 0x31
};


static void Hex_Dump(u8 *data, u32 len)
{
	u32 i;

	/* Print hex values */
	for (i = 0; i < len; ++i)
		printf("%02X ", data[i]);

	printf("\n");
}

static void Fill_Random(u8 *buffer, u32 len)
{
	u32 i;

	/* Fill buffer */
	for (i = 0; i < len; ++i)
		buffer[i] = rand() % (UCHAR_MAX + 1);
}

static s32 Pkg_Encrypt(u8 *inbuf, u32 inlen, u8 *outbuf, u32 outlen, u8 *key, u8 *iv, u8 *metaKey, u8 *metaIV, u8 deflate)
{
	struct pkgHdr  *header = (struct pkgHdr  *)outbuf;
	struct pkgMHdr *mhdr   = (struct pkgMHdr *)outbuf;
	struct pkgMeta *meta   = (struct pkgMeta *)mhdr->data;

	u64 dataOffset, dataSize, metaSize, pkgSize;

	printf("Metadata Key:\t");
	Hex_Dump(metaKey, 16);
	printf("Metadata IV:\t");
	Hex_Dump(metaIV, 16);

	printf("\n");

	printf("PKG Key:\t");
	Hex_Dump(key, 16);
	printf("PKG IV:\t\t");
	Hex_Dump(iv, 16);

	/* Data offset */
	dataOffset = 0x300;

	/* Copy PKG data */
	if (!deflate) {
		memcpy(outbuf + dataOffset, inbuf, inlen);
		outlen = inlen;
	} else
		Zlib_Deflate(inbuf, inlen, outbuf + dataOffset, &outlen);

	/* Data/PKG size */
	dataSize = outlen;
	pkgSize  = (dataOffset + dataSize);

	/* Fill PKG header */
	header->version     = Swap32(2);
	header->attribute   = Swap16(0);
	header->category    = Swap16(3);
	header->meta_offset = Swap32(0);
	header->file_offset = Swap64(0x280);

	strcpy(header->magic, "SCE");

	/* Fill PKG metadata */
	meta->data_offset = Swap64(dataOffset);
	meta->data_size   = Swap64(dataSize);

	memcpy(meta->pkg_key, key, 16);
	memcpy(meta->pkg_iv,  iv,  16);

	memcpy(mhdr->key, metaKey, 16);
	memcpy(mhdr->iv,  metaIV,  16);

	/* Metadata size */
	metaSize = Swap64(header->file_offset) - (Swap32(header->meta_offset) + sizeof(*mhdr));

	/* Encrypt PKG data */
	AES_CTR128_Encrypt(outbuf + dataOffset, outbuf + dataOffset, dataSize, key, iv);

	/* Encrypt PKG metadata */
	AES_CTR128_Encrypt(meta, meta, metaSize, metaKey, metaIV);

	/* Encrypt metadata header */
	AES_CBC256_Encrypt(mhdr->key, mhdr->key, 64, pkg_erk, pkg_riv);

	printf("PKG Size:\t%llu bytes (%.2f MB)\n", pkgSize, pkgSize / MEGABYTE);

	/* Return size */
	return pkgSize;
}

static s32 Pkg_Decrypt(u8 *inbuf, u32 inlen, u8 *outbuf, u32 outlen)
{
	struct pkgHdr  *header = (struct pkgHdr  *)(inbuf);
	struct pkgMHdr *mhdr   = (struct pkgMHdr *)(inbuf + Swap32(header->meta_offset));
	struct pkgMeta *meta   = (struct pkgMeta *)(mhdr->data);

	u64 dataOffset, dataSize, metaSize;
	s32 ret;

	/* Decrypt metadata header */
	AES_CBC256_Decrypt(mhdr->key, mhdr->key, 64, pkg_erk, pkg_riv);

	/* Metadata size */
	metaSize = Swap64(header->file_offset) - (Swap32(header->meta_offset) + sizeof(*mhdr));

	printf("Metadata Key:\t");
	Hex_Dump(mhdr->key, 16);
	printf("Metadata IV:\t");
	Hex_Dump(mhdr->iv, 16);
	printf("Metadata Size:\t%llu bytes\n", metaSize);
	printf("\n");

	/* Decrypt PKG metadata */
	AES_CTR128_Encrypt(meta, meta, metaSize, mhdr->key, mhdr->iv);

	/* Data offset/size */
	dataOffset = Swap64(meta->data_offset);
	dataSize   = Swap64(meta->data_size);

	printf("Data Start:\t0x%08llX\n", dataOffset);
	printf("Data Size:\t%llu bytes (%.2f MB)\n", dataSize, dataSize / MEGABYTE);
	printf("\n");

	printf("PKG Key:\t");
	Hex_Dump(meta->pkg_key, 16);
	printf("PKG IV:\t\t");
	Hex_Dump(meta->pkg_iv, 16);

	/* Decrypt PKG data */
	AES_CTR128_Encrypt(inbuf + dataOffset, inbuf + dataOffset, dataSize, meta->pkg_key, meta->pkg_iv);

	/* Inflate data */
	ret = Zlib_Inflate(inbuf + dataOffset, dataSize, outbuf, &outlen);

	/* Not compressed */
	if (ret) {
		memcpy(outbuf, inbuf + dataOffset, dataSize);
		outlen = dataSize;
	}

	printf("PKG Size:\t%d bytes (%.2f MB)\n", outlen, outlen / MEGABYTE); 

	/* Return length */
	return outlen;
}


int main(int argc, const char *argv[])
{
	char mode;
	const char *infile, *outfile;

	u8 *inbuf, *outbuf;
	s32 inlen,  outlen;

	/* Check arguments */
	if (argc < 4) {
		fprintf(stderr, "USAGE: %s <mode> <input file> <output file>\n\n", argv[0]);

		fprintf(stderr, ">> Mode can be the following values:\n");
		fprintf(stderr, "   - e: Encrypt PKG\n");
		fprintf(stderr, "   - d: Decrypt PKG\n\n");

		return 0;
	}

	/* Read arguments */
	mode    = argv[1][0];
	infile  = argv[2];
	outfile = argv[3];

	/* Random seed */
	srand(time(NULL));

	/* Read input file */
	inlen = File_Read(infile, &inbuf);
	if (inlen <= 0) {
		fprintf(stderr, "ERROR: Could not read file \"%s\"! (%d)\n", infile, inlen);
		goto out;
	}

	/* Allocate buffer */
	outbuf = malloc(BUF_SIZE);
	if (!outbuf) {
		fprintf(stderr, "ERROR: No enough memory available!\n");
		goto out;
	}

	/* Clean buffer */
	memset(outbuf, 0, BUF_SIZE);

	switch (mode) {
	case 'd':
	case 'D': {
		/* Decrypt PKG */
		outlen = Pkg_Decrypt(inbuf, inlen, outbuf, BUF_SIZE);

		break;
	}

	case 'e':
	case 'E': {
		u8 pkgKey[16],  pkgIV[16];
		u8 metaKey[16], metaIV[16];

		/* Generate random keys */
		Fill_Random(pkgKey,  16);
		Fill_Random(pkgIV,   16);
		Fill_Random(metaKey, 16);
		Fill_Random(metaIV,  16);

		/* Encrypt PKG */
		outlen = Pkg_Encrypt(inbuf, inlen, outbuf, BUF_SIZE, pkgKey, pkgIV, metaKey, metaIV, 1);

		break;
	}

	default:
		fprintf(stderr, "ERROR: Invalid mode specified! (%c)\n", mode);
		goto out;
	}

	/* Action failed */
	if (outlen <= 0) {
		fprintf(stderr, "ERROR: Package encryption/decryption failed!\n");
		goto out;
	}

	/* Write decrypted PKG */
	outlen = File_Write(outfile, outbuf, outlen);
	if (outlen < 0)
		fprintf(stderr, "ERROR: Could not write file \"%s\"! (%d)\n", outfile, outlen);

out:
	/* Free buffesr */
	if (inbuf)  free(inbuf);
	if (outbuf) free(outbuf);

	return 0;
}

