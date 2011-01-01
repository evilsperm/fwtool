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
#include <errno.h>

#include "types.h"


s32 File_Read(const char *filename, u8 **buffer)
{
	FILE *fp  = NULL;
	u8   *buf = NULL;

	size_t  len;
	ssize_t ret = -1;

	/* Open file */
	fp = fopen(filename, "rb");
	if (!fp)
		return -errno;

	/* Get size */
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);

	/* Allocate buffer */
	buf = malloc(len);
	if (!buf)
		goto out;

	/* Read file */
	ret = fread(buf, 1, len, fp);

out:
	/* Close file */
	if (fp) fclose(fp);

	/* Output buffer */
	*buffer = (buf) ? buf : NULL;

	return (ret != len) ? -errno : ret;
}

s32 File_Write(const char *filename, const u8 *buffer, size_t len)
{
	FILE *fp = NULL;

	size_t  cnt;
	ssize_t ret;

	/* Open file */
	fp = fopen(filename, "wb");
	if (!fp)
		return -errno;

	/* Write loop */
	for (cnt = 0; cnt < len; cnt += ret) {
		/* Write to file */
		ret = fwrite(buffer + cnt, 1, len - cnt, fp);

		/* Write error */
		if (ret < 0)
			break;
	}

	/* Close file */
	fclose(fp);

	return (cnt != len) ? -errno : cnt;
}

