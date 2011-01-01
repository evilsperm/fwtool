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

#include <errno.h>
#include <zlib.h>

#include "types.h"


s32 Zlib_Inflate(u8 *source, u32 inlen, u8 *dest, u32 *outlen)
{
	z_stream strm;
	s32      ret;

	/* Setup stream */
	strm.zalloc    = Z_NULL;
	strm.zfree     = Z_NULL;
	strm.opaque    = Z_NULL;
	strm.avail_in  = inlen;
	strm.next_in   = source;
	strm.avail_out = *outlen;
	strm.next_out  = dest;

	/* Initialize inflate */
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* Inflate data */
	ret = inflate(&strm, Z_NO_FLUSH);

	/* Output size */
	*outlen -= strm.avail_out;

	/* End inflate */
	inflateEnd(&strm);

	return (ret < 0) ? -EINVAL : 0;
}

s32 Zlib_Deflate(u8 *source, u32 inlen, u8 *dest, u32 *outlen)
{
	z_stream strm;
	s32      ret;

	/* Setup stream */
	strm.zalloc    = Z_NULL;
	strm.zfree     = Z_NULL;
	strm.opaque    = Z_NULL;
	strm.avail_in  = inlen;
	strm.next_in   = source;
	strm.avail_out = *outlen;
	strm.next_out  = dest;

	/* Initialize deflate */
	ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if (ret != Z_OK)
		return ret;

	/* Deflate data */
	ret = deflate(&strm, Z_NO_FLUSH);

	/* Output size */
	*outlen -= strm.avail_out;

	/* End deflate */
	deflateEnd(&strm);

	return (ret < 0) ? -EINVAL : 0;
}
