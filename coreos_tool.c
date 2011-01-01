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

#include "coreos.h"
#include "endian.h"
#include "types.h"
#include "utils.h"

/* Constants */
#define FILENAME_LEN	32
#define MEGABYTE	(1024.0 * 1024.0)


static void CoreOS_Pack(const char *outfile, const char **filelist, u32 files)
{
	struct coreHdr   *hdr   = NULL;
	struct coreEntry *entry = NULL;

	FILE *fp;
	u32   i, j;

	/* Create file */
	fp = fopen(outfile, "wb");
	if (!fp) {
		fprintf(stderr, "ERROR: Could not create file \"%s\"! (%d)\n", outfile, errno);
		return;
	}

	/* Allocate memory */
	hdr   = malloc(sizeof(*hdr));
	entry = malloc(sizeof(*entry) * files);

	/* Write empty header */
	fwrite(hdr, 1, sizeof(*hdr), fp);

	/* Write empty entries */
	fwrite(entry, 1, sizeof(*entry) * files, fp);

	/* Pack entries */
	for (i = 0; i < files; ++i) {
		const char *filename = filelist[i];
		u64         offset   = ftell(fp);

		u8  *buffer = NULL;
		u32  buflen;

		/* Read file */
		buflen = File_Read(filename, &buffer);
		if (buflen <= 0) {
			fprintf(stderr, "ERROR: Could not pack file \"%s\"! (%d)\n", filename, buflen);
			goto out;
		}

		/* File info */
		printf("File: %-32s (offset: 0x%08llX, size: %.2f MB)\n", filename, offset, buflen / MEGABYTE);

		/* Write entry */
		fwrite(buffer, 1, buflen, fp);

		/* Write padding */
		for (j = 0; j < (buflen % 4); ++j)
			fputc(0, fp);

		/* Free buffer */
		free(buffer);

		/* Fill entry */
		entry[i].offset   = Swap64(offset);
		entry[i].filesize = Swap64(buflen);

		strncpy(entry[i].filename, filename, FILENAME_LEN);
	}

	/* Fill header */
	hdr->unk   = Swap32(1);
	hdr->files = Swap32(files);
	hdr->size  = Swap64(ftell(fp));

	/* Write header */
	fseek(fp, 0, SEEK_SET);
	fwrite(hdr, 1, sizeof(*hdr), fp);

	/* Write entries */
	fwrite(entry, 1, sizeof(*entry) * files, fp);

out:
	/* Close file */
	if (fp) fclose(fp);

	/* Free buffers */
	if (hdr)   free(hdr);
	if (entry) free(entry);
}

static void CoreOS_Unpack(u8 *buffer)
{
	struct coreHdr   *hdr   = (struct coreHdr   *)buffer;
	struct coreEntry *entry = (struct coreEntry *)hdr->entries;

	u32 i;
	s32 ret;

	/* Unpack entries */
	for (i = 0; i < Swap32(hdr->files); i++, entry++) {
		char *filename = entry->filename;
		u64   filesize = Swap64(entry->filesize);
		u64   offset   = Swap64(entry->offset);

		/* Entry info */
		printf("File: %-32s (offset: 0x%08llX, size: %.2f MB)\n", filename, offset, filesize / MEGABYTE);

		/* Write file */
		ret = File_Write(filename, buffer + offset, filesize);
		if (ret <= 0)
			fprintf(stderr, "ERROR: Could not unpack file \"%s\"! (%d)\n", filename, ret);
	}
}


int main(int argc, const char **argv)
{
	char mode;

	/* Check arguments */
	if (argc < 3) {
		fprintf(stderr, "Pack CoreOS  : %s p <output pkg> <files...>\n", argv[0]);
		fprintf(stderr, "Unpack CoreOS: %s u <decrypted CORE_OS_PACKAGE.pkg>\n", argv[0]);

		return 0;
	}

	/* Read arguments */
	mode = argv[1][0];

	/* Check mode */
	switch (mode) {
	case 'p':
	case 'P': {
		const char **filelist = argv + 3;
		const char  *filename = argv[2];

		u32 files = (argc - 3);

		/* No files */
		if (!files) {
			fprintf(stderr, "ERROR: No files have been specified!\n");
			break;
		}

		/* Pack CoreOS */
		CoreOS_Pack(filename, filelist, files);

		break;
	}

	case 'u':
	case 'U': {
		const char *filename;

		u8 *buffer = NULL;
		u32 buflen;

		/* Read arguments */
		filename = argv[2];

		/* Read file */
		buflen = File_Read(filename, &buffer);
		if (buflen <= 0) {
			fprintf(stderr, "ERROR: Could not read file \"%s\"! (%d)\n", filename, buflen);
			break;
		}

		/* Unpack CoreOS */
		CoreOS_Unpack(buffer);

		/* Free buffer */
		free(buffer);

		break;
	}

	default:
		fprintf(stderr, "ERROR: Invalid mode specified! (%c)\n", mode);
	}

	return 0;
}
