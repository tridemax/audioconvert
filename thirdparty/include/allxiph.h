/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2002             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: #ifdef jail to whip a few platforms into the UNIX ideal.
 last mod: $Id: os_types.h 19098 2014-02-26 19:06:45Z giles $

 ********************************************************************/
#ifndef _OS_TYPES_H
#define _OS_TYPES_H

/* make it easy on the folks that want to compile the libs with a
   different malloc than stdlib */
#define _ogg_malloc  malloc
#define _ogg_calloc  calloc
#define _ogg_realloc realloc
#define _ogg_free    free

#if defined(_WIN32)

#  if defined(__CYGWIN__)
#    include <stdint.h>
     typedef int16_t ogg_int16_t;
     typedef uint16_t ogg_uint16_t;
     typedef int32_t ogg_int32_t;
     typedef uint32_t ogg_uint32_t;
     typedef int64_t ogg_int64_t;
     typedef uint64_t ogg_uint64_t;
#  elif defined(__MINGW32__)
#    include <sys/types.h>
     typedef short ogg_int16_t;
     typedef unsigned short ogg_uint16_t;
     typedef int ogg_int32_t;
     typedef unsigned int ogg_uint32_t;
     typedef long long ogg_int64_t;
     typedef unsigned long long ogg_uint64_t;
#  elif defined(__MWERKS__)
     typedef long long ogg_int64_t;
     typedef int ogg_int32_t;
     typedef unsigned int ogg_uint32_t;
     typedef short ogg_int16_t;
     typedef unsigned short ogg_uint16_t;
#  else
     /* MSVC/Borland */
     typedef __int64 ogg_int64_t;
     typedef __int32 ogg_int32_t;
     typedef unsigned __int32 ogg_uint32_t;
     typedef __int16 ogg_int16_t;
     typedef unsigned __int16 ogg_uint16_t;
#  endif

#elif defined(__MACOS__)

#  include <sys/types.h>
   typedef SInt16 ogg_int16_t;
   typedef UInt16 ogg_uint16_t;
   typedef SInt32 ogg_int32_t;
   typedef UInt32 ogg_uint32_t;
   typedef SInt64 ogg_int64_t;

#elif (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */

#  include <inttypes.h>
   typedef int16_t ogg_int16_t;
   typedef uint16_t ogg_uint16_t;
   typedef int32_t ogg_int32_t;
   typedef uint32_t ogg_uint32_t;
   typedef int64_t ogg_int64_t;

#elif defined(__HAIKU__)

  /* Haiku */
#  include <sys/types.h>
   typedef short ogg_int16_t;
   typedef unsigned short ogg_uint16_t;
   typedef int ogg_int32_t;
   typedef unsigned int ogg_uint32_t;
   typedef long long ogg_int64_t;

#elif defined(__BEOS__)

   /* Be */
#  include <inttypes.h>
   typedef int16_t ogg_int16_t;
   typedef uint16_t ogg_uint16_t;
   typedef int32_t ogg_int32_t;
   typedef uint32_t ogg_uint32_t;
   typedef int64_t ogg_int64_t;

#elif defined (__EMX__)

   /* OS/2 GCC */
   typedef short ogg_int16_t;
   typedef unsigned short ogg_uint16_t;
   typedef int ogg_int32_t;
   typedef unsigned int ogg_uint32_t;
   typedef long long ogg_int64_t;

#elif defined (DJGPP)

   /* DJGPP */
   typedef short ogg_int16_t;
   typedef int ogg_int32_t;
   typedef unsigned int ogg_uint32_t;
   typedef long long ogg_int64_t;

#elif defined(R5900)

   /* PS2 EE */
   typedef long ogg_int64_t;
   typedef int ogg_int32_t;
   typedef unsigned ogg_uint32_t;
   typedef short ogg_int16_t;

#elif defined(__SYMBIAN32__)

   /* Symbian GCC */
   typedef signed short ogg_int16_t;
   typedef unsigned short ogg_uint16_t;
   typedef signed int ogg_int32_t;
   typedef unsigned int ogg_uint32_t;
   typedef long long int ogg_int64_t;

#elif defined(__TMS320C6X__)

   /* TI C64x compiler */
   typedef signed short ogg_int16_t;
   typedef unsigned short ogg_uint16_t;
   typedef signed int ogg_int32_t;
   typedef unsigned int ogg_uint32_t;
   typedef long long int ogg_int64_t;

#else

#  include <ogg/config_types.h>

#endif

#endif  /* _OS_TYPES_H */
/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: toplevel libogg include
 last mod: $Id: ogg.h 18044 2011-08-01 17:55:20Z gmaxwell $

 ********************************************************************/
#ifndef _OGG_H
#define _OGG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct {
  void *iov_base;
  size_t iov_len;
} ogg_iovec_t;

typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;

/* ogg_page is used to encapsulate the data in one Ogg bitstream page *****/

typedef struct {
  unsigned char *header;
  long header_len;
  unsigned char *body;
  long body_len;
} ogg_page;

/* ogg_stream_state contains the current encode/decode state of a logical
   Ogg bitstream **********************************************************/

typedef struct {
  unsigned char   *body_data;    /* bytes from packet bodies */
  long    body_storage;          /* storage elements allocated */
  long    body_fill;             /* elements stored; fill mark */
  long    body_returned;         /* elements of fill returned */


  int     *lacing_vals;      /* The values that will go to the segment table */
  ogg_int64_t *granule_vals; /* granulepos values for headers. Not compact
                                this way, but it is simple coupled to the
                                lacing fifo */
  long    lacing_storage;
  long    lacing_fill;
  long    lacing_packet;
  long    lacing_returned;

  unsigned char    header[282];      /* working space for header encode */
  int              header_fill;

  int     e_o_s;          /* set when we have buffered the last packet in the
                             logical bitstream */
  int     b_o_s;          /* set after we've written the initial page
                             of a logical bitstream */
  long    serialno;
  long    pageno;
  ogg_int64_t  packetno;  /* sequence number for decode; the framing
                             knows where there's a hole in the data,
                             but we need coupling so that the codec
                             (which is in a separate abstraction
                             layer) also knows about the gap */
  ogg_int64_t   granulepos;

} ogg_stream_state;

/* ogg_packet is used to encapsulate the data and metadata belonging
   to a single raw Ogg/Vorbis packet *************************************/

typedef struct {
  unsigned char *packet;
  long  bytes;
  long  b_o_s;
  long  e_o_s;

  ogg_int64_t  granulepos;

  ogg_int64_t  packetno;     /* sequence number for decode; the framing
                                knows where there's a hole in the data,
                                but we need coupling so that the codec
                                (which is in a separate abstraction
                                layer) also knows about the gap */
} ogg_packet;

typedef struct {
  unsigned char *data;
  int storage;
  int fill;
  int returned;

  int unsynced;
  int headerbytes;
  int bodybytes;
} ogg_sync_state;

/* Ogg BITSTREAM PRIMITIVES: bitstream ************************/

extern void  oggpack_writeinit(oggpack_buffer *b);
extern int   oggpack_writecheck(oggpack_buffer *b);
extern void  oggpack_writetrunc(oggpack_buffer *b,long bits);
extern void  oggpack_writealign(oggpack_buffer *b);
extern void  oggpack_writecopy(oggpack_buffer *b,void *source,long bits);
extern void  oggpack_reset(oggpack_buffer *b);
extern void  oggpack_writeclear(oggpack_buffer *b);
extern void  oggpack_readinit(oggpack_buffer *b,unsigned char *buf,int bytes);
extern void  oggpack_write(oggpack_buffer *b,unsigned long value,int bits);
extern long  oggpack_look(oggpack_buffer *b,int bits);
extern long  oggpack_look1(oggpack_buffer *b);
extern void  oggpack_adv(oggpack_buffer *b,int bits);
extern void  oggpack_adv1(oggpack_buffer *b);
extern long  oggpack_read(oggpack_buffer *b,int bits);
extern long  oggpack_read1(oggpack_buffer *b);
extern long  oggpack_bytes(oggpack_buffer *b);
extern long  oggpack_bits(oggpack_buffer *b);
extern unsigned char *oggpack_get_buffer(oggpack_buffer *b);

extern void  oggpackB_writeinit(oggpack_buffer *b);
extern int   oggpackB_writecheck(oggpack_buffer *b);
extern void  oggpackB_writetrunc(oggpack_buffer *b,long bits);
extern void  oggpackB_writealign(oggpack_buffer *b);
extern void  oggpackB_writecopy(oggpack_buffer *b,void *source,long bits);
extern void  oggpackB_reset(oggpack_buffer *b);
extern void  oggpackB_writeclear(oggpack_buffer *b);
extern void  oggpackB_readinit(oggpack_buffer *b,unsigned char *buf,int bytes);
extern void  oggpackB_write(oggpack_buffer *b,unsigned long value,int bits);
extern long  oggpackB_look(oggpack_buffer *b,int bits);
extern long  oggpackB_look1(oggpack_buffer *b);
extern void  oggpackB_adv(oggpack_buffer *b,int bits);
extern void  oggpackB_adv1(oggpack_buffer *b);
extern long  oggpackB_read(oggpack_buffer *b,int bits);
extern long  oggpackB_read1(oggpack_buffer *b);
extern long  oggpackB_bytes(oggpack_buffer *b);
extern long  oggpackB_bits(oggpack_buffer *b);
extern unsigned char *oggpackB_get_buffer(oggpack_buffer *b);

/* Ogg BITSTREAM PRIMITIVES: encoding **************************/

extern int      ogg_stream_packetin(ogg_stream_state *os, ogg_packet *op);
extern int      ogg_stream_iovecin(ogg_stream_state *os, ogg_iovec_t *iov,
                                   int count, long e_o_s, ogg_int64_t granulepos);
extern int      ogg_stream_pageout(ogg_stream_state *os, ogg_page *og);
extern int      ogg_stream_pageout_fill(ogg_stream_state *os, ogg_page *og, int nfill);
extern int      ogg_stream_flush(ogg_stream_state *os, ogg_page *og);
extern int      ogg_stream_flush_fill(ogg_stream_state *os, ogg_page *og, int nfill);

/* Ogg BITSTREAM PRIMITIVES: decoding **************************/

extern int      ogg_sync_init(ogg_sync_state *oy);
extern int      ogg_sync_clear(ogg_sync_state *oy);
extern int      ogg_sync_reset(ogg_sync_state *oy);
extern int      ogg_sync_destroy(ogg_sync_state *oy);
extern int      ogg_sync_check(ogg_sync_state *oy);

extern char    *ogg_sync_buffer(ogg_sync_state *oy, long size);
extern int      ogg_sync_wrote(ogg_sync_state *oy, long bytes);
extern long     ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og);
extern int      ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og);
extern int      ogg_stream_pagein(ogg_stream_state *os, ogg_page *og);
extern int      ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op);
extern int      ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op);

/* Ogg BITSTREAM PRIMITIVES: general ***************************/

extern int      ogg_stream_init(ogg_stream_state *os,int serialno);
extern int      ogg_stream_clear(ogg_stream_state *os);
extern int      ogg_stream_reset(ogg_stream_state *os);
extern int      ogg_stream_reset_serialno(ogg_stream_state *os,int serialno);
extern int      ogg_stream_destroy(ogg_stream_state *os);
extern int      ogg_stream_check(ogg_stream_state *os);
extern int      ogg_stream_eos(ogg_stream_state *os);

extern void     ogg_page_checksum_set(ogg_page *og);

extern int      ogg_page_version(const ogg_page *og);
extern int      ogg_page_continued(const ogg_page *og);
extern int      ogg_page_bos(const ogg_page *og);
extern int      ogg_page_eos(const ogg_page *og);
extern ogg_int64_t  ogg_page_granulepos(const ogg_page *og);
extern int      ogg_page_serialno(const ogg_page *og);
extern long     ogg_page_pageno(const ogg_page *og);
extern int      ogg_page_packets(const ogg_page *og);

extern void     ogg_packet_clear(ogg_packet *op);


#ifdef __cplusplus
}
#endif

#endif  /* _OGG_H */
/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000-2009  Josh Coalson
 * Copyright (C) 2011-2014  Xiph.Org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__EXPORT_H
#define FLAC__EXPORT_H

/** \file include/FLAC/export.h
 *
 *  \brief
 *  This module contains #defines and symbols for exporting function
 *  calls, and providing version information and compiled-in features.
 *
 *  See the \link flac_export export \endlink module.
 */

/** \defgroup flac_export FLAC/export.h: export symbols
 *  \ingroup flac
 *
 *  \brief
 *  This module contains #defines and symbols for exporting function
 *  calls, and providing version information and compiled-in features.
 *
 *  If you are compiling with MSVC and will link to the static library
 *  (libFLAC.lib) you should define FLAC__NO_DLL in your project to
 *  make sure the symbols are exported properly.
 *
 * \{
 */

#if defined(FLAC__NO_DLL)
#define FLAC_API

#elif defined(_MSC_VER)
#ifdef FLAC_API_EXPORTS
#define	FLAC_API __declspec(dllexport)
#else
#define FLAC_API __declspec(dllimport)
#endif

#elif defined(FLAC__USE_VISIBILITY_ATTR)
#define FLAC_API __attribute__ ((visibility ("default")))

#else
#define FLAC_API

#endif

/** These #defines will mirror the libtool-based library version number, see
 * http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
 */
#define FLAC_API_VERSION_CURRENT 11
#define FLAC_API_VERSION_REVISION 0 /**< see above */
#define FLAC_API_VERSION_AGE 3 /**< see above */

#ifdef __cplusplus
extern "C" {
#endif

/** \c 1 if the library has been compiled with support for Ogg FLAC, else \c 0. */
extern FLAC_API int FLAC_API_SUPPORTS_OGG_FLAC;

#ifdef __cplusplus
}
#endif

/* \} */

#endif
/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000-2009  Josh Coalson
 * Copyright (C) 2011-2014  Xiph.Org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__ORDINALS_H
#define FLAC__ORDINALS_H

#if defined(_MSC_VER) && _MSC_VER < 1600

/* Microsoft Visual Studio earlier than the 2010 version did not provide
 * the 1999 ISO C Standard header file <stdint.h>.
 */

typedef __int8 FLAC__int8;
typedef unsigned __int8 FLAC__uint8;

typedef __int16 FLAC__int16;
typedef __int32 FLAC__int32;
typedef __int64 FLAC__int64;
typedef unsigned __int16 FLAC__uint16;
typedef unsigned __int32 FLAC__uint32;
typedef unsigned __int64 FLAC__uint64;

#else

/* For MSVC 2010 and everything else which provides <stdint.h>. */

#include <stdint.h>

typedef int8_t FLAC__int8;
typedef uint8_t FLAC__uint8;

typedef int16_t FLAC__int16;
typedef int32_t FLAC__int32;
typedef int64_t FLAC__int64;
typedef uint16_t FLAC__uint16;
typedef uint32_t FLAC__uint32;
typedef uint64_t FLAC__uint64;

#endif

typedef int FLAC__bool;

typedef FLAC__uint8 FLAC__byte;


#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif
#ifndef __cplusplus
#define true 1
#define false 0
#endif

#endif
/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000-2009  Josh Coalson
 * Copyright (C) 2011-2014  Xiph.Org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__FORMAT_H
#define FLAC__FORMAT_H


#ifdef __cplusplus
extern "C" {
#endif

/** \file include/FLAC/format.h
 *
 *  \brief
 *  This module contains structure definitions for the representation
 *  of FLAC format components in memory.  These are the basic
 *  structures used by the rest of the interfaces.
 *
 *  See the detailed documentation in the
 *  \link flac_format format \endlink module.
 */

/** \defgroup flac_format FLAC/format.h: format components
 *  \ingroup flac
 *
 *  \brief
 *  This module contains structure definitions for the representation
 *  of FLAC format components in memory.  These are the basic
 *  structures used by the rest of the interfaces.
 *
 *  First, you should be familiar with the
 *  <A HREF="../format.html">FLAC format</A>.  Many of the values here
 *  follow directly from the specification.  As a user of libFLAC, the
 *  interesting parts really are the structures that describe the frame
 *  header and metadata blocks.
 *
 *  The format structures here are very primitive, designed to store
 *  information in an efficient way.  Reading information from the
 *  structures is easy but creating or modifying them directly is
 *  more complex.  For the most part, as a user of a library, editing
 *  is not necessary; however, for metadata blocks it is, so there are
 *  convenience functions provided in the \link flac_metadata metadata
 *  module \endlink to simplify the manipulation of metadata blocks.
 *
 * \note
 * It's not the best convention, but symbols ending in _LEN are in bits
 * and _LENGTH are in bytes.  _LENGTH symbols are \#defines instead of
 * global variables because they are usually used when declaring byte
 * arrays and some compilers require compile-time knowledge of array
 * sizes when declared on the stack.
 *
 * \{
 */


/*
	Most of the values described in this file are defined by the FLAC
	format specification.  There is nothing to tune here.
*/

/** The largest legal metadata type code. */
#define FLAC__MAX_METADATA_TYPE_CODE (126u)

/** The minimum block size, in samples, permitted by the format. */
#define FLAC__MIN_BLOCK_SIZE (16u)

/** The maximum block size, in samples, permitted by the format. */
#define FLAC__MAX_BLOCK_SIZE (65535u)

/** The maximum block size, in samples, permitted by the FLAC subset for
 *  sample rates up to 48kHz. */
#define FLAC__SUBSET_MAX_BLOCK_SIZE_48000HZ (4608u)

/** The maximum number of channels permitted by the format. */
#define FLAC__MAX_CHANNELS (8u)

/** The minimum sample resolution permitted by the format. */
#define FLAC__MIN_BITS_PER_SAMPLE (4u)

/** The maximum sample resolution permitted by the format. */
#define FLAC__MAX_BITS_PER_SAMPLE (32u)

/** The maximum sample resolution permitted by libFLAC.
 *
 * \warning
 * FLAC__MAX_BITS_PER_SAMPLE is the limit of the FLAC format.  However,
 * the reference encoder/decoder is currently limited to 24 bits because
 * of prevalent 32-bit math, so make sure and use this value when
 * appropriate.
 */
#define FLAC__REFERENCE_CODEC_MAX_BITS_PER_SAMPLE (24u)

/** The maximum sample rate permitted by the format.  The value is
 *  ((2 ^ 16) - 1) * 10; see <A HREF="../format.html">FLAC format</A>
 *  as to why.
 */
#define FLAC__MAX_SAMPLE_RATE (655350u)

/** The maximum LPC order permitted by the format. */
#define FLAC__MAX_LPC_ORDER (32u)

/** The maximum LPC order permitted by the FLAC subset for sample rates
 *  up to 48kHz. */
#define FLAC__SUBSET_MAX_LPC_ORDER_48000HZ (12u)

/** The minimum quantized linear predictor coefficient precision
 *  permitted by the format.
 */
#define FLAC__MIN_QLP_COEFF_PRECISION (5u)

/** The maximum quantized linear predictor coefficient precision
 *  permitted by the format.
 */
#define FLAC__MAX_QLP_COEFF_PRECISION (15u)

/** The maximum order of the fixed predictors permitted by the format. */
#define FLAC__MAX_FIXED_ORDER (4u)

/** The maximum Rice partition order permitted by the format. */
#define FLAC__MAX_RICE_PARTITION_ORDER (15u)

/** The maximum Rice partition order permitted by the FLAC Subset. */
#define FLAC__SUBSET_MAX_RICE_PARTITION_ORDER (8u)

/** The version string of the release, stamped onto the libraries and binaries.
 *
 * \note
 * This does not correspond to the shared library version number, which
 * is used to determine binary compatibility.
 */
extern FLAC_API const char *FLAC__VERSION_STRING;

/** The vendor string inserted by the encoder into the VORBIS_COMMENT block.
 *  This is a NUL-terminated ASCII string; when inserted into the
 *  VORBIS_COMMENT the trailing null is stripped.
 */
extern FLAC_API const char *FLAC__VENDOR_STRING;

/** The byte string representation of the beginning of a FLAC stream. */
extern FLAC_API const FLAC__byte FLAC__STREAM_SYNC_STRING[4]; /* = "fLaC" */

/** The 32-bit integer big-endian representation of the beginning of
 *  a FLAC stream.
 */
extern FLAC_API const unsigned FLAC__STREAM_SYNC; /* = 0x664C6143 */

/** The length of the FLAC signature in bits. */
extern FLAC_API const unsigned FLAC__STREAM_SYNC_LEN; /* = 32 bits */

/** The length of the FLAC signature in bytes. */
#define FLAC__STREAM_SYNC_LENGTH (4u)


/*****************************************************************************
 *
 * Subframe structures
 *
 *****************************************************************************/

/*****************************************************************************/

/** An enumeration of the available entropy coding methods. */
typedef enum {
	FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE = 0,
	/**< Residual is coded by partitioning into contexts, each with it's own
	 * 4-bit Rice parameter. */

	FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE2 = 1
	/**< Residual is coded by partitioning into contexts, each with it's own
	 * 5-bit Rice parameter. */
} FLAC__EntropyCodingMethodType;

/** Maps a FLAC__EntropyCodingMethodType to a C string.
 *
 *  Using a FLAC__EntropyCodingMethodType as the index to this array will
 *  give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__EntropyCodingMethodTypeString[];


/** Contents of a Rice partitioned residual
 */
typedef struct {

	unsigned *parameters;
	/**< The Rice parameters for each context. */

	unsigned *raw_bits;
	/**< Widths for escape-coded partitions.  Will be non-zero for escaped
	 * partitions and zero for unescaped partitions.
	 */

	unsigned capacity_by_order;
	/**< The capacity of the \a parameters and \a raw_bits arrays
	 * specified as an order, i.e. the number of array elements
	 * allocated is 2 ^ \a capacity_by_order.
	 */
} FLAC__EntropyCodingMethod_PartitionedRiceContents;

/** Header for a Rice partitioned residual.  (c.f. <A HREF="../format.html#partitioned_rice">format specification</A>)
 */
typedef struct {

	unsigned order;
	/**< The partition order, i.e. # of contexts = 2 ^ \a order. */

	const FLAC__EntropyCodingMethod_PartitionedRiceContents *contents;
	/**< The context's Rice parameters and/or raw bits. */

} FLAC__EntropyCodingMethod_PartitionedRice;

extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE_ORDER_LEN; /**< == 4 (bits) */
extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE_PARAMETER_LEN; /**< == 4 (bits) */
extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE2_PARAMETER_LEN; /**< == 5 (bits) */
extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE_RAW_LEN; /**< == 5 (bits) */

extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE_ESCAPE_PARAMETER;
/**< == (1<<FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE_PARAMETER_LEN)-1 */
extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE2_ESCAPE_PARAMETER;
/**< == (1<<FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE2_PARAMETER_LEN)-1 */

/** Header for the entropy coding method.  (c.f. <A HREF="../format.html#residual">format specification</A>)
 */
typedef struct {
	FLAC__EntropyCodingMethodType type;
	union {
		FLAC__EntropyCodingMethod_PartitionedRice partitioned_rice;
	} data;
} FLAC__EntropyCodingMethod;

extern FLAC_API const unsigned FLAC__ENTROPY_CODING_METHOD_TYPE_LEN; /**< == 2 (bits) */

/*****************************************************************************/

/** An enumeration of the available subframe types. */
typedef enum {
	FLAC__SUBFRAME_TYPE_CONSTANT = 0, /**< constant signal */
	FLAC__SUBFRAME_TYPE_VERBATIM = 1, /**< uncompressed signal */
	FLAC__SUBFRAME_TYPE_FIXED = 2, /**< fixed polynomial prediction */
	FLAC__SUBFRAME_TYPE_LPC = 3 /**< linear prediction */
} FLAC__SubframeType;

/** Maps a FLAC__SubframeType to a C string.
 *
 *  Using a FLAC__SubframeType as the index to this array will
 *  give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__SubframeTypeString[];


/** CONSTANT subframe.  (c.f. <A HREF="../format.html#subframe_constant">format specification</A>)
 */
typedef struct {
	FLAC__int32 value; /**< The constant signal value. */
} FLAC__Subframe_Constant;


/** VERBATIM subframe.  (c.f. <A HREF="../format.html#subframe_verbatim">format specification</A>)
 */
typedef struct {
	const FLAC__int32 *data; /**< A pointer to verbatim signal. */
} FLAC__Subframe_Verbatim;


/** FIXED subframe.  (c.f. <A HREF="../format.html#subframe_fixed">format specification</A>)
 */
typedef struct {
	FLAC__EntropyCodingMethod entropy_coding_method;
	/**< The residual coding method. */

	unsigned order;
	/**< The polynomial order. */

	FLAC__int32 warmup[FLAC__MAX_FIXED_ORDER];
	/**< Warmup samples to prime the predictor, length == order. */

	const FLAC__int32 *residual;
	/**< The residual signal, length == (blocksize minus order) samples. */
} FLAC__Subframe_Fixed;


/** LPC subframe.  (c.f. <A HREF="../format.html#subframe_lpc">format specification</A>)
 */
typedef struct {
	FLAC__EntropyCodingMethod entropy_coding_method;
	/**< The residual coding method. */

	unsigned order;
	/**< The FIR order. */

	unsigned qlp_coeff_precision;
	/**< Quantized FIR filter coefficient precision in bits. */

	int quantization_level;
	/**< The qlp coeff shift needed. */

	FLAC__int32 qlp_coeff[FLAC__MAX_LPC_ORDER];
	/**< FIR filter coefficients. */

	FLAC__int32 warmup[FLAC__MAX_LPC_ORDER];
	/**< Warmup samples to prime the predictor, length == order. */

	const FLAC__int32 *residual;
	/**< The residual signal, length == (blocksize minus order) samples. */
} FLAC__Subframe_LPC;

extern FLAC_API const unsigned FLAC__SUBFRAME_LPC_QLP_COEFF_PRECISION_LEN; /**< == 4 (bits) */
extern FLAC_API const unsigned FLAC__SUBFRAME_LPC_QLP_SHIFT_LEN; /**< == 5 (bits) */


/** FLAC subframe structure.  (c.f. <A HREF="../format.html#subframe">format specification</A>)
 */
typedef struct {
	FLAC__SubframeType type;
	union {
		FLAC__Subframe_Constant constant;
		FLAC__Subframe_Fixed fixed;
		FLAC__Subframe_LPC lpc;
		FLAC__Subframe_Verbatim verbatim;
	} data;
	unsigned wasted_bits;
} FLAC__Subframe;

/** == 1 (bit)
 *
 * This used to be a zero-padding bit (hence the name
 * FLAC__SUBFRAME_ZERO_PAD_LEN) but is now a reserved bit.  It still has a
 * mandatory value of \c 0 but in the future may take on the value \c 0 or \c 1
 * to mean something else.
 */
extern FLAC_API const unsigned FLAC__SUBFRAME_ZERO_PAD_LEN;
extern FLAC_API const unsigned FLAC__SUBFRAME_TYPE_LEN; /**< == 6 (bits) */
extern FLAC_API const unsigned FLAC__SUBFRAME_WASTED_BITS_FLAG_LEN; /**< == 1 (bit) */

extern FLAC_API const unsigned FLAC__SUBFRAME_TYPE_CONSTANT_BYTE_ALIGNED_MASK; /**< = 0x00 */
extern FLAC_API const unsigned FLAC__SUBFRAME_TYPE_VERBATIM_BYTE_ALIGNED_MASK; /**< = 0x02 */
extern FLAC_API const unsigned FLAC__SUBFRAME_TYPE_FIXED_BYTE_ALIGNED_MASK; /**< = 0x10 */
extern FLAC_API const unsigned FLAC__SUBFRAME_TYPE_LPC_BYTE_ALIGNED_MASK; /**< = 0x40 */

/*****************************************************************************/


/*****************************************************************************
 *
 * Frame structures
 *
 *****************************************************************************/

/** An enumeration of the available channel assignments. */
typedef enum {
	FLAC__CHANNEL_ASSIGNMENT_INDEPENDENT = 0, /**< independent channels */
	FLAC__CHANNEL_ASSIGNMENT_LEFT_SIDE = 1, /**< left+side stereo */
	FLAC__CHANNEL_ASSIGNMENT_RIGHT_SIDE = 2, /**< right+side stereo */
	FLAC__CHANNEL_ASSIGNMENT_MID_SIDE = 3 /**< mid+side stereo */
} FLAC__ChannelAssignment;

/** Maps a FLAC__ChannelAssignment to a C string.
 *
 *  Using a FLAC__ChannelAssignment as the index to this array will
 *  give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__ChannelAssignmentString[];

/** An enumeration of the possible frame numbering methods. */
typedef enum {
	FLAC__FRAME_NUMBER_TYPE_FRAME_NUMBER, /**< number contains the frame number */
	FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER /**< number contains the sample number of first sample in frame */
} FLAC__FrameNumberType;

/** Maps a FLAC__FrameNumberType to a C string.
 *
 *  Using a FLAC__FrameNumberType as the index to this array will
 *  give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__FrameNumberTypeString[];


/** FLAC frame header structure.  (c.f. <A HREF="../format.html#frame_header">format specification</A>)
 */
typedef struct {
	unsigned blocksize;
	/**< The number of samples per subframe. */

	unsigned sample_rate;
	/**< The sample rate in Hz. */

	unsigned channels;
	/**< The number of channels (== number of subframes). */

	FLAC__ChannelAssignment channel_assignment;
	/**< The channel assignment for the frame. */

	unsigned bits_per_sample;
	/**< The sample resolution. */

	FLAC__FrameNumberType number_type;
	/**< The numbering scheme used for the frame.  As a convenience, the
	 * decoder will always convert a frame number to a sample number because
	 * the rules are complex. */

	union {
		FLAC__uint32 frame_number;
		FLAC__uint64 sample_number;
	} number;
	/**< The frame number or sample number of first sample in frame;
	 * use the \a number_type value to determine which to use. */

	FLAC__uint8 crc;
	/**< CRC-8 (polynomial = x^8 + x^2 + x^1 + x^0, initialized with 0)
	 * of the raw frame header bytes, meaning everything before the CRC byte
	 * including the sync code.
	 */
} FLAC__FrameHeader;

extern FLAC_API const unsigned FLAC__FRAME_HEADER_SYNC; /**< == 0x3ffe; the frame header sync code */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_SYNC_LEN; /**< == 14 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_RESERVED_LEN; /**< == 1 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_BLOCKING_STRATEGY_LEN; /**< == 1 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_BLOCK_SIZE_LEN; /**< == 4 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_SAMPLE_RATE_LEN; /**< == 4 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_CHANNEL_ASSIGNMENT_LEN; /**< == 4 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_BITS_PER_SAMPLE_LEN; /**< == 3 (bits) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_ZERO_PAD_LEN; /**< == 1 (bit) */
extern FLAC_API const unsigned FLAC__FRAME_HEADER_CRC_LEN; /**< == 8 (bits) */


/** FLAC frame footer structure.  (c.f. <A HREF="../format.html#frame_footer">format specification</A>)
 */
typedef struct {
	FLAC__uint16 crc;
	/**< CRC-16 (polynomial = x^16 + x^15 + x^2 + x^0, initialized with
	 * 0) of the bytes before the crc, back to and including the frame header
	 * sync code.
	 */
} FLAC__FrameFooter;

extern FLAC_API const unsigned FLAC__FRAME_FOOTER_CRC_LEN; /**< == 16 (bits) */


/** FLAC frame structure.  (c.f. <A HREF="../format.html#frame">format specification</A>)
 */
typedef struct {
	FLAC__FrameHeader header;
	FLAC__Subframe subframes[FLAC__MAX_CHANNELS];
	FLAC__FrameFooter footer;
} FLAC__Frame;

/*****************************************************************************/


/*****************************************************************************
 *
 * Meta-data structures
 *
 *****************************************************************************/

/** An enumeration of the available metadata block types. */
typedef enum {

	FLAC__METADATA_TYPE_STREAMINFO = 0,
	/**< <A HREF="../format.html#metadata_block_streaminfo">STREAMINFO</A> block */

	FLAC__METADATA_TYPE_PADDING = 1,
	/**< <A HREF="../format.html#metadata_block_padding">PADDING</A> block */

	FLAC__METADATA_TYPE_APPLICATION = 2,
	/**< <A HREF="../format.html#metadata_block_application">APPLICATION</A> block */

	FLAC__METADATA_TYPE_SEEKTABLE = 3,
	/**< <A HREF="../format.html#metadata_block_seektable">SEEKTABLE</A> block */

	FLAC__METADATA_TYPE_VORBIS_COMMENT = 4,
	/**< <A HREF="../format.html#metadata_block_vorbis_comment">VORBISCOMMENT</A> block (a.k.a. FLAC tags) */

	FLAC__METADATA_TYPE_CUESHEET = 5,
	/**< <A HREF="../format.html#metadata_block_cuesheet">CUESHEET</A> block */

	FLAC__METADATA_TYPE_PICTURE = 6,
	/**< <A HREF="../format.html#metadata_block_picture">PICTURE</A> block */

	FLAC__METADATA_TYPE_UNDEFINED = 7,
	/**< marker to denote beginning of undefined type range; this number will increase as new metadata types are added */

        FLAC__MAX_METADATA_TYPE = FLAC__MAX_METADATA_TYPE_CODE,
        /**< No type will ever be greater than this. There is not enough room in the protocol block. */
} FLAC__MetadataType;

/** Maps a FLAC__MetadataType to a C string.
 *
 *  Using a FLAC__MetadataType as the index to this array will
 *  give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__MetadataTypeString[];


/** FLAC STREAMINFO structure.  (c.f. <A HREF="../format.html#metadata_block_streaminfo">format specification</A>)
 */
typedef struct {
	unsigned min_blocksize, max_blocksize;
	unsigned min_framesize, max_framesize;
	unsigned sample_rate;
	unsigned channels;
	unsigned bits_per_sample;
	FLAC__uint64 total_samples;
	FLAC__byte md5sum[16];
} FLAC__StreamMetadata_StreamInfo;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_MIN_BLOCK_SIZE_LEN; /**< == 16 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_MAX_BLOCK_SIZE_LEN; /**< == 16 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_MIN_FRAME_SIZE_LEN; /**< == 24 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_MAX_FRAME_SIZE_LEN; /**< == 24 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_SAMPLE_RATE_LEN; /**< == 20 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_CHANNELS_LEN; /**< == 3 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_BITS_PER_SAMPLE_LEN; /**< == 5 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_TOTAL_SAMPLES_LEN; /**< == 36 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_STREAMINFO_MD5SUM_LEN; /**< == 128 (bits) */

/** The total stream length of the STREAMINFO block in bytes. */
#define FLAC__STREAM_METADATA_STREAMINFO_LENGTH (34u)

/** FLAC PADDING structure.  (c.f. <A HREF="../format.html#metadata_block_padding">format specification</A>)
 */
typedef struct {
	int dummy;
	/**< Conceptually this is an empty struct since we don't store the
	 * padding bytes.  Empty structs are not allowed by some C compilers,
	 * hence the dummy.
	 */
} FLAC__StreamMetadata_Padding;


/** FLAC APPLICATION structure.  (c.f. <A HREF="../format.html#metadata_block_application">format specification</A>)
 */
typedef struct {
	FLAC__byte id[4];
	FLAC__byte *data;
} FLAC__StreamMetadata_Application;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_APPLICATION_ID_LEN; /**< == 32 (bits) */

/** SeekPoint structure used in SEEKTABLE blocks.  (c.f. <A HREF="../format.html#seekpoint">format specification</A>)
 */
typedef struct {
	FLAC__uint64 sample_number;
	/**<  The sample number of the target frame. */

	FLAC__uint64 stream_offset;
	/**< The offset, in bytes, of the target frame with respect to
	 * beginning of the first frame. */

	unsigned frame_samples;
	/**< The number of samples in the target frame. */
} FLAC__StreamMetadata_SeekPoint;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_SEEKPOINT_SAMPLE_NUMBER_LEN; /**< == 64 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_SEEKPOINT_STREAM_OFFSET_LEN; /**< == 64 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_SEEKPOINT_FRAME_SAMPLES_LEN; /**< == 16 (bits) */

/** The total stream length of a seek point in bytes. */
#define FLAC__STREAM_METADATA_SEEKPOINT_LENGTH (18u)

/** The value used in the \a sample_number field of
 *  FLAC__StreamMetadataSeekPoint used to indicate a placeholder
 *  point (== 0xffffffffffffffff).
 */
extern FLAC_API const FLAC__uint64 FLAC__STREAM_METADATA_SEEKPOINT_PLACEHOLDER;


/** FLAC SEEKTABLE structure.  (c.f. <A HREF="../format.html#metadata_block_seektable">format specification</A>)
 *
 * \note From the format specification:
 * - The seek points must be sorted by ascending sample number.
 * - Each seek point's sample number must be the first sample of the
 *   target frame.
 * - Each seek point's sample number must be unique within the table.
 * - Existence of a SEEKTABLE block implies a correct setting of
 *   total_samples in the stream_info block.
 * - Behavior is undefined when more than one SEEKTABLE block is
 *   present in a stream.
 */
typedef struct {
	unsigned num_points;
	FLAC__StreamMetadata_SeekPoint *points;
} FLAC__StreamMetadata_SeekTable;


/** Vorbis comment entry structure used in VORBIS_COMMENT blocks.  (c.f. <A HREF="../format.html#metadata_block_vorbis_comment">format specification</A>)
 *
 *  For convenience, the APIs maintain a trailing NUL character at the end of
 *  \a entry which is not counted toward \a length, i.e.
 *  \code strlen(entry) == length \endcode
 */
typedef struct {
	FLAC__uint32 length;
	FLAC__byte *entry;
} FLAC__StreamMetadata_VorbisComment_Entry;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_VORBIS_COMMENT_ENTRY_LENGTH_LEN; /**< == 32 (bits) */


/** FLAC VORBIS_COMMENT structure.  (c.f. <A HREF="../format.html#metadata_block_vorbis_comment">format specification</A>)
 */
typedef struct {
	FLAC__StreamMetadata_VorbisComment_Entry vendor_string;
	FLAC__uint32 num_comments;
	FLAC__StreamMetadata_VorbisComment_Entry *comments;
} FLAC__StreamMetadata_VorbisComment;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_VORBIS_COMMENT_NUM_COMMENTS_LEN; /**< == 32 (bits) */


/** FLAC CUESHEET track index structure.  (See the
 * <A HREF="../format.html#cuesheet_track_index">format specification</A> for
 * the full description of each field.)
 */
typedef struct {
	FLAC__uint64 offset;
	/**< Offset in samples, relative to the track offset, of the index
	 * point.
	 */

	FLAC__byte number;
	/**< The index point number. */
} FLAC__StreamMetadata_CueSheet_Index;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_INDEX_OFFSET_LEN; /**< == 64 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_INDEX_NUMBER_LEN; /**< == 8 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_INDEX_RESERVED_LEN; /**< == 3*8 (bits) */


/** FLAC CUESHEET track structure.  (See the
 * <A HREF="../format.html#cuesheet_track">format specification</A> for
 * the full description of each field.)
 */
typedef struct {
	FLAC__uint64 offset;
	/**< Track offset in samples, relative to the beginning of the FLAC audio stream. */

	FLAC__byte number;
	/**< The track number. */

	char isrc[13];
	/**< Track ISRC.  This is a 12-digit alphanumeric code plus a trailing \c NUL byte */

	unsigned type:1;
	/**< The track type: 0 for audio, 1 for non-audio. */

	unsigned pre_emphasis:1;
	/**< The pre-emphasis flag: 0 for no pre-emphasis, 1 for pre-emphasis. */

	FLAC__byte num_indices;
	/**< The number of track index points. */

	FLAC__StreamMetadata_CueSheet_Index *indices;
	/**< NULL if num_indices == 0, else pointer to array of index points. */

} FLAC__StreamMetadata_CueSheet_Track;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_OFFSET_LEN; /**< == 64 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_NUMBER_LEN; /**< == 8 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_ISRC_LEN; /**< == 12*8 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_TYPE_LEN; /**< == 1 (bit) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_PRE_EMPHASIS_LEN; /**< == 1 (bit) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_RESERVED_LEN; /**< == 6+13*8 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_TRACK_NUM_INDICES_LEN; /**< == 8 (bits) */


/** FLAC CUESHEET structure.  (See the
 * <A HREF="../format.html#metadata_block_cuesheet">format specification</A>
 * for the full description of each field.)
 */
typedef struct {
	char media_catalog_number[129];
	/**< Media catalog number, in ASCII printable characters 0x20-0x7e.  In
	 * general, the media catalog number may be 0 to 128 bytes long; any
	 * unused characters should be right-padded with NUL characters.
	 */

	FLAC__uint64 lead_in;
	/**< The number of lead-in samples. */

	FLAC__bool is_cd;
	/**< \c true if CUESHEET corresponds to a Compact Disc, else \c false. */

	unsigned num_tracks;
	/**< The number of tracks. */

	FLAC__StreamMetadata_CueSheet_Track *tracks;
	/**< NULL if num_tracks == 0, else pointer to array of tracks. */

} FLAC__StreamMetadata_CueSheet;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_MEDIA_CATALOG_NUMBER_LEN; /**< == 128*8 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_LEAD_IN_LEN; /**< == 64 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_IS_CD_LEN; /**< == 1 (bit) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_RESERVED_LEN; /**< == 7+258*8 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_CUESHEET_NUM_TRACKS_LEN; /**< == 8 (bits) */


/** An enumeration of the PICTURE types (see FLAC__StreamMetadataPicture and id3 v2.4 APIC tag). */
typedef enum {
	FLAC__STREAM_METADATA_PICTURE_TYPE_OTHER = 0, /**< Other */
	FLAC__STREAM_METADATA_PICTURE_TYPE_FILE_ICON_STANDARD = 1, /**< 32x32 pixels 'file icon' (PNG only) */
	FLAC__STREAM_METADATA_PICTURE_TYPE_FILE_ICON = 2, /**< Other file icon */
	FLAC__STREAM_METADATA_PICTURE_TYPE_FRONT_COVER = 3, /**< Cover (front) */
	FLAC__STREAM_METADATA_PICTURE_TYPE_BACK_COVER = 4, /**< Cover (back) */
	FLAC__STREAM_METADATA_PICTURE_TYPE_LEAFLET_PAGE = 5, /**< Leaflet page */
	FLAC__STREAM_METADATA_PICTURE_TYPE_MEDIA = 6, /**< Media (e.g. label side of CD) */
	FLAC__STREAM_METADATA_PICTURE_TYPE_LEAD_ARTIST = 7, /**< Lead artist/lead performer/soloist */
	FLAC__STREAM_METADATA_PICTURE_TYPE_ARTIST = 8, /**< Artist/performer */
	FLAC__STREAM_METADATA_PICTURE_TYPE_CONDUCTOR = 9, /**< Conductor */
	FLAC__STREAM_METADATA_PICTURE_TYPE_BAND = 10, /**< Band/Orchestra */
	FLAC__STREAM_METADATA_PICTURE_TYPE_COMPOSER = 11, /**< Composer */
	FLAC__STREAM_METADATA_PICTURE_TYPE_LYRICIST = 12, /**< Lyricist/text writer */
	FLAC__STREAM_METADATA_PICTURE_TYPE_RECORDING_LOCATION = 13, /**< Recording Location */
	FLAC__STREAM_METADATA_PICTURE_TYPE_DURING_RECORDING = 14, /**< During recording */
	FLAC__STREAM_METADATA_PICTURE_TYPE_DURING_PERFORMANCE = 15, /**< During performance */
	FLAC__STREAM_METADATA_PICTURE_TYPE_VIDEO_SCREEN_CAPTURE = 16, /**< Movie/video screen capture */
	FLAC__STREAM_METADATA_PICTURE_TYPE_FISH = 17, /**< A bright coloured fish */
	FLAC__STREAM_METADATA_PICTURE_TYPE_ILLUSTRATION = 18, /**< Illustration */
	FLAC__STREAM_METADATA_PICTURE_TYPE_BAND_LOGOTYPE = 19, /**< Band/artist logotype */
	FLAC__STREAM_METADATA_PICTURE_TYPE_PUBLISHER_LOGOTYPE = 20, /**< Publisher/Studio logotype */
	FLAC__STREAM_METADATA_PICTURE_TYPE_UNDEFINED
} FLAC__StreamMetadata_Picture_Type;

/** Maps a FLAC__StreamMetadata_Picture_Type to a C string.
 *
 *  Using a FLAC__StreamMetadata_Picture_Type as the index to this array
 *  will give the string equivalent.  The contents should not be
 *  modified.
 */
extern FLAC_API const char * const FLAC__StreamMetadata_Picture_TypeString[];

/** FLAC PICTURE structure.  (See the
 * <A HREF="../format.html#metadata_block_picture">format specification</A>
 * for the full description of each field.)
 */
typedef struct {
	FLAC__StreamMetadata_Picture_Type type;
	/**< The kind of picture stored. */

	char *mime_type;
	/**< Picture data's MIME type, in ASCII printable characters
	 * 0x20-0x7e, NUL terminated.  For best compatibility with players,
	 * use picture data of MIME type \c image/jpeg or \c image/png.  A
	 * MIME type of '-->' is also allowed, in which case the picture
	 * data should be a complete URL.  In file storage, the MIME type is
	 * stored as a 32-bit length followed by the ASCII string with no NUL
	 * terminator, but is converted to a plain C string in this structure
	 * for convenience.
	 */

	FLAC__byte *description;
	/**< Picture's description in UTF-8, NUL terminated.  In file storage,
	 * the description is stored as a 32-bit length followed by the UTF-8
	 * string with no NUL terminator, but is converted to a plain C string
	 * in this structure for convenience.
	 */

	FLAC__uint32 width;
	/**< Picture's width in pixels. */

	FLAC__uint32 height;
	/**< Picture's height in pixels. */

	FLAC__uint32 depth;
	/**< Picture's color depth in bits-per-pixel. */

	FLAC__uint32 colors;
	/**< For indexed palettes (like GIF), picture's number of colors (the
	 * number of palette entries), or \c 0 for non-indexed (i.e. 2^depth).
	 */

	FLAC__uint32 data_length;
	/**< Length of binary picture data in bytes. */

	FLAC__byte *data;
	/**< Binary picture data. */

} FLAC__StreamMetadata_Picture;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_TYPE_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_MIME_TYPE_LENGTH_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_DESCRIPTION_LENGTH_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_WIDTH_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_HEIGHT_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_DEPTH_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_COLORS_LEN; /**< == 32 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_PICTURE_DATA_LENGTH_LEN; /**< == 32 (bits) */


/** Structure that is used when a metadata block of unknown type is loaded.
 *  The contents are opaque.  The structure is used only internally to
 *  correctly handle unknown metadata.
 */
typedef struct {
	FLAC__byte *data;
} FLAC__StreamMetadata_Unknown;


/** FLAC metadata block structure.  (c.f. <A HREF="../format.html#metadata_block">format specification</A>)
 */
typedef struct {
	FLAC__MetadataType type;
	/**< The type of the metadata block; used determine which member of the
	 * \a data union to dereference.  If type >= FLAC__METADATA_TYPE_UNDEFINED
	 * then \a data.unknown must be used. */

	FLAC__bool is_last;
	/**< \c true if this metadata block is the last, else \a false */

	unsigned length;
	/**< Length, in bytes, of the block data as it appears in the stream. */

	union {
		FLAC__StreamMetadata_StreamInfo stream_info;
		FLAC__StreamMetadata_Padding padding;
		FLAC__StreamMetadata_Application application;
		FLAC__StreamMetadata_SeekTable seek_table;
		FLAC__StreamMetadata_VorbisComment vorbis_comment;
		FLAC__StreamMetadata_CueSheet cue_sheet;
		FLAC__StreamMetadata_Picture picture;
		FLAC__StreamMetadata_Unknown unknown;
	} data;
	/**< Polymorphic block data; use the \a type value to determine which
	 * to use. */
} FLAC__StreamMetadata;

extern FLAC_API const unsigned FLAC__STREAM_METADATA_IS_LAST_LEN; /**< == 1 (bit) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_TYPE_LEN; /**< == 7 (bits) */
extern FLAC_API const unsigned FLAC__STREAM_METADATA_LENGTH_LEN; /**< == 24 (bits) */

/** The total stream length of a metadata block header in bytes. */
#define FLAC__STREAM_METADATA_HEADER_LENGTH (4u)

/*****************************************************************************/


/*****************************************************************************
 *
 * Utility functions
 *
 *****************************************************************************/

/** Tests that a sample rate is valid for FLAC.
 *
 * \param sample_rate  The sample rate to test for compliance.
 * \retval FLAC__bool
 *    \c true if the given sample rate conforms to the specification, else
 *    \c false.
 */
FLAC_API FLAC__bool FLAC__format_sample_rate_is_valid(unsigned sample_rate);

/** Tests that a blocksize at the given sample rate is valid for the FLAC
 *  subset.
 *
 * \param blocksize    The blocksize to test for compliance.
 * \param sample_rate  The sample rate is needed, since the valid subset
 *                     blocksize depends on the sample rate.
 * \retval FLAC__bool
 *    \c true if the given blocksize conforms to the specification for the
 *    subset at the given sample rate, else \c false.
 */
FLAC_API FLAC__bool FLAC__format_blocksize_is_subset(unsigned blocksize, unsigned sample_rate);

/** Tests that a sample rate is valid for the FLAC subset.  The subset rules
 *  for valid sample rates are slightly more complex since the rate has to
 *  be expressible completely in the frame header.
 *
 * \param sample_rate  The sample rate to test for compliance.
 * \retval FLAC__bool
 *    \c true if the given sample rate conforms to the specification for the
 *    subset, else \c false.
 */
FLAC_API FLAC__bool FLAC__format_sample_rate_is_subset(unsigned sample_rate);

/** Check a Vorbis comment entry name to see if it conforms to the Vorbis
 *  comment specification.
 *
 *  Vorbis comment names must be composed only of characters from
 *  [0x20-0x3C,0x3E-0x7D].
 *
 * \param name       A NUL-terminated string to be checked.
 * \assert
 *    \code name != NULL \endcode
 * \retval FLAC__bool
 *    \c false if entry name is illegal, else \c true.
 */
FLAC_API FLAC__bool FLAC__format_vorbiscomment_entry_name_is_legal(const char *name);

/** Check a Vorbis comment entry value to see if it conforms to the Vorbis
 *  comment specification.
 *
 *  Vorbis comment values must be valid UTF-8 sequences.
 *
 * \param value      A string to be checked.
 * \param length     A the length of \a value in bytes.  May be
 *                   \c (unsigned)(-1) to indicate that \a value is a plain
 *                   UTF-8 NUL-terminated string.
 * \assert
 *    \code value != NULL \endcode
 * \retval FLAC__bool
 *    \c false if entry name is illegal, else \c true.
 */
FLAC_API FLAC__bool FLAC__format_vorbiscomment_entry_value_is_legal(const FLAC__byte *value, unsigned length);

/** Check a Vorbis comment entry to see if it conforms to the Vorbis
 *  comment specification.
 *
 *  Vorbis comment entries must be of the form 'name=value', and 'name' and
 *  'value' must be legal according to
 *  FLAC__format_vorbiscomment_entry_name_is_legal() and
 *  FLAC__format_vorbiscomment_entry_value_is_legal() respectively.
 *
 * \param entry      An entry to be checked.
 * \param length     The length of \a entry in bytes.
 * \assert
 *    \code value != NULL \endcode
 * \retval FLAC__bool
 *    \c false if entry name is illegal, else \c true.
 */
FLAC_API FLAC__bool FLAC__format_vorbiscomment_entry_is_legal(const FLAC__byte *entry, unsigned length);

/** Check a seek table to see if it conforms to the FLAC specification.
 *  See the format specification for limits on the contents of the
 *  seek table.
 *
 * \param seek_table  A pointer to a seek table to be checked.
 * \assert
 *    \code seek_table != NULL \endcode
 * \retval FLAC__bool
 *    \c false if seek table is illegal, else \c true.
 */
FLAC_API FLAC__bool FLAC__format_seektable_is_legal(const FLAC__StreamMetadata_SeekTable *seek_table);

/** Sort a seek table's seek points according to the format specification.
 *  This includes a "unique-ification" step to remove duplicates, i.e.
 *  seek points with identical \a sample_number values.  Duplicate seek
 *  points are converted into placeholder points and sorted to the end of
 *  the table.
 *
 * \param seek_table  A pointer to a seek table to be sorted.
 * \assert
 *    \code seek_table != NULL \endcode
 * \retval unsigned
 *    The number of duplicate seek points converted into placeholders.
 */
FLAC_API unsigned FLAC__format_seektable_sort(FLAC__StreamMetadata_SeekTable *seek_table);

/** Check a cue sheet to see if it conforms to the FLAC specification.
 *  See the format specification for limits on the contents of the
 *  cue sheet.
 *
 * \param cue_sheet  A pointer to an existing cue sheet to be checked.
 * \param check_cd_da_subset  If \c true, check CUESHEET against more
 *                   stringent requirements for a CD-DA (audio) disc.
 * \param violation  Address of a pointer to a string.  If there is a
 *                   violation, a pointer to a string explanation of the
 *                   violation will be returned here. \a violation may be
 *                   \c NULL if you don't need the returned string.  Do not
 *                   free the returned string; it will always point to static
 *                   data.
 * \assert
 *    \code cue_sheet != NULL \endcode
 * \retval FLAC__bool
 *    \c false if cue sheet is illegal, else \c true.
 */
FLAC_API FLAC__bool FLAC__format_cuesheet_is_legal(const FLAC__StreamMetadata_CueSheet *cue_sheet, FLAC__bool check_cd_da_subset, const char **violation);

/** Check picture data to see if it conforms to the FLAC specification.
 *  See the format specification for limits on the contents of the
 *  PICTURE block.
 *
 * \param picture    A pointer to existing picture data to be checked.
 * \param violation  Address of a pointer to a string.  If there is a
 *                   violation, a pointer to a string explanation of the
 *                   violation will be returned here. \a violation may be
 *                   \c NULL if you don't need the returned string.  Do not
 *                   free the returned string; it will always point to static
 *                   data.
 * \assert
 *    \code picture != NULL \endcode
 * \retval FLAC__bool
 *    \c false if picture data is illegal, else \c true.
 */
FLAC_API FLAC__bool FLAC__format_picture_is_legal(const FLAC__StreamMetadata_Picture *picture, const char **violation);

/* \} */

#ifdef __cplusplus
}
#endif

#endif
/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000-2009  Josh Coalson
 * Copyright (C) 2011-2014  Xiph.Org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__STREAM_DECODER_H
#define FLAC__STREAM_DECODER_H

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif


/** \file include/FLAC/stream_decoder.h
 *
 *  \brief
 *  This module contains the functions which implement the stream
 *  decoder.
 *
 *  See the detailed documentation in the
 *  \link flac_stream_decoder stream decoder \endlink module.
 */

/** \defgroup flac_decoder FLAC/ \*_decoder.h: decoder interfaces
 *  \ingroup flac
 *
 *  \brief
 *  This module describes the decoder layers provided by libFLAC.
 *
 * The stream decoder can be used to decode complete streams either from
 * the client via callbacks, or directly from a file, depending on how
 * it is initialized.  When decoding via callbacks, the client provides
 * callbacks for reading FLAC data and writing decoded samples, and
 * handling metadata and errors.  If the client also supplies seek-related
 * callback, the decoder function for sample-accurate seeking within the
 * FLAC input is also available.  When decoding from a file, the client
 * needs only supply a filename or open \c FILE* and write/metadata/error
 * callbacks; the rest of the callbacks are supplied internally.  For more
 * info see the \link flac_stream_decoder stream decoder \endlink module.
 */

/** \defgroup flac_stream_decoder FLAC/stream_decoder.h: stream decoder interface
 *  \ingroup flac_decoder
 *
 *  \brief
 *  This module contains the functions which implement the stream
 *  decoder.
 *
 * The stream decoder can decode native FLAC, and optionally Ogg FLAC
 * (check FLAC_API_SUPPORTS_OGG_FLAC) streams and files.
 *
 * The basic usage of this decoder is as follows:
 * - The program creates an instance of a decoder using
 *   FLAC__stream_decoder_new().
 * - The program overrides the default settings using
 *   FLAC__stream_decoder_set_*() functions.
 * - The program initializes the instance to validate the settings and
 *   prepare for decoding using
 *   - FLAC__stream_decoder_init_stream() or FLAC__stream_decoder_init_FILE()
 *     or FLAC__stream_decoder_init_file() for native FLAC,
 *   - FLAC__stream_decoder_init_ogg_stream() or FLAC__stream_decoder_init_ogg_FILE()
 *     or FLAC__stream_decoder_init_ogg_file() for Ogg FLAC
 * - The program calls the FLAC__stream_decoder_process_*() functions
 *   to decode data, which subsequently calls the callbacks.
 * - The program finishes the decoding with FLAC__stream_decoder_finish(),
 *   which flushes the input and output and resets the decoder to the
 *   uninitialized state.
 * - The instance may be used again or deleted with
 *   FLAC__stream_decoder_delete().
 *
 * In more detail, the program will create a new instance by calling
 * FLAC__stream_decoder_new(), then call FLAC__stream_decoder_set_*()
 * functions to override the default decoder options, and call
 * one of the FLAC__stream_decoder_init_*() functions.
 *
 * There are three initialization functions for native FLAC, one for
 * setting up the decoder to decode FLAC data from the client via
 * callbacks, and two for decoding directly from a FLAC file.
 *
 * For decoding via callbacks, use FLAC__stream_decoder_init_stream().
 * You must also supply several callbacks for handling I/O.  Some (like
 * seeking) are optional, depending on the capabilities of the input.
 *
 * For decoding directly from a file, use FLAC__stream_decoder_init_FILE()
 * or FLAC__stream_decoder_init_file().  Then you must only supply an open
 * \c FILE* or filename and fewer callbacks; the decoder will handle
 * the other callbacks internally.
 *
 * There are three similarly-named init functions for decoding from Ogg
 * FLAC streams.  Check \c FLAC_API_SUPPORTS_OGG_FLAC to find out if the
 * library has been built with Ogg support.
 *
 * Once the decoder is initialized, your program will call one of several
 * functions to start the decoding process:
 *
 * - FLAC__stream_decoder_process_single() - Tells the decoder to process at
 *   most one metadata block or audio frame and return, calling either the
 *   metadata callback or write callback, respectively, once.  If the decoder
 *   loses sync it will return with only the error callback being called.
 * - FLAC__stream_decoder_process_until_end_of_metadata() - Tells the decoder
 *   to process the stream from the current location and stop upon reaching
 *   the first audio frame.  The client will get one metadata, write, or error
 *   callback per metadata block, audio frame, or sync error, respectively.
 * - FLAC__stream_decoder_process_until_end_of_stream() - Tells the decoder
 *   to process the stream from the current location until the read callback
 *   returns FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM or
 *   FLAC__STREAM_DECODER_READ_STATUS_ABORT.  The client will get one metadata,
 *   write, or error callback per metadata block, audio frame, or sync error,
 *   respectively.
 *
 * When the decoder has finished decoding (normally or through an abort),
 * the instance is finished by calling FLAC__stream_decoder_finish(), which
 * ensures the decoder is in the correct state and frees memory.  Then the
 * instance may be deleted with FLAC__stream_decoder_delete() or initialized
 * again to decode another stream.
 *
 * Seeking is exposed through the FLAC__stream_decoder_seek_absolute() method.
 * At any point after the stream decoder has been initialized, the client can
 * call this function to seek to an exact sample within the stream.
 * Subsequently, the first time the write callback is called it will be
 * passed a (possibly partial) block starting at that sample.
 *
 * If the client cannot seek via the callback interface provided, but still
 * has another way of seeking, it can flush the decoder using
 * FLAC__stream_decoder_flush() and start feeding data from the new position
 * through the read callback.
 *
 * The stream decoder also provides MD5 signature checking.  If this is
 * turned on before initialization, FLAC__stream_decoder_finish() will
 * report when the decoded MD5 signature does not match the one stored
 * in the STREAMINFO block.  MD5 checking is automatically turned off
 * (until the next FLAC__stream_decoder_reset()) if there is no signature
 * in the STREAMINFO block or when a seek is attempted.
 *
 * The FLAC__stream_decoder_set_metadata_*() functions deserve special
 * attention.  By default, the decoder only calls the metadata_callback for
 * the STREAMINFO block.  These functions allow you to tell the decoder
 * explicitly which blocks to parse and return via the metadata_callback
 * and/or which to skip.  Use a FLAC__stream_decoder_set_metadata_respond_all(),
 * FLAC__stream_decoder_set_metadata_ignore() ... or FLAC__stream_decoder_set_metadata_ignore_all(),
 * FLAC__stream_decoder_set_metadata_respond() ... sequence to exactly specify
 * which blocks to return.  Remember that metadata blocks can potentially
 * be big (for example, cover art) so filtering out the ones you don't
 * use can reduce the memory requirements of the decoder.  Also note the
 * special forms FLAC__stream_decoder_set_metadata_respond_application(id)
 * and FLAC__stream_decoder_set_metadata_ignore_application(id) for
 * filtering APPLICATION blocks based on the application ID.
 *
 * STREAMINFO and SEEKTABLE blocks are always parsed and used internally, but
 * they still can legally be filtered from the metadata_callback.
 *
 * \note
 * The "set" functions may only be called when the decoder is in the
 * state FLAC__STREAM_DECODER_UNINITIALIZED, i.e. after
 * FLAC__stream_decoder_new() or FLAC__stream_decoder_finish(), but
 * before FLAC__stream_decoder_init_*().  If this is the case they will
 * return \c true, otherwise \c false.
 *
 * \note
 * FLAC__stream_decoder_finish() resets all settings to the constructor
 * defaults, including the callbacks.
 *
 * \{
 */


/** State values for a FLAC__StreamDecoder
 *
 * The decoder's state can be obtained by calling FLAC__stream_decoder_get_state().
 */
typedef enum {

	FLAC__STREAM_DECODER_SEARCH_FOR_METADATA = 0,
	/**< The decoder is ready to search for metadata. */

	FLAC__STREAM_DECODER_READ_METADATA,
	/**< The decoder is ready to or is in the process of reading metadata. */

	FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC,
	/**< The decoder is ready to or is in the process of searching for the
	 * frame sync code.
	 */

	FLAC__STREAM_DECODER_READ_FRAME,
	/**< The decoder is ready to or is in the process of reading a frame. */

	FLAC__STREAM_DECODER_END_OF_STREAM,
	/**< The decoder has reached the end of the stream. */

	FLAC__STREAM_DECODER_OGG_ERROR,
	/**< An error occurred in the underlying Ogg layer.  */

	FLAC__STREAM_DECODER_SEEK_ERROR,
	/**< An error occurred while seeking.  The decoder must be flushed
	 * with FLAC__stream_decoder_flush() or reset with
	 * FLAC__stream_decoder_reset() before decoding can continue.
	 */

	FLAC__STREAM_DECODER_ABORTED,
	/**< The decoder was aborted by the read callback. */

	FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR,
	/**< An error occurred allocating memory.  The decoder is in an invalid
	 * state and can no longer be used.
	 */

	FLAC__STREAM_DECODER_UNINITIALIZED
	/**< The decoder is in the uninitialized state; one of the
	 * FLAC__stream_decoder_init_*() functions must be called before samples
	 * can be processed.
	 */

} FLAC__StreamDecoderState;

/** Maps a FLAC__StreamDecoderState to a C string.
 *
 *  Using a FLAC__StreamDecoderState as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderStateString[];


/** Possible return values for the FLAC__stream_decoder_init_*() functions.
 */
typedef enum {

	FLAC__STREAM_DECODER_INIT_STATUS_OK = 0,
	/**< Initialization was successful. */

	FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER,
	/**< The library was not compiled with support for the given container
	 * format.
	 */

	FLAC__STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS,
	/**< A required callback was not supplied. */

	FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR,
	/**< An error occurred allocating memory. */

	FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE,
	/**< fopen() failed in FLAC__stream_decoder_init_file() or
	 * FLAC__stream_decoder_init_ogg_file(). */

	FLAC__STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED
	/**< FLAC__stream_decoder_init_*() was called when the decoder was
	 * already initialized, usually because
	 * FLAC__stream_decoder_finish() was not called.
	 */

} FLAC__StreamDecoderInitStatus;

/** Maps a FLAC__StreamDecoderInitStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderInitStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderInitStatusString[];


/** Return values for the FLAC__StreamDecoder read callback.
 */
typedef enum {

	FLAC__STREAM_DECODER_READ_STATUS_CONTINUE,
	/**< The read was OK and decoding can continue. */

	FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM,
	/**< The read was attempted while at the end of the stream.  Note that
	 * the client must only return this value when the read callback was
	 * called when already at the end of the stream.  Otherwise, if the read
	 * itself moves to the end of the stream, the client should still return
	 * the data and \c FLAC__STREAM_DECODER_READ_STATUS_CONTINUE, and then on
	 * the next read callback it should return
	 * \c FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM with a byte count
	 * of \c 0.
	 */

	FLAC__STREAM_DECODER_READ_STATUS_ABORT
	/**< An unrecoverable error occurred.  The decoder will return from the process call. */

} FLAC__StreamDecoderReadStatus;

/** Maps a FLAC__StreamDecoderReadStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderReadStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderReadStatusString[];


/** Return values for the FLAC__StreamDecoder seek callback.
 */
typedef enum {

	FLAC__STREAM_DECODER_SEEK_STATUS_OK,
	/**< The seek was OK and decoding can continue. */

	FLAC__STREAM_DECODER_SEEK_STATUS_ERROR,
	/**< An unrecoverable error occurred.  The decoder will return from the process call. */

	FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED
	/**< Client does not support seeking. */

} FLAC__StreamDecoderSeekStatus;

/** Maps a FLAC__StreamDecoderSeekStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderSeekStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderSeekStatusString[];


/** Return values for the FLAC__StreamDecoder tell callback.
 */
typedef enum {

	FLAC__STREAM_DECODER_TELL_STATUS_OK,
	/**< The tell was OK and decoding can continue. */

	FLAC__STREAM_DECODER_TELL_STATUS_ERROR,
	/**< An unrecoverable error occurred.  The decoder will return from the process call. */

	FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED
	/**< Client does not support telling the position. */

} FLAC__StreamDecoderTellStatus;

/** Maps a FLAC__StreamDecoderTellStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderTellStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderTellStatusString[];


/** Return values for the FLAC__StreamDecoder length callback.
 */
typedef enum {

	FLAC__STREAM_DECODER_LENGTH_STATUS_OK,
	/**< The length call was OK and decoding can continue. */

	FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR,
	/**< An unrecoverable error occurred.  The decoder will return from the process call. */

	FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED
	/**< Client does not support reporting the length. */

} FLAC__StreamDecoderLengthStatus;

/** Maps a FLAC__StreamDecoderLengthStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderLengthStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderLengthStatusString[];


/** Return values for the FLAC__StreamDecoder write callback.
 */
typedef enum {

	FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE,
	/**< The write was OK and decoding can continue. */

	FLAC__STREAM_DECODER_WRITE_STATUS_ABORT
	/**< An unrecoverable error occurred.  The decoder will return from the process call. */

} FLAC__StreamDecoderWriteStatus;

/** Maps a FLAC__StreamDecoderWriteStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderWriteStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderWriteStatusString[];


/** Possible values passed back to the FLAC__StreamDecoder error callback.
 *  \c FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC is the generic catch-
 *  all.  The rest could be caused by bad sync (false synchronization on
 *  data that is not the start of a frame) or corrupted data.  The error
 *  itself is the decoder's best guess at what happened assuming a correct
 *  sync.  For example \c FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER
 *  could be caused by a correct sync on the start of a frame, but some
 *  data in the frame header was corrupted.  Or it could be the result of
 *  syncing on a point the stream that looked like the starting of a frame
 *  but was not.  \c FLAC__STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM
 *  could be because the decoder encountered a valid frame made by a future
 *  version of the encoder which it cannot parse, or because of a false
 *  sync making it appear as though an encountered frame was generated by
 *  a future encoder.
 */
typedef enum {

	FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC,
	/**< An error in the stream caused the decoder to lose synchronization. */

	FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER,
	/**< The decoder encountered a corrupted frame header. */

	FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH,
	/**< The frame's data did not match the CRC in the footer. */

	FLAC__STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM
	/**< The decoder encountered reserved fields in use in the stream. */

} FLAC__StreamDecoderErrorStatus;

/** Maps a FLAC__StreamDecoderErrorStatus to a C string.
 *
 *  Using a FLAC__StreamDecoderErrorStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamDecoderErrorStatusString[];


/***********************************************************************
 *
 * class FLAC__StreamDecoder
 *
 ***********************************************************************/

struct FLAC__StreamDecoderProtected;
struct FLAC__StreamDecoderPrivate;
/** The opaque structure definition for the stream decoder type.
 *  See the \link flac_stream_decoder stream decoder module \endlink
 *  for a detailed description.
 */
typedef struct {
	struct FLAC__StreamDecoderProtected *protected_; /* avoid the C++ keyword 'protected' */
	struct FLAC__StreamDecoderPrivate *private_; /* avoid the C++ keyword 'private' */
} FLAC__StreamDecoder;

/** Signature for the read callback.
 *
 *  A function pointer matching this signature must be passed to
 *  FLAC__stream_decoder_init*_stream(). The supplied function will be
 *  called when the decoder needs more input data.  The address of the
 *  buffer to be filled is supplied, along with the number of bytes the
 *  buffer can hold.  The callback may choose to supply less data and
 *  modify the byte count but must be careful not to overflow the buffer.
 *  The callback then returns a status code chosen from
 *  FLAC__StreamDecoderReadStatus.
 *
 * Here is an example of a read callback for stdio streams:
 * \code
 * FLAC__StreamDecoderReadStatus read_cb(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   if(*bytes > 0) {
 *     *bytes = fread(buffer, sizeof(FLAC__byte), *bytes, file);
 *     if(ferror(file))
 *       return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
 *     else if(*bytes == 0)
 *       return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
 *     else
 *       return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
 *   }
 *   else
 *     return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  buffer   A pointer to a location for the callee to store
 *                  data to be decoded.
 * \param  bytes    A pointer to the size of the buffer.  On entry
 *                  to the callback, it contains the maximum number
 *                  of bytes that may be stored in \a buffer.  The
 *                  callee must set it to the actual number of bytes
 *                  stored (0 in case of error or end-of-stream) before
 *                  returning.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 * \retval FLAC__StreamDecoderReadStatus
 *    The callee's return status.  Note that the callback should return
 *    \c FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM if and only if
 *    zero bytes were read and there is no more data to be read.
 */
typedef FLAC__StreamDecoderReadStatus (*FLAC__StreamDecoderReadCallback)(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data);

/** Signature for the seek callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_decoder_init*_stream().  The supplied function will be
 *  called when the decoder needs to seek the input stream.  The decoder
 *  will pass the absolute byte offset to seek to, 0 meaning the
 *  beginning of the stream.
 *
 * Here is an example of a seek callback for stdio streams:
 * \code
 * FLAC__StreamDecoderSeekStatus seek_cb(const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   if(file == stdin)
 *     return FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED;
 *   else if(fseeko(file, (off_t)absolute_byte_offset, SEEK_SET) < 0)
 *     return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
 *   else
 *     return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  absolute_byte_offset  The offset from the beginning of the stream
 *                               to seek to.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 * \retval FLAC__StreamDecoderSeekStatus
 *    The callee's return status.
 */
typedef FLAC__StreamDecoderSeekStatus (*FLAC__StreamDecoderSeekCallback)(const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data);

/** Signature for the tell callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_decoder_init*_stream().  The supplied function will be
 *  called when the decoder wants to know the current position of the
 *  stream.  The callback should return the byte offset from the
 *  beginning of the stream.
 *
 * Here is an example of a tell callback for stdio streams:
 * \code
 * FLAC__StreamDecoderTellStatus tell_cb(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   off_t pos;
 *   if(file == stdin)
 *     return FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED;
 *   else if((pos = ftello(file)) < 0)
 *     return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
 *   else {
 *     *absolute_byte_offset = (FLAC__uint64)pos;
 *     return FLAC__STREAM_DECODER_TELL_STATUS_OK;
 *   }
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  absolute_byte_offset  A pointer to storage for the current offset
 *                               from the beginning of the stream.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 * \retval FLAC__StreamDecoderTellStatus
 *    The callee's return status.
 */
typedef FLAC__StreamDecoderTellStatus (*FLAC__StreamDecoderTellCallback)(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data);

/** Signature for the length callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_decoder_init*_stream().  The supplied function will be
 *  called when the decoder wants to know the total length of the stream
 *  in bytes.
 *
 * Here is an example of a length callback for stdio streams:
 * \code
 * FLAC__StreamDecoderLengthStatus length_cb(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   struct stat filestats;
 *
 *   if(file == stdin)
 *     return FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED;
 *   else if(fstat(fileno(file), &filestats) != 0)
 *     return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
 *   else {
 *     *stream_length = (FLAC__uint64)filestats.st_size;
 *     return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
 *   }
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  stream_length  A pointer to storage for the length of the stream
 *                        in bytes.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 * \retval FLAC__StreamDecoderLengthStatus
 *    The callee's return status.
 */
typedef FLAC__StreamDecoderLengthStatus (*FLAC__StreamDecoderLengthCallback)(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data);

/** Signature for the EOF callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_decoder_init*_stream().  The supplied function will be
 *  called when the decoder needs to know if the end of the stream has
 *  been reached.
 *
 * Here is an example of a EOF callback for stdio streams:
 * FLAC__bool eof_cb(const FLAC__StreamDecoder *decoder, void *client_data)
 * \code
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   return feof(file)? true : false;
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 * \retval FLAC__bool
 *    \c true if the currently at the end of the stream, else \c false.
 */
typedef FLAC__bool (*FLAC__StreamDecoderEofCallback)(const FLAC__StreamDecoder *decoder, void *client_data);

/** Signature for the write callback.
 *
 *  A function pointer matching this signature must be passed to one of
 *  the FLAC__stream_decoder_init_*() functions.
 *  The supplied function will be called when the decoder has decoded a
 *  single audio frame.  The decoder will pass the frame metadata as well
 *  as an array of pointers (one for each channel) pointing to the
 *  decoded audio.
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  frame    The description of the decoded frame.  See
 *                  FLAC__Frame.
 * \param  buffer   An array of pointers to decoded channels of data.
 *                  Each pointer will point to an array of signed
 *                  samples of length \a frame->header.blocksize.
 *                  Channels will be ordered according to the FLAC
 *                  specification; see the documentation for the
 *                  <A HREF="../format.html#frame_header">frame header</A>.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 * \retval FLAC__StreamDecoderWriteStatus
 *    The callee's return status.
 */
typedef FLAC__StreamDecoderWriteStatus (*FLAC__StreamDecoderWriteCallback)(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);

/** Signature for the metadata callback.
 *
 *  A function pointer matching this signature must be passed to one of
 *  the FLAC__stream_decoder_init_*() functions.
 *  The supplied function will be called when the decoder has decoded a
 *  metadata block.  In a valid FLAC file there will always be one
 *  \c STREAMINFO block, followed by zero or more other metadata blocks.
 *  These will be supplied by the decoder in the same order as they
 *  appear in the stream and always before the first audio frame (i.e.
 *  write callback).  The metadata block that is passed in must not be
 *  modified, and it doesn't live beyond the callback, so you should make
 *  a copy of it with FLAC__metadata_object_clone() if you will need it
 *  elsewhere.  Since metadata blocks can potentially be large, by
 *  default the decoder only calls the metadata callback for the
 *  \c STREAMINFO block; you can instruct the decoder to pass or filter
 *  other blocks with FLAC__stream_decoder_set_metadata_*() calls.
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  metadata The decoded metadata block.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 */
typedef void (*FLAC__StreamDecoderMetadataCallback)(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);

/** Signature for the error callback.
 *
 *  A function pointer matching this signature must be passed to one of
 *  the FLAC__stream_decoder_init_*() functions.
 *  The supplied function will be called whenever an error occurs during
 *  decoding.
 *
 * \note In general, FLAC__StreamDecoder functions which change the
 * state should not be called on the \a decoder while in the callback.
 *
 * \param  decoder  The decoder instance calling the callback.
 * \param  status   The error encountered by the decoder.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_decoder_init_*().
 */
typedef void (*FLAC__StreamDecoderErrorCallback)(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);


/***********************************************************************
 *
 * Class constructor/destructor
 *
 ***********************************************************************/

/** Create a new stream decoder instance.  The instance is created with
 *  default settings; see the individual FLAC__stream_decoder_set_*()
 *  functions for each setting's default.
 *
 * \retval FLAC__StreamDecoder*
 *    \c NULL if there was an error allocating memory, else the new instance.
 */
FLAC_API FLAC__StreamDecoder *FLAC__stream_decoder_new(void);

/** Free a decoder instance.  Deletes the object pointed to by \a decoder.
 *
 * \param decoder  A pointer to an existing decoder.
 * \assert
 *    \code decoder != NULL \endcode
 */
FLAC_API void FLAC__stream_decoder_delete(FLAC__StreamDecoder *decoder);


/***********************************************************************
 *
 * Public class method prototypes
 *
 ***********************************************************************/

/** Set the serial number for the FLAC stream within the Ogg container.
 *  The default behavior is to use the serial number of the first Ogg
 *  page.  Setting a serial number here will explicitly specify which
 *  stream is to be decoded.
 *
 * \note
 * This does not need to be set for native FLAC decoding.
 *
 * \default \c use serial number of first page
 * \param  decoder        A decoder instance to set.
 * \param  serial_number  See above.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_ogg_serial_number(FLAC__StreamDecoder *decoder, long serial_number);

/** Set the "MD5 signature checking" flag.  If \c true, the decoder will
 *  compute the MD5 signature of the unencoded audio data while decoding
 *  and compare it to the signature from the STREAMINFO block, if it
 *  exists, during FLAC__stream_decoder_finish().
 *
 *  MD5 signature checking will be turned off (until the next
 *  FLAC__stream_decoder_reset()) if there is no signature in the
 *  STREAMINFO block or when a seek is attempted.
 *
 *  Clients that do not use the MD5 check should leave this off to speed
 *  up decoding.
 *
 * \default \c false
 * \param  decoder  A decoder instance to set.
 * \param  value    Flag value (see above).
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_md5_checking(FLAC__StreamDecoder *decoder, FLAC__bool value);

/** Direct the decoder to pass on all metadata blocks of type \a type.
 *
 * \default By default, only the \c STREAMINFO block is returned via the
 *          metadata callback.
 * \param  decoder  A decoder instance to set.
 * \param  type     See above.
 * \assert
 *    \code decoder != NULL \endcode
 *    \a type is valid
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_respond(FLAC__StreamDecoder *decoder, FLAC__MetadataType type);

/** Direct the decoder to pass on all APPLICATION metadata blocks of the
 *  given \a id.
 *
 * \default By default, only the \c STREAMINFO block is returned via the
 *          metadata callback.
 * \param  decoder  A decoder instance to set.
 * \param  id       See above.
 * \assert
 *    \code decoder != NULL \endcode
 *    \code id != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_respond_application(FLAC__StreamDecoder *decoder, const FLAC__byte id[4]);

/** Direct the decoder to pass on all metadata blocks of any type.
 *
 * \default By default, only the \c STREAMINFO block is returned via the
 *          metadata callback.
 * \param  decoder  A decoder instance to set.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_respond_all(FLAC__StreamDecoder *decoder);

/** Direct the decoder to filter out all metadata blocks of type \a type.
 *
 * \default By default, only the \c STREAMINFO block is returned via the
 *          metadata callback.
 * \param  decoder  A decoder instance to set.
 * \param  type     See above.
 * \assert
 *    \code decoder != NULL \endcode
 *    \a type is valid
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_ignore(FLAC__StreamDecoder *decoder, FLAC__MetadataType type);

/** Direct the decoder to filter out all APPLICATION metadata blocks of
 *  the given \a id.
 *
 * \default By default, only the \c STREAMINFO block is returned via the
 *          metadata callback.
 * \param  decoder  A decoder instance to set.
 * \param  id       See above.
 * \assert
 *    \code decoder != NULL \endcode
 *    \code id != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_ignore_application(FLAC__StreamDecoder *decoder, const FLAC__byte id[4]);

/** Direct the decoder to filter out all metadata blocks of any type.
 *
 * \default By default, only the \c STREAMINFO block is returned via the
 *          metadata callback.
 * \param  decoder  A decoder instance to set.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the decoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_ignore_all(FLAC__StreamDecoder *decoder);

/** Get the current decoder state.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__StreamDecoderState
 *    The current decoder state.
 */
FLAC_API FLAC__StreamDecoderState FLAC__stream_decoder_get_state(const FLAC__StreamDecoder *decoder);

/** Get the current decoder state as a C string.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval const char *
 *    The decoder state as a C string.  Do not modify the contents.
 */
FLAC_API const char *FLAC__stream_decoder_get_resolved_state_string(const FLAC__StreamDecoder *decoder);

/** Get the "MD5 signature checking" flag.
 *  This is the value of the setting, not whether or not the decoder is
 *  currently checking the MD5 (remember, it can be turned off automatically
 *  by a seek).  When the decoder is reset the flag will be restored to the
 *  value returned by this function.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    See above.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_get_md5_checking(const FLAC__StreamDecoder *decoder);

/** Get the total number of samples in the stream being decoded.
 *  Will only be valid after decoding has started and will contain the
 *  value from the \c STREAMINFO block.  A value of \c 0 means "unknown".
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval unsigned
 *    See above.
 */
FLAC_API FLAC__uint64 FLAC__stream_decoder_get_total_samples(const FLAC__StreamDecoder *decoder);

/** Get the current number of channels in the stream being decoded.
 *  Will only be valid after decoding has started and will contain the
 *  value from the most recently decoded frame header.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval unsigned
 *    See above.
 */
FLAC_API unsigned FLAC__stream_decoder_get_channels(const FLAC__StreamDecoder *decoder);

/** Get the current channel assignment in the stream being decoded.
 *  Will only be valid after decoding has started and will contain the
 *  value from the most recently decoded frame header.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__ChannelAssignment
 *    See above.
 */
FLAC_API FLAC__ChannelAssignment FLAC__stream_decoder_get_channel_assignment(const FLAC__StreamDecoder *decoder);

/** Get the current sample resolution in the stream being decoded.
 *  Will only be valid after decoding has started and will contain the
 *  value from the most recently decoded frame header.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval unsigned
 *    See above.
 */
FLAC_API unsigned FLAC__stream_decoder_get_bits_per_sample(const FLAC__StreamDecoder *decoder);

/** Get the current sample rate in Hz of the stream being decoded.
 *  Will only be valid after decoding has started and will contain the
 *  value from the most recently decoded frame header.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval unsigned
 *    See above.
 */
FLAC_API unsigned FLAC__stream_decoder_get_sample_rate(const FLAC__StreamDecoder *decoder);

/** Get the current blocksize of the stream being decoded.
 *  Will only be valid after decoding has started and will contain the
 *  value from the most recently decoded frame header.
 *
 * \param  decoder  A decoder instance to query.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval unsigned
 *    See above.
 */
FLAC_API unsigned FLAC__stream_decoder_get_blocksize(const FLAC__StreamDecoder *decoder);

/** Returns the decoder's current read position within the stream.
 *  The position is the byte offset from the start of the stream.
 *  Bytes before this position have been fully decoded.  Note that
 *  there may still be undecoded bytes in the decoder's read FIFO.
 *  The returned position is correct even after a seek.
 *
 *  \warning This function currently only works for native FLAC,
 *           not Ogg FLAC streams.
 *
 * \param  decoder   A decoder instance to query.
 * \param  position  Address at which to return the desired position.
 * \assert
 *    \code decoder != NULL \endcode
 *    \code position != NULL \endcode
 * \retval FLAC__bool
 *    \c true if successful, \c false if the stream is not native FLAC,
 *    or there was an error from the 'tell' callback or it returned
 *    \c FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_get_decode_position(const FLAC__StreamDecoder *decoder, FLAC__uint64 *position);

/** Initialize the decoder instance to decode native FLAC streams.
 *
 *  This flavor of initialization sets up the decoder to decode from a
 *  native FLAC stream. I/O is performed via callbacks to the client.
 *  For decoding from a plain file via filename or open FILE*,
 *  FLAC__stream_decoder_init_file() and FLAC__stream_decoder_init_FILE()
 *  provide a simpler interface.
 *
 *  This function should be called after FLAC__stream_decoder_new() and
 *  FLAC__stream_decoder_set_*() but before any of the
 *  FLAC__stream_decoder_process_*() functions.  Will set and return the
 *  decoder state, which will be FLAC__STREAM_DECODER_SEARCH_FOR_METADATA
 *  if initialization succeeded.
 *
 * \param  decoder            An uninitialized decoder instance.
 * \param  read_callback      See FLAC__StreamDecoderReadCallback.  This
 *                            pointer must not be \c NULL.
 * \param  seek_callback      See FLAC__StreamDecoderSeekCallback.  This
 *                            pointer may be \c NULL if seeking is not
 *                            supported.  If \a seek_callback is not \c NULL then a
 *                            \a tell_callback, \a length_callback, and \a eof_callback must also be supplied.
 *                            Alternatively, a dummy seek callback that just
 *                            returns \c FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  tell_callback      See FLAC__StreamDecoderTellCallback.  This
 *                            pointer may be \c NULL if not supported by the client.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a tell_callback must also be supplied.
 *                            Alternatively, a dummy tell callback that just
 *                            returns \c FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  length_callback    See FLAC__StreamDecoderLengthCallback.  This
 *                            pointer may be \c NULL if not supported by the client.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a length_callback must also be supplied.
 *                            Alternatively, a dummy length callback that just
 *                            returns \c FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  eof_callback       See FLAC__StreamDecoderEofCallback.  This
 *                            pointer may be \c NULL if not supported by the client.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a eof_callback must also be supplied.
 *                            Alternatively, a dummy length callback that just
 *                            returns \c false
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  write_callback     See FLAC__StreamDecoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  metadata_callback  See FLAC__StreamDecoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  error_callback     See FLAC__StreamDecoderErrorCallback.  This
 *                            pointer must not be \c NULL.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__StreamDecoderInitStatus
 *    \c FLAC__STREAM_DECODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamDecoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_stream(
	FLAC__StreamDecoder *decoder,
	FLAC__StreamDecoderReadCallback read_callback,
	FLAC__StreamDecoderSeekCallback seek_callback,
	FLAC__StreamDecoderTellCallback tell_callback,
	FLAC__StreamDecoderLengthCallback length_callback,
	FLAC__StreamDecoderEofCallback eof_callback,
	FLAC__StreamDecoderWriteCallback write_callback,
	FLAC__StreamDecoderMetadataCallback metadata_callback,
	FLAC__StreamDecoderErrorCallback error_callback,
	void *client_data
);

/** Initialize the decoder instance to decode Ogg FLAC streams.
 *
 *  This flavor of initialization sets up the decoder to decode from a
 *  FLAC stream in an Ogg container. I/O is performed via callbacks to the
 *  client.  For decoding from a plain file via filename or open FILE*,
 *  FLAC__stream_decoder_init_ogg_file() and FLAC__stream_decoder_init_ogg_FILE()
 *  provide a simpler interface.
 *
 *  This function should be called after FLAC__stream_decoder_new() and
 *  FLAC__stream_decoder_set_*() but before any of the
 *  FLAC__stream_decoder_process_*() functions.  Will set and return the
 *  decoder state, which will be FLAC__STREAM_DECODER_SEARCH_FOR_METADATA
 *  if initialization succeeded.
 *
 *  \note Support for Ogg FLAC in the library is optional.  If this
 *  library has been built without support for Ogg FLAC, this function
 *  will return \c FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER.
 *
 * \param  decoder            An uninitialized decoder instance.
 * \param  read_callback      See FLAC__StreamDecoderReadCallback.  This
 *                            pointer must not be \c NULL.
 * \param  seek_callback      See FLAC__StreamDecoderSeekCallback.  This
 *                            pointer may be \c NULL if seeking is not
 *                            supported.  If \a seek_callback is not \c NULL then a
 *                            \a tell_callback, \a length_callback, and \a eof_callback must also be supplied.
 *                            Alternatively, a dummy seek callback that just
 *                            returns \c FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  tell_callback      See FLAC__StreamDecoderTellCallback.  This
 *                            pointer may be \c NULL if not supported by the client.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a tell_callback must also be supplied.
 *                            Alternatively, a dummy tell callback that just
 *                            returns \c FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  length_callback    See FLAC__StreamDecoderLengthCallback.  This
 *                            pointer may be \c NULL if not supported by the client.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a length_callback must also be supplied.
 *                            Alternatively, a dummy length callback that just
 *                            returns \c FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  eof_callback       See FLAC__StreamDecoderEofCallback.  This
 *                            pointer may be \c NULL if not supported by the client.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a eof_callback must also be supplied.
 *                            Alternatively, a dummy length callback that just
 *                            returns \c false
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the decoder.
 * \param  write_callback     See FLAC__StreamDecoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  metadata_callback  See FLAC__StreamDecoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  error_callback     See FLAC__StreamDecoderErrorCallback.  This
 *                            pointer must not be \c NULL.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__StreamDecoderInitStatus
 *    \c FLAC__STREAM_DECODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamDecoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_ogg_stream(
	FLAC__StreamDecoder *decoder,
	FLAC__StreamDecoderReadCallback read_callback,
	FLAC__StreamDecoderSeekCallback seek_callback,
	FLAC__StreamDecoderTellCallback tell_callback,
	FLAC__StreamDecoderLengthCallback length_callback,
	FLAC__StreamDecoderEofCallback eof_callback,
	FLAC__StreamDecoderWriteCallback write_callback,
	FLAC__StreamDecoderMetadataCallback metadata_callback,
	FLAC__StreamDecoderErrorCallback error_callback,
	void *client_data
);

/** Initialize the decoder instance to decode native FLAC files.
 *
 *  This flavor of initialization sets up the decoder to decode from a
 *  plain native FLAC file.  For non-stdio streams, you must use
 *  FLAC__stream_decoder_init_stream() and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_decoder_new() and
 *  FLAC__stream_decoder_set_*() but before any of the
 *  FLAC__stream_decoder_process_*() functions.  Will set and return the
 *  decoder state, which will be FLAC__STREAM_DECODER_SEARCH_FOR_METADATA
 *  if initialization succeeded.
 *
 * \param  decoder            An uninitialized decoder instance.
 * \param  file               An open FLAC file.  The file should have been
 *                            opened with mode \c "rb" and rewound.  The file
 *                            becomes owned by the decoder and should not be
 *                            manipulated by the client while decoding.
 *                            Unless \a file is \c stdin, it will be closed
 *                            when FLAC__stream_decoder_finish() is called.
 *                            Note however that seeking will not work when
 *                            decoding from \c stdout since it is not seekable.
 * \param  write_callback     See FLAC__StreamDecoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  metadata_callback  See FLAC__StreamDecoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  error_callback     See FLAC__StreamDecoderErrorCallback.  This
 *                            pointer must not be \c NULL.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code decoder != NULL \endcode
 *    \code file != NULL \endcode
 * \retval FLAC__StreamDecoderInitStatus
 *    \c FLAC__STREAM_DECODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamDecoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_FILE(
	FLAC__StreamDecoder *decoder,
	FILE *file,
	FLAC__StreamDecoderWriteCallback write_callback,
	FLAC__StreamDecoderMetadataCallback metadata_callback,
	FLAC__StreamDecoderErrorCallback error_callback,
	void *client_data
);

/** Initialize the decoder instance to decode Ogg FLAC files.
 *
 *  This flavor of initialization sets up the decoder to decode from a
 *  plain Ogg FLAC file.  For non-stdio streams, you must use
 *  FLAC__stream_decoder_init_ogg_stream() and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_decoder_new() and
 *  FLAC__stream_decoder_set_*() but before any of the
 *  FLAC__stream_decoder_process_*() functions.  Will set and return the
 *  decoder state, which will be FLAC__STREAM_DECODER_SEARCH_FOR_METADATA
 *  if initialization succeeded.
 *
 *  \note Support for Ogg FLAC in the library is optional.  If this
 *  library has been built without support for Ogg FLAC, this function
 *  will return \c FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER.
 *
 * \param  decoder            An uninitialized decoder instance.
 * \param  file               An open FLAC file.  The file should have been
 *                            opened with mode \c "rb" and rewound.  The file
 *                            becomes owned by the decoder and should not be
 *                            manipulated by the client while decoding.
 *                            Unless \a file is \c stdin, it will be closed
 *                            when FLAC__stream_decoder_finish() is called.
 *                            Note however that seeking will not work when
 *                            decoding from \c stdout since it is not seekable.
 * \param  write_callback     See FLAC__StreamDecoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  metadata_callback  See FLAC__StreamDecoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  error_callback     See FLAC__StreamDecoderErrorCallback.  This
 *                            pointer must not be \c NULL.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code decoder != NULL \endcode
 *    \code file != NULL \endcode
 * \retval FLAC__StreamDecoderInitStatus
 *    \c FLAC__STREAM_DECODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamDecoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_ogg_FILE(
	FLAC__StreamDecoder *decoder,
	FILE *file,
	FLAC__StreamDecoderWriteCallback write_callback,
	FLAC__StreamDecoderMetadataCallback metadata_callback,
	FLAC__StreamDecoderErrorCallback error_callback,
	void *client_data
);

/** Initialize the decoder instance to decode native FLAC files.
 *
 *  This flavor of initialization sets up the decoder to decode from a plain
 *  native FLAC file.  If POSIX fopen() semantics are not sufficient, (for
 *  example, with Unicode filenames on Windows), you must use
 *  FLAC__stream_decoder_init_FILE(), or FLAC__stream_decoder_init_stream()
 *  and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_decoder_new() and
 *  FLAC__stream_decoder_set_*() but before any of the
 *  FLAC__stream_decoder_process_*() functions.  Will set and return the
 *  decoder state, which will be FLAC__STREAM_DECODER_SEARCH_FOR_METADATA
 *  if initialization succeeded.
 *
 * \param  decoder            An uninitialized decoder instance.
 * \param  filename           The name of the file to decode from.  The file will
 *                            be opened with fopen().  Use \c NULL to decode from
 *                            \c stdin.  Note that \c stdin is not seekable.
 * \param  write_callback     See FLAC__StreamDecoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  metadata_callback  See FLAC__StreamDecoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  error_callback     See FLAC__StreamDecoderErrorCallback.  This
 *                            pointer must not be \c NULL.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__StreamDecoderInitStatus
 *    \c FLAC__STREAM_DECODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamDecoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_file(
	FLAC__StreamDecoder *decoder,
	const char *filename,
	FLAC__StreamDecoderWriteCallback write_callback,
	FLAC__StreamDecoderMetadataCallback metadata_callback,
	FLAC__StreamDecoderErrorCallback error_callback,
	void *client_data
);

/** Initialize the decoder instance to decode Ogg FLAC files.
 *
 *  This flavor of initialization sets up the decoder to decode from a plain
 *  Ogg FLAC file.  If POSIX fopen() semantics are not sufficient, (for
 *  example, with Unicode filenames on Windows), you must use
 *  FLAC__stream_decoder_init_ogg_FILE(), or FLAC__stream_decoder_init_ogg_stream()
 *  and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_decoder_new() and
 *  FLAC__stream_decoder_set_*() but before any of the
 *  FLAC__stream_decoder_process_*() functions.  Will set and return the
 *  decoder state, which will be FLAC__STREAM_DECODER_SEARCH_FOR_METADATA
 *  if initialization succeeded.
 *
 *  \note Support for Ogg FLAC in the library is optional.  If this
 *  library has been built without support for Ogg FLAC, this function
 *  will return \c FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER.
 *
 * \param  decoder            An uninitialized decoder instance.
 * \param  filename           The name of the file to decode from.  The file will
 *                            be opened with fopen().  Use \c NULL to decode from
 *                            \c stdin.  Note that \c stdin is not seekable.
 * \param  write_callback     See FLAC__StreamDecoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  metadata_callback  See FLAC__StreamDecoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  error_callback     See FLAC__StreamDecoderErrorCallback.  This
 *                            pointer must not be \c NULL.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__StreamDecoderInitStatus
 *    \c FLAC__STREAM_DECODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamDecoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_ogg_file(
	FLAC__StreamDecoder *decoder,
	const char *filename,
	FLAC__StreamDecoderWriteCallback write_callback,
	FLAC__StreamDecoderMetadataCallback metadata_callback,
	FLAC__StreamDecoderErrorCallback error_callback,
	void *client_data
);

/** Finish the decoding process.
 *  Flushes the decoding buffer, releases resources, resets the decoder
 *  settings to their defaults, and returns the decoder state to
 *  FLAC__STREAM_DECODER_UNINITIALIZED.
 *
 *  In the event of a prematurely-terminated decode, it is not strictly
 *  necessary to call this immediately before FLAC__stream_decoder_delete()
 *  but it is good practice to match every FLAC__stream_decoder_init_*()
 *  with a FLAC__stream_decoder_finish().
 *
 * \param  decoder  An uninitialized decoder instance.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if MD5 checking is on AND a STREAMINFO block was available
 *    AND the MD5 signature in the STREAMINFO block was non-zero AND the
 *    signature does not match the one computed by the decoder; else
 *    \c true.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_finish(FLAC__StreamDecoder *decoder);

/** Flush the stream input.
 *  The decoder's input buffer will be cleared and the state set to
 *  \c FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC.  This will also turn
 *  off MD5 checking.
 *
 * \param  decoder  A decoder instance.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c true if successful, else \c false if a memory allocation
 *    error occurs (in which case the state will be set to
 *    \c FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR).
 */
FLAC_API FLAC__bool FLAC__stream_decoder_flush(FLAC__StreamDecoder *decoder);

/** Reset the decoding process.
 *  The decoder's input buffer will be cleared and the state set to
 *  \c FLAC__STREAM_DECODER_SEARCH_FOR_METADATA.  This is similar to
 *  FLAC__stream_decoder_finish() except that the settings are
 *  preserved; there is no need to call FLAC__stream_decoder_init_*()
 *  before decoding again.  MD5 checking will be restored to its original
 *  setting.
 *
 *  If the decoder is seekable, or was initialized with
 *  FLAC__stream_decoder_init*_FILE() or FLAC__stream_decoder_init*_file(),
 *  the decoder will also attempt to seek to the beginning of the file.
 *  If this rewind fails, this function will return \c false.  It follows
 *  that FLAC__stream_decoder_reset() cannot be used when decoding from
 *  \c stdin.
 *
 *  If the decoder was initialized with FLAC__stream_encoder_init*_stream()
 *  and is not seekable (i.e. no seek callback was provided or the seek
 *  callback returns \c FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED), it
 *  is the duty of the client to start feeding data from the beginning of
 *  the stream on the next FLAC__stream_decoder_process() or
 *  FLAC__stream_decoder_process_interleaved() call.
 *
 * \param  decoder  A decoder instance.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c true if successful, else \c false if a memory allocation occurs
 *    (in which case the state will be set to
 *    \c FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR) or a seek error
 *    occurs (the state will be unchanged).
 */
FLAC_API FLAC__bool FLAC__stream_decoder_reset(FLAC__StreamDecoder *decoder);

/** Decode one metadata block or audio frame.
 *  This version instructs the decoder to decode a either a single metadata
 *  block or a single frame and stop, unless the callbacks return a fatal
 *  error or the read callback returns
 *  \c FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM.
 *
 *  As the decoder needs more input it will call the read callback.
 *  Depending on what was decoded, the metadata or write callback will be
 *  called with the decoded metadata block or audio frame.
 *
 *  Unless there is a fatal read error or end of stream, this function
 *  will return once one whole frame is decoded.  In other words, if the
 *  stream is not synchronized or points to a corrupt frame header, the
 *  decoder will continue to try and resync until it gets to a valid
 *  frame, then decode one frame, then return.  If the decoder points to
 *  a frame whose frame CRC in the frame footer does not match the
 *  computed frame CRC, this function will issue a
 *  FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH error to the
 *  error callback, and return, having decoded one complete, although
 *  corrupt, frame.  (Such corrupted frames are sent as silence of the
 *  correct length to the write callback.)
 *
 * \param  decoder  An initialized decoder instance.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if any fatal read, write, or memory allocation error
 *    occurred (meaning decoding must stop), else \c true; for more
 *    information about the decoder, check the decoder state with
 *    FLAC__stream_decoder_get_state().
 */
FLAC_API FLAC__bool FLAC__stream_decoder_process_single(FLAC__StreamDecoder *decoder);

/** Decode until the end of the metadata.
 *  This version instructs the decoder to decode from the current position
 *  and continue until all the metadata has been read, or until the
 *  callbacks return a fatal error or the read callback returns
 *  \c FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM.
 *
 *  As the decoder needs more input it will call the read callback.
 *  As each metadata block is decoded, the metadata callback will be called
 *  with the decoded metadata.
 *
 * \param  decoder  An initialized decoder instance.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if any fatal read, write, or memory allocation error
 *    occurred (meaning decoding must stop), else \c true; for more
 *    information about the decoder, check the decoder state with
 *    FLAC__stream_decoder_get_state().
 */
FLAC_API FLAC__bool FLAC__stream_decoder_process_until_end_of_metadata(FLAC__StreamDecoder *decoder);

/** Decode until the end of the stream.
 *  This version instructs the decoder to decode from the current position
 *  and continue until the end of stream (the read callback returns
 *  \c FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM), or until the
 *  callbacks return a fatal error.
 *
 *  As the decoder needs more input it will call the read callback.
 *  As each metadata block and frame is decoded, the metadata or write
 *  callback will be called with the decoded metadata or frame.
 *
 * \param  decoder  An initialized decoder instance.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if any fatal read, write, or memory allocation error
 *    occurred (meaning decoding must stop), else \c true; for more
 *    information about the decoder, check the decoder state with
 *    FLAC__stream_decoder_get_state().
 */
FLAC_API FLAC__bool FLAC__stream_decoder_process_until_end_of_stream(FLAC__StreamDecoder *decoder);

/** Skip one audio frame.
 *  This version instructs the decoder to 'skip' a single frame and stop,
 *  unless the callbacks return a fatal error or the read callback returns
 *  \c FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM.
 *
 *  The decoding flow is the same as what occurs when
 *  FLAC__stream_decoder_process_single() is called to process an audio
 *  frame, except that this function does not decode the parsed data into
 *  PCM or call the write callback.  The integrity of the frame is still
 *  checked the same way as in the other process functions.
 *
 *  This function will return once one whole frame is skipped, in the
 *  same way that FLAC__stream_decoder_process_single() will return once
 *  one whole frame is decoded.
 *
 *  This function can be used in more quickly determining FLAC frame
 *  boundaries when decoding of the actual data is not needed, for
 *  example when an application is separating a FLAC stream into frames
 *  for editing or storing in a container.  To do this, the application
 *  can use FLAC__stream_decoder_skip_single_frame() to quickly advance
 *  to the next frame, then use
 *  FLAC__stream_decoder_get_decode_position() to find the new frame
 *  boundary.
 *
 *  This function should only be called when the stream has advanced
 *  past all the metadata, otherwise it will return \c false.
 *
 * \param  decoder  An initialized decoder instance not in a metadata
 *                  state.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if any fatal read, write, or memory allocation error
 *    occurred (meaning decoding must stop), or if the decoder
 *    is in the FLAC__STREAM_DECODER_SEARCH_FOR_METADATA or
 *    FLAC__STREAM_DECODER_READ_METADATA state, else \c true; for more
 *    information about the decoder, check the decoder state with
 *    FLAC__stream_decoder_get_state().
 */
FLAC_API FLAC__bool FLAC__stream_decoder_skip_single_frame(FLAC__StreamDecoder *decoder);

/** Flush the input and seek to an absolute sample.
 *  Decoding will resume at the given sample.  Note that because of
 *  this, the next write callback may contain a partial block.  The
 *  client must support seeking the input or this function will fail
 *  and return \c false.  Furthermore, if the decoder state is
 *  \c FLAC__STREAM_DECODER_SEEK_ERROR, then the decoder must be flushed
 *  with FLAC__stream_decoder_flush() or reset with
 *  FLAC__stream_decoder_reset() before decoding can continue.
 *
 * \param  decoder  A decoder instance.
 * \param  sample   The target sample number to seek to.
 * \assert
 *    \code decoder != NULL \endcode
 * \retval FLAC__bool
 *    \c true if successful, else \c false.
 */
FLAC_API FLAC__bool FLAC__stream_decoder_seek_absolute(FLAC__StreamDecoder *decoder, FLAC__uint64 sample);

/* \} */

#ifdef __cplusplus
}
#endif

#endif
/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000-2009  Josh Coalson
 * Copyright (C) 2011-2014  Xiph.Org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__STREAM_ENCODER_H
#define FLAC__STREAM_ENCODER_H

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif


/** \file include/FLAC/stream_encoder.h
 *
 *  \brief
 *  This module contains the functions which implement the stream
 *  encoder.
 *
 *  See the detailed documentation in the
 *  \link flac_stream_encoder stream encoder \endlink module.
 */

/** \defgroup flac_encoder FLAC/ \*_encoder.h: encoder interfaces
 *  \ingroup flac
 *
 *  \brief
 *  This module describes the encoder layers provided by libFLAC.
 *
 * The stream encoder can be used to encode complete streams either to the
 * client via callbacks, or directly to a file, depending on how it is
 * initialized.  When encoding via callbacks, the client provides a write
 * callback which will be called whenever FLAC data is ready to be written.
 * If the client also supplies a seek callback, the encoder will also
 * automatically handle the writing back of metadata discovered while
 * encoding, like stream info, seek points offsets, etc.  When encoding to
 * a file, the client needs only supply a filename or open \c FILE* and an
 * optional progress callback for periodic notification of progress; the
 * write and seek callbacks are supplied internally.  For more info see the
 * \link flac_stream_encoder stream encoder \endlink module.
 */

/** \defgroup flac_stream_encoder FLAC/stream_encoder.h: stream encoder interface
 *  \ingroup flac_encoder
 *
 *  \brief
 *  This module contains the functions which implement the stream
 *  encoder.
 *
 * The stream encoder can encode to native FLAC, and optionally Ogg FLAC
 * (check FLAC_API_SUPPORTS_OGG_FLAC) streams and files.
 *
 * The basic usage of this encoder is as follows:
 * - The program creates an instance of an encoder using
 *   FLAC__stream_encoder_new().
 * - The program overrides the default settings using
 *   FLAC__stream_encoder_set_*() functions.  At a minimum, the following
 *   functions should be called:
 *   - FLAC__stream_encoder_set_channels()
 *   - FLAC__stream_encoder_set_bits_per_sample()
 *   - FLAC__stream_encoder_set_sample_rate()
 *   - FLAC__stream_encoder_set_ogg_serial_number() (if encoding to Ogg FLAC)
 *   - FLAC__stream_encoder_set_total_samples_estimate() (if known)
 * - If the application wants to control the compression level or set its own
 *   metadata, then the following should also be called:
 *   - FLAC__stream_encoder_set_compression_level()
 *   - FLAC__stream_encoder_set_verify()
 *   - FLAC__stream_encoder_set_metadata()
 * - The rest of the set functions should only be called if the client needs
 *   exact control over how the audio is compressed; thorough understanding
 *   of the FLAC format is necessary to achieve good results.
 * - The program initializes the instance to validate the settings and
 *   prepare for encoding using
 *   - FLAC__stream_encoder_init_stream() or FLAC__stream_encoder_init_FILE()
 *     or FLAC__stream_encoder_init_file() for native FLAC
 *   - FLAC__stream_encoder_init_ogg_stream() or FLAC__stream_encoder_init_ogg_FILE()
 *     or FLAC__stream_encoder_init_ogg_file() for Ogg FLAC
 * - The program calls FLAC__stream_encoder_process() or
 *   FLAC__stream_encoder_process_interleaved() to encode data, which
 *   subsequently calls the callbacks when there is encoder data ready
 *   to be written.
 * - The program finishes the encoding with FLAC__stream_encoder_finish(),
 *   which causes the encoder to encode any data still in its input pipe,
 *   update the metadata with the final encoding statistics if output
 *   seeking is possible, and finally reset the encoder to the
 *   uninitialized state.
 * - The instance may be used again or deleted with
 *   FLAC__stream_encoder_delete().
 *
 * In more detail, the stream encoder functions similarly to the
 * \link flac_stream_decoder stream decoder \endlink, but has fewer
 * callbacks and more options.  Typically the client will create a new
 * instance by calling FLAC__stream_encoder_new(), then set the necessary
 * parameters with FLAC__stream_encoder_set_*(), and initialize it by
 * calling one of the FLAC__stream_encoder_init_*() functions.
 *
 * Unlike the decoders, the stream encoder has many options that can
 * affect the speed and compression ratio.  When setting these parameters
 * you should have some basic knowledge of the format (see the
 * <A HREF="../documentation_format_overview.html">user-level documentation</A>
 * or the <A HREF="../format.html">formal description</A>).  The
 * FLAC__stream_encoder_set_*() functions themselves do not validate the
 * values as many are interdependent.  The FLAC__stream_encoder_init_*()
 * functions will do this, so make sure to pay attention to the state
 * returned by FLAC__stream_encoder_init_*() to make sure that it is
 * FLAC__STREAM_ENCODER_INIT_STATUS_OK.  Any parameters that are not set
 * before FLAC__stream_encoder_init_*() will take on the defaults from
 * the constructor.
 *
 * There are three initialization functions for native FLAC, one for
 * setting up the encoder to encode FLAC data to the client via
 * callbacks, and two for encoding directly to a file.
 *
 * For encoding via callbacks, use FLAC__stream_encoder_init_stream().
 * You must also supply a write callback which will be called anytime
 * there is raw encoded data to write.  If the client can seek the output
 * it is best to also supply seek and tell callbacks, as this allows the
 * encoder to go back after encoding is finished to write back
 * information that was collected while encoding, like seek point offsets,
 * frame sizes, etc.
 *
 * For encoding directly to a file, use FLAC__stream_encoder_init_FILE()
 * or FLAC__stream_encoder_init_file().  Then you must only supply a
 * filename or open \c FILE*; the encoder will handle all the callbacks
 * internally.  You may also supply a progress callback for periodic
 * notification of the encoding progress.
 *
 * There are three similarly-named init functions for encoding to Ogg
 * FLAC streams.  Check \c FLAC_API_SUPPORTS_OGG_FLAC to find out if the
 * library has been built with Ogg support.
 *
 * The call to FLAC__stream_encoder_init_*() currently will also immediately
 * call the write callback several times, once with the \c fLaC signature,
 * and once for each encoded metadata block.  Note that for Ogg FLAC
 * encoding you will usually get at least twice the number of callbacks than
 * with native FLAC, one for the Ogg page header and one for the page body.
 *
 * After initializing the instance, the client may feed audio data to the
 * encoder in one of two ways:
 *
 * - Channel separate, through FLAC__stream_encoder_process() - The client
 *   will pass an array of pointers to buffers, one for each channel, to
 *   the encoder, each of the same length.  The samples need not be
 *   block-aligned, but each channel should have the same number of samples.
 * - Channel interleaved, through
 *   FLAC__stream_encoder_process_interleaved() - The client will pass a single
 *   pointer to data that is channel-interleaved (i.e. channel0_sample0,
 *   channel1_sample0, ... , channelN_sample0, channel0_sample1, ...).
 *   Again, the samples need not be block-aligned but they must be
 *   sample-aligned, i.e. the first value should be channel0_sample0 and
 *   the last value channelN_sampleM.
 *
 * Note that for either process call, each sample in the buffers should be a
 * signed integer, right-justified to the resolution set by
 * FLAC__stream_encoder_set_bits_per_sample().  For example, if the resolution
 * is 16 bits per sample, the samples should all be in the range [-32768,32767].
 *
 * When the client is finished encoding data, it calls
 * FLAC__stream_encoder_finish(), which causes the encoder to encode any
 * data still in its input pipe, and call the metadata callback with the
 * final encoding statistics.  Then the instance may be deleted with
 * FLAC__stream_encoder_delete() or initialized again to encode another
 * stream.
 *
 * For programs that write their own metadata, but that do not know the
 * actual metadata until after encoding, it is advantageous to instruct
 * the encoder to write a PADDING block of the correct size, so that
 * instead of rewriting the whole stream after encoding, the program can
 * just overwrite the PADDING block.  If only the maximum size of the
 * metadata is known, the program can write a slightly larger padding
 * block, then split it after encoding.
 *
 * Make sure you understand how lengths are calculated.  All FLAC metadata
 * blocks have a 4 byte header which contains the type and length.  This
 * length does not include the 4 bytes of the header.  See the format page
 * for the specification of metadata blocks and their lengths.
 *
 * \note
 * If you are writing the FLAC data to a file via callbacks, make sure it
 * is open for update (e.g. mode "w+" for stdio streams).  This is because
 * after the first encoding pass, the encoder will try to seek back to the
 * beginning of the stream, to the STREAMINFO block, to write some data
 * there.  (If using FLAC__stream_encoder_init*_file() or
 * FLAC__stream_encoder_init*_FILE(), the file is managed internally.)
 *
 * \note
 * The "set" functions may only be called when the encoder is in the
 * state FLAC__STREAM_ENCODER_UNINITIALIZED, i.e. after
 * FLAC__stream_encoder_new() or FLAC__stream_encoder_finish(), but
 * before FLAC__stream_encoder_init_*().  If this is the case they will
 * return \c true, otherwise \c false.
 *
 * \note
 * FLAC__stream_encoder_finish() resets all settings to the constructor
 * defaults.
 *
 * \{
 */


/** State values for a FLAC__StreamEncoder.
 *
 * The encoder's state can be obtained by calling FLAC__stream_encoder_get_state().
 *
 * If the encoder gets into any other state besides \c FLAC__STREAM_ENCODER_OK
 * or \c FLAC__STREAM_ENCODER_UNINITIALIZED, it becomes invalid for encoding and
 * must be deleted with FLAC__stream_encoder_delete().
 */
typedef enum {

	FLAC__STREAM_ENCODER_OK = 0,
	/**< The encoder is in the normal OK state and samples can be processed. */

	FLAC__STREAM_ENCODER_UNINITIALIZED,
	/**< The encoder is in the uninitialized state; one of the
	 * FLAC__stream_encoder_init_*() functions must be called before samples
	 * can be processed.
	 */

	FLAC__STREAM_ENCODER_OGG_ERROR,
	/**< An error occurred in the underlying Ogg layer.  */

	FLAC__STREAM_ENCODER_VERIFY_DECODER_ERROR,
	/**< An error occurred in the underlying verify stream decoder;
	 * check FLAC__stream_encoder_get_verify_decoder_state().
	 */

	FLAC__STREAM_ENCODER_VERIFY_MISMATCH_IN_AUDIO_DATA,
	/**< The verify decoder detected a mismatch between the original
	 * audio signal and the decoded audio signal.
	 */

	FLAC__STREAM_ENCODER_CLIENT_ERROR,
	/**< One of the callbacks returned a fatal error. */

	FLAC__STREAM_ENCODER_IO_ERROR,
	/**< An I/O error occurred while opening/reading/writing a file.
	 * Check \c errno.
	 */

	FLAC__STREAM_ENCODER_FRAMING_ERROR,
	/**< An error occurred while writing the stream; usually, the
	 * write_callback returned an error.
	 */

	FLAC__STREAM_ENCODER_MEMORY_ALLOCATION_ERROR
	/**< Memory allocation failed. */

} FLAC__StreamEncoderState;

/** Maps a FLAC__StreamEncoderState to a C string.
 *
 *  Using a FLAC__StreamEncoderState as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamEncoderStateString[];


/** Possible return values for the FLAC__stream_encoder_init_*() functions.
 */
typedef enum {

	FLAC__STREAM_ENCODER_INIT_STATUS_OK = 0,
	/**< Initialization was successful. */

	FLAC__STREAM_ENCODER_INIT_STATUS_ENCODER_ERROR,
	/**< General failure to set up encoder; call FLAC__stream_encoder_get_state() for cause. */

	FLAC__STREAM_ENCODER_INIT_STATUS_UNSUPPORTED_CONTAINER,
	/**< The library was not compiled with support for the given container
	 * format.
	 */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_CALLBACKS,
	/**< A required callback was not supplied. */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_NUMBER_OF_CHANNELS,
	/**< The encoder has an invalid setting for number of channels. */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_BITS_PER_SAMPLE,
	/**< The encoder has an invalid setting for bits-per-sample.
	 * FLAC supports 4-32 bps but the reference encoder currently supports
	 * only up to 24 bps.
	 */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_SAMPLE_RATE,
	/**< The encoder has an invalid setting for the input sample rate. */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_BLOCK_SIZE,
	/**< The encoder has an invalid setting for the block size. */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_MAX_LPC_ORDER,
	/**< The encoder has an invalid setting for the maximum LPC order. */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_QLP_COEFF_PRECISION,
	/**< The encoder has an invalid setting for the precision of the quantized linear predictor coefficients. */

	FLAC__STREAM_ENCODER_INIT_STATUS_BLOCK_SIZE_TOO_SMALL_FOR_LPC_ORDER,
	/**< The specified block size is less than the maximum LPC order. */

	FLAC__STREAM_ENCODER_INIT_STATUS_NOT_STREAMABLE,
	/**< The encoder is bound to the <A HREF="../format.html#subset">Subset</A> but other settings violate it. */

	FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_METADATA,
	/**< The metadata input to the encoder is invalid, in one of the following ways:
	 * - FLAC__stream_encoder_set_metadata() was called with a null pointer but a block count > 0
	 * - One of the metadata blocks contains an undefined type
	 * - It contains an illegal CUESHEET as checked by FLAC__format_cuesheet_is_legal()
	 * - It contains an illegal SEEKTABLE as checked by FLAC__format_seektable_is_legal()
	 * - It contains more than one SEEKTABLE block or more than one VORBIS_COMMENT block
	 */

	FLAC__STREAM_ENCODER_INIT_STATUS_ALREADY_INITIALIZED
	/**< FLAC__stream_encoder_init_*() was called when the encoder was
	 * already initialized, usually because
	 * FLAC__stream_encoder_finish() was not called.
	 */

} FLAC__StreamEncoderInitStatus;

/** Maps a FLAC__StreamEncoderInitStatus to a C string.
 *
 *  Using a FLAC__StreamEncoderInitStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamEncoderInitStatusString[];


/** Return values for the FLAC__StreamEncoder read callback.
 */
typedef enum {

	FLAC__STREAM_ENCODER_READ_STATUS_CONTINUE,
	/**< The read was OK and decoding can continue. */

	FLAC__STREAM_ENCODER_READ_STATUS_END_OF_STREAM,
	/**< The read was attempted at the end of the stream. */

	FLAC__STREAM_ENCODER_READ_STATUS_ABORT,
	/**< An unrecoverable error occurred. */

	FLAC__STREAM_ENCODER_READ_STATUS_UNSUPPORTED
	/**< Client does not support reading back from the output. */

} FLAC__StreamEncoderReadStatus;

/** Maps a FLAC__StreamEncoderReadStatus to a C string.
 *
 *  Using a FLAC__StreamEncoderReadStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamEncoderReadStatusString[];


/** Return values for the FLAC__StreamEncoder write callback.
 */
typedef enum {

	FLAC__STREAM_ENCODER_WRITE_STATUS_OK = 0,
	/**< The write was OK and encoding can continue. */

	FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR
	/**< An unrecoverable error occurred.  The encoder will return from the process call. */

} FLAC__StreamEncoderWriteStatus;

/** Maps a FLAC__StreamEncoderWriteStatus to a C string.
 *
 *  Using a FLAC__StreamEncoderWriteStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamEncoderWriteStatusString[];


/** Return values for the FLAC__StreamEncoder seek callback.
 */
typedef enum {

	FLAC__STREAM_ENCODER_SEEK_STATUS_OK,
	/**< The seek was OK and encoding can continue. */

	FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR,
	/**< An unrecoverable error occurred. */

	FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED
	/**< Client does not support seeking. */

} FLAC__StreamEncoderSeekStatus;

/** Maps a FLAC__StreamEncoderSeekStatus to a C string.
 *
 *  Using a FLAC__StreamEncoderSeekStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamEncoderSeekStatusString[];


/** Return values for the FLAC__StreamEncoder tell callback.
 */
typedef enum {

	FLAC__STREAM_ENCODER_TELL_STATUS_OK,
	/**< The tell was OK and encoding can continue. */

	FLAC__STREAM_ENCODER_TELL_STATUS_ERROR,
	/**< An unrecoverable error occurred. */

	FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED
	/**< Client does not support seeking. */

} FLAC__StreamEncoderTellStatus;

/** Maps a FLAC__StreamEncoderTellStatus to a C string.
 *
 *  Using a FLAC__StreamEncoderTellStatus as the index to this array
 *  will give the string equivalent.  The contents should not be modified.
 */
extern FLAC_API const char * const FLAC__StreamEncoderTellStatusString[];


/***********************************************************************
 *
 * class FLAC__StreamEncoder
 *
 ***********************************************************************/

struct FLAC__StreamEncoderProtected;
struct FLAC__StreamEncoderPrivate;
/** The opaque structure definition for the stream encoder type.
 *  See the \link flac_stream_encoder stream encoder module \endlink
 *  for a detailed description.
 */
typedef struct {
	struct FLAC__StreamEncoderProtected *protected_; /* avoid the C++ keyword 'protected' */
	struct FLAC__StreamEncoderPrivate *private_; /* avoid the C++ keyword 'private' */
} FLAC__StreamEncoder;

/** Signature for the read callback.
 *
 *  A function pointer matching this signature must be passed to
 *  FLAC__stream_encoder_init_ogg_stream() if seeking is supported.
 *  The supplied function will be called when the encoder needs to read back
 *  encoded data.  This happens during the metadata callback, when the encoder
 *  has to read, modify, and rewrite the metadata (e.g. seekpoints) gathered
 *  while encoding.  The address of the buffer to be filled is supplied, along
 *  with the number of bytes the buffer can hold.  The callback may choose to
 *  supply less data and modify the byte count but must be careful not to
 *  overflow the buffer.  The callback then returns a status code chosen from
 *  FLAC__StreamEncoderReadStatus.
 *
 * Here is an example of a read callback for stdio streams:
 * \code
 * FLAC__StreamEncoderReadStatus read_cb(const FLAC__StreamEncoder *encoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   if(*bytes > 0) {
 *     *bytes = fread(buffer, sizeof(FLAC__byte), *bytes, file);
 *     if(ferror(file))
 *       return FLAC__STREAM_ENCODER_READ_STATUS_ABORT;
 *     else if(*bytes == 0)
 *       return FLAC__STREAM_ENCODER_READ_STATUS_END_OF_STREAM;
 *     else
 *       return FLAC__STREAM_ENCODER_READ_STATUS_CONTINUE;
 *   }
 *   else
 *     return FLAC__STREAM_ENCODER_READ_STATUS_ABORT;
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamEncoder functions which change the
 * state should not be called on the \a encoder while in the callback.
 *
 * \param  encoder  The encoder instance calling the callback.
 * \param  buffer   A pointer to a location for the callee to store
 *                  data to be encoded.
 * \param  bytes    A pointer to the size of the buffer.  On entry
 *                  to the callback, it contains the maximum number
 *                  of bytes that may be stored in \a buffer.  The
 *                  callee must set it to the actual number of bytes
 *                  stored (0 in case of error or end-of-stream) before
 *                  returning.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_encoder_set_client_data().
 * \retval FLAC__StreamEncoderReadStatus
 *    The callee's return status.
 */
typedef FLAC__StreamEncoderReadStatus (*FLAC__StreamEncoderReadCallback)(const FLAC__StreamEncoder *encoder, FLAC__byte buffer[], size_t *bytes, void *client_data);

/** Signature for the write callback.
 *
 *  A function pointer matching this signature must be passed to
 *  FLAC__stream_encoder_init*_stream().  The supplied function will be called
 *  by the encoder anytime there is raw encoded data ready to write.  It may
 *  include metadata mixed with encoded audio frames and the data is not
 *  guaranteed to be aligned on frame or metadata block boundaries.
 *
 *  The only duty of the callback is to write out the \a bytes worth of data
 *  in \a buffer to the current position in the output stream.  The arguments
 *  \a samples and \a current_frame are purely informational.  If \a samples
 *  is greater than \c 0, then \a current_frame will hold the current frame
 *  number that is being written; otherwise it indicates that the write
 *  callback is being called to write metadata.
 *
 * \note
 * Unlike when writing to native FLAC, when writing to Ogg FLAC the
 * write callback will be called twice when writing each audio
 * frame; once for the page header, and once for the page body.
 * When writing the page header, the \a samples argument to the
 * write callback will be \c 0.
 *
 * \note In general, FLAC__StreamEncoder functions which change the
 * state should not be called on the \a encoder while in the callback.
 *
 * \param  encoder  The encoder instance calling the callback.
 * \param  buffer   An array of encoded data of length \a bytes.
 * \param  bytes    The byte length of \a buffer.
 * \param  samples  The number of samples encoded by \a buffer.
 *                  \c 0 has a special meaning; see above.
 * \param  current_frame  The number of the current frame being encoded.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_encoder_init_*().
 * \retval FLAC__StreamEncoderWriteStatus
 *    The callee's return status.
 */
typedef FLAC__StreamEncoderWriteStatus (*FLAC__StreamEncoderWriteCallback)(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame, void *client_data);

/** Signature for the seek callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_encoder_init*_stream().  The supplied function will be called
 *  when the encoder needs to seek the output stream.  The encoder will pass
 *  the absolute byte offset to seek to, 0 meaning the beginning of the stream.
 *
 * Here is an example of a seek callback for stdio streams:
 * \code
 * FLAC__StreamEncoderSeekStatus seek_cb(const FLAC__StreamEncoder *encoder, FLAC__uint64 absolute_byte_offset, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   if(file == stdin)
 *     return FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED;
 *   else if(fseeko(file, (off_t)absolute_byte_offset, SEEK_SET) < 0)
 *     return FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR;
 *   else
 *     return FLAC__STREAM_ENCODER_SEEK_STATUS_OK;
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamEncoder functions which change the
 * state should not be called on the \a encoder while in the callback.
 *
 * \param  encoder  The encoder instance calling the callback.
 * \param  absolute_byte_offset  The offset from the beginning of the stream
 *                               to seek to.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_encoder_init_*().
 * \retval FLAC__StreamEncoderSeekStatus
 *    The callee's return status.
 */
typedef FLAC__StreamEncoderSeekStatus (*FLAC__StreamEncoderSeekCallback)(const FLAC__StreamEncoder *encoder, FLAC__uint64 absolute_byte_offset, void *client_data);

/** Signature for the tell callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_encoder_init*_stream().  The supplied function will be called
 *  when the encoder needs to know the current position of the output stream.
 *
 * \warning
 * The callback must return the true current byte offset of the output to
 * which the encoder is writing.  If you are buffering the output, make
 * sure and take this into account.  If you are writing directly to a
 * FILE* from your write callback, ftell() is sufficient.  If you are
 * writing directly to a file descriptor from your write callback, you
 * can use lseek(fd, SEEK_CUR, 0).  The encoder may later seek back to
 * these points to rewrite metadata after encoding.
 *
 * Here is an example of a tell callback for stdio streams:
 * \code
 * FLAC__StreamEncoderTellStatus tell_cb(const FLAC__StreamEncoder *encoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
 * {
 *   FILE *file = ((MyClientData*)client_data)->file;
 *   off_t pos;
 *   if(file == stdin)
 *     return FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED;
 *   else if((pos = ftello(file)) < 0)
 *     return FLAC__STREAM_ENCODER_TELL_STATUS_ERROR;
 *   else {
 *     *absolute_byte_offset = (FLAC__uint64)pos;
 *     return FLAC__STREAM_ENCODER_TELL_STATUS_OK;
 *   }
 * }
 * \endcode
 *
 * \note In general, FLAC__StreamEncoder functions which change the
 * state should not be called on the \a encoder while in the callback.
 *
 * \param  encoder  The encoder instance calling the callback.
 * \param  absolute_byte_offset  The address at which to store the current
 *                               position of the output.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_encoder_init_*().
 * \retval FLAC__StreamEncoderTellStatus
 *    The callee's return status.
 */
typedef FLAC__StreamEncoderTellStatus (*FLAC__StreamEncoderTellCallback)(const FLAC__StreamEncoder *encoder, FLAC__uint64 *absolute_byte_offset, void *client_data);

/** Signature for the metadata callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_encoder_init*_stream().  The supplied function will be called
 *  once at the end of encoding with the populated STREAMINFO structure.  This
 *  is so the client can seek back to the beginning of the file and write the
 *  STREAMINFO block with the correct statistics after encoding (like
 *  minimum/maximum frame size and total samples).
 *
 * \note In general, FLAC__StreamEncoder functions which change the
 * state should not be called on the \a encoder while in the callback.
 *
 * \param  encoder      The encoder instance calling the callback.
 * \param  metadata     The final populated STREAMINFO block.
 * \param  client_data  The callee's client data set through
 *                      FLAC__stream_encoder_init_*().
 */
typedef void (*FLAC__StreamEncoderMetadataCallback)(const FLAC__StreamEncoder *encoder, const FLAC__StreamMetadata *metadata, void *client_data);

/** Signature for the progress callback.
 *
 *  A function pointer matching this signature may be passed to
 *  FLAC__stream_encoder_init*_file() or FLAC__stream_encoder_init*_FILE().
 *  The supplied function will be called when the encoder has finished
 *  writing a frame.  The \c total_frames_estimate argument to the
 *  callback will be based on the value from
 *  FLAC__stream_encoder_set_total_samples_estimate().
 *
 * \note In general, FLAC__StreamEncoder functions which change the
 * state should not be called on the \a encoder while in the callback.
 *
 * \param  encoder          The encoder instance calling the callback.
 * \param  bytes_written    Bytes written so far.
 * \param  samples_written  Samples written so far.
 * \param  frames_written   Frames written so far.
 * \param  total_frames_estimate  The estimate of the total number of
 *                                frames to be written.
 * \param  client_data      The callee's client data set through
 *                          FLAC__stream_encoder_init_*().
 */
typedef void (*FLAC__StreamEncoderProgressCallback)(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data);


/***********************************************************************
 *
 * Class constructor/destructor
 *
 ***********************************************************************/

/** Create a new stream encoder instance.  The instance is created with
 *  default settings; see the individual FLAC__stream_encoder_set_*()
 *  functions for each setting's default.
 *
 * \retval FLAC__StreamEncoder*
 *    \c NULL if there was an error allocating memory, else the new instance.
 */
FLAC_API FLAC__StreamEncoder *FLAC__stream_encoder_new(void);

/** Free an encoder instance.  Deletes the object pointed to by \a encoder.
 *
 * \param encoder  A pointer to an existing encoder.
 * \assert
 *    \code encoder != NULL \endcode
 */
FLAC_API void FLAC__stream_encoder_delete(FLAC__StreamEncoder *encoder);


/***********************************************************************
 *
 * Public class method prototypes
 *
 ***********************************************************************/

/** Set the serial number for the FLAC stream to use in the Ogg container.
 *
 * \note
 * This does not need to be set for native FLAC encoding.
 *
 * \note
 * It is recommended to set a serial number explicitly as the default of '0'
 * may collide with other streams.
 *
 * \default \c 0
 * \param  encoder        An encoder instance to set.
 * \param  serial_number  See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_ogg_serial_number(FLAC__StreamEncoder *encoder, long serial_number);

/** Set the "verify" flag.  If \c true, the encoder will verify it's own
 *  encoded output by feeding it through an internal decoder and comparing
 *  the original signal against the decoded signal.  If a mismatch occurs,
 *  the process call will return \c false.  Note that this will slow the
 *  encoding process by the extra time required for decoding and comparison.
 *
 * \default \c false
 * \param  encoder  An encoder instance to set.
 * \param  value    Flag value (see above).
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_verify(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Set the <A HREF="../format.html#subset">Subset</A> flag.  If \c true,
 *  the encoder will comply with the Subset and will check the
 *  settings during FLAC__stream_encoder_init_*() to see if all settings
 *  comply.  If \c false, the settings may take advantage of the full
 *  range that the format allows.
 *
 *  Make sure you know what it entails before setting this to \c false.
 *
 * \default \c true
 * \param  encoder  An encoder instance to set.
 * \param  value    Flag value (see above).
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_streamable_subset(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Set the number of channels to be encoded.
 *
 * \default \c 2
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_channels(FLAC__StreamEncoder *encoder, unsigned value);

/** Set the sample resolution of the input to be encoded.
 *
 * \warning
 * Do not feed the encoder data that is wider than the value you
 * set here or you will generate an invalid stream.
 *
 * \default \c 16
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_bits_per_sample(FLAC__StreamEncoder *encoder, unsigned value);

/** Set the sample rate (in Hz) of the input to be encoded.
 *
 * \default \c 44100
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_sample_rate(FLAC__StreamEncoder *encoder, unsigned value);

/** Set the compression level
 *
 * The compression level is roughly proportional to the amount of effort
 * the encoder expends to compress the file.  A higher level usually
 * means more computation but higher compression.  The default level is
 * suitable for most applications.
 *
 * Currently the levels range from \c 0 (fastest, least compression) to
 * \c 8 (slowest, most compression).  A value larger than \c 8 will be
 * treated as \c 8.
 *
 * This function automatically calls the following other \c _set_
 * functions with appropriate values, so the client does not need to
 * unless it specifically wants to override them:
 * - FLAC__stream_encoder_set_do_mid_side_stereo()
 * - FLAC__stream_encoder_set_loose_mid_side_stereo()
 * - FLAC__stream_encoder_set_apodization()
 * - FLAC__stream_encoder_set_max_lpc_order()
 * - FLAC__stream_encoder_set_qlp_coeff_precision()
 * - FLAC__stream_encoder_set_do_qlp_coeff_prec_search()
 * - FLAC__stream_encoder_set_do_escape_coding()
 * - FLAC__stream_encoder_set_do_exhaustive_model_search()
 * - FLAC__stream_encoder_set_min_residual_partition_order()
 * - FLAC__stream_encoder_set_max_residual_partition_order()
 * - FLAC__stream_encoder_set_rice_parameter_search_dist()
 *
 * The actual values set for each level are:
 * <table>
 * <tr>
 *  <td><b>level</b></td>
 *  <td>do mid-side stereo</td>
 *  <td>loose mid-side stereo</td>
 *  <td>apodization</td>
 *  <td>max lpc order</td>
 *  <td>qlp coeff precision</td>
 *  <td>qlp coeff prec search</td>
 *  <td>escape coding</td>
 *  <td>exhaustive model search</td>
 *  <td>min residual partition order</td>
 *  <td>max residual partition order</td>
 *  <td>rice parameter search dist</td>
 * </tr>
 * <tr>  <td><b>0</b></td> <td>false</td> <td>false</td> <td>tukey(0.5)<td>                                     <td>0</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>3</td> <td>0</td> </tr>
 * <tr>  <td><b>1</b></td> <td>true</td>  <td>true</td>  <td>tukey(0.5)<td>                                     <td>0</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>3</td> <td>0</td> </tr>
 * <tr>  <td><b>2</b></td> <td>true</td>  <td>false</td> <td>tukey(0.5)<td>                                     <td>0</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>3</td> <td>0</td> </tr>
 * <tr>  <td><b>3</b></td> <td>false</td> <td>false</td> <td>tukey(0.5)<td>                                     <td>6</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>4</td> <td>0</td> </tr>
 * <tr>  <td><b>4</b></td> <td>true</td>  <td>true</td>  <td>tukey(0.5)<td>                                     <td>8</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>4</td> <td>0</td> </tr>
 * <tr>  <td><b>5</b></td> <td>true</td>  <td>false</td> <td>tukey(0.5)<td>                                     <td>8</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>5</td> <td>0</td> </tr>
 * <tr>  <td><b>6</b></td> <td>true</td>  <td>false</td> <td>tukey(0.5);partial_tukey(2)<td>                    <td>8</td>  <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>6</td> <td>0</td> </tr>
 * <tr>  <td><b>7</b></td> <td>true</td>  <td>false</td> <td>tukey(0.5);partial_tukey(2)<td>                    <td>12</td> <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>6</td> <td>0</td> </tr>
 * <tr>  <td><b>8</b></td> <td>true</td>  <td>false</td> <td>tukey(0.5);partial_tukey(2);punchout_tukey(3)</td> <td>12</td> <td>0</td> <td>false</td> <td>false</td> <td>false</td> <td>0</td> <td>6</td> <td>0</td> </tr>
 * </table>
 *
 * \default \c 5
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_compression_level(FLAC__StreamEncoder *encoder, unsigned value);

/** Set the blocksize to use while encoding.
 *
 * The number of samples to use per frame.  Use \c 0 to let the encoder
 * estimate a blocksize; this is usually best.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_blocksize(FLAC__StreamEncoder *encoder, unsigned value);

/** Set to \c true to enable mid-side encoding on stereo input.  The
 *  number of channels must be 2 for this to have any effect.  Set to
 *  \c false to use only independent channel coding.
 *
 * \default \c false
 * \param  encoder  An encoder instance to set.
 * \param  value    Flag value (see above).
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_do_mid_side_stereo(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Set to \c true to enable adaptive switching between mid-side and
 *  left-right encoding on stereo input.  Set to \c false to use
 *  exhaustive searching.  Setting this to \c true requires
 *  FLAC__stream_encoder_set_do_mid_side_stereo() to also be set to
 *  \c true in order to have any effect.
 *
 * \default \c false
 * \param  encoder  An encoder instance to set.
 * \param  value    Flag value (see above).
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_loose_mid_side_stereo(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Sets the apodization function(s) the encoder will use when windowing
 *  audio data for LPC analysis.
 *
 * The \a specification is a plain ASCII string which specifies exactly
 * which functions to use.  There may be more than one (up to 32),
 * separated by \c ';' characters.  Some functions take one or more
 * comma-separated arguments in parentheses.
 *
 * The available functions are \c bartlett, \c bartlett_hann,
 * \c blackman, \c blackman_harris_4term_92db, \c connes, \c flattop,
 * \c gauss(STDDEV), \c hamming, \c hann, \c kaiser_bessel, \c nuttall,
 * \c rectangle, \c triangle, \c tukey(P), \c partial_tukey(n[/ov[/P]]),
 * \c punchout_tukey(n[/ov[/P]]), \c welch.
 *
 * For \c gauss(STDDEV), STDDEV specifies the standard deviation
 * (0<STDDEV<=0.5).
 *
 * For \c tukey(P), P specifies the fraction of the window that is
 * tapered (0<=P<=1).  P=0 corresponds to \c rectangle and P=1
 * corresponds to \c hann.
 *
 * Specifying \c partial_tukey or \c punchout_tukey works a little
 * different. These do not specify a single apodization function, but
 * a series of them with some overlap. partial_tukey specifies a series
 * of small windows (all treated separately) while punchout_tukey
 * specifies a series of windows that have a hole in them. In this way,
 * the predictor is constructed with only a part of the block, which
 * helps in case a block consists of dissimilar parts.
 *
 * The three parameters that can be specified for the functions are
 * n, ov and P. n is the number of functions to add, ov is the overlap
 * of the windows in case of partial_tukey and the overlap in the gaps
 * in case of punchout_tukey. P is the fraction of the window that is
 * tapered, like with a regular tukey window. The function can be
 * specified with only a number, a number and an overlap, or a number
 * an overlap and a P, for example, partial_tukey(3), partial_tukey(3/0.3)
 * and partial_tukey(3/0.3/0.5) are all valid. ov should be smaller than 1
 * and can be negative.
 *
 * Example specifications are \c "blackman" or
 * \c "hann;triangle;tukey(0.5);tukey(0.25);tukey(0.125)"
 *
 * Any function that is specified erroneously is silently dropped.  Up
 * to 32 functions are kept, the rest are dropped.  If the specification
 * is empty the encoder defaults to \c "tukey(0.5)".
 *
 * When more than one function is specified, then for every subframe the
 * encoder will try each of them separately and choose the window that
 * results in the smallest compressed subframe.
 *
 * Note that each function specified causes the encoder to occupy a
 * floating point array in which to store the window. Also note that the
 * values of P, STDDEV and ov are locale-specific, so if the comma
 * separator specified by the locale is a comma, a comma should be used.
 *
 * \default \c "tukey(0.5)"
 * \param  encoder        An encoder instance to set.
 * \param  specification  See above.
 * \assert
 *    \code encoder != NULL \endcode
 *    \code specification != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_apodization(FLAC__StreamEncoder *encoder, const char *specification);

/** Set the maximum LPC order, or \c 0 to use only the fixed predictors.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_max_lpc_order(FLAC__StreamEncoder *encoder, unsigned value);

/** Set the precision, in bits, of the quantized linear predictor
 *  coefficients, or \c 0 to let the encoder select it based on the
 *  blocksize.
 *
 * \note
 * In the current implementation, qlp_coeff_precision + bits_per_sample must
 * be less than 32.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_qlp_coeff_precision(FLAC__StreamEncoder *encoder, unsigned value);

/** Set to \c false to use only the specified quantized linear predictor
 *  coefficient precision, or \c true to search neighboring precision
 *  values and use the best one.
 *
 * \default \c false
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_do_qlp_coeff_prec_search(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Deprecated.  Setting this value has no effect.
 *
 * \default \c false
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_do_escape_coding(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Set to \c false to let the encoder estimate the best model order
 *  based on the residual signal energy, or \c true to force the
 *  encoder to evaluate all order models and select the best.
 *
 * \default \c false
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_do_exhaustive_model_search(FLAC__StreamEncoder *encoder, FLAC__bool value);

/** Set the minimum partition order to search when coding the residual.
 *  This is used in tandem with
 *  FLAC__stream_encoder_set_max_residual_partition_order().
 *
 *  The partition order determines the context size in the residual.
 *  The context size will be approximately <tt>blocksize / (2 ^ order)</tt>.
 *
 *  Set both min and max values to \c 0 to force a single context,
 *  whose Rice parameter is based on the residual signal variance.
 *  Otherwise, set a min and max order, and the encoder will search
 *  all orders, using the mean of each context for its Rice parameter,
 *  and use the best.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_min_residual_partition_order(FLAC__StreamEncoder *encoder, unsigned value);

/** Set the maximum partition order to search when coding the residual.
 *  This is used in tandem with
 *  FLAC__stream_encoder_set_min_residual_partition_order().
 *
 *  The partition order determines the context size in the residual.
 *  The context size will be approximately <tt>blocksize / (2 ^ order)</tt>.
 *
 *  Set both min and max values to \c 0 to force a single context,
 *  whose Rice parameter is based on the residual signal variance.
 *  Otherwise, set a min and max order, and the encoder will search
 *  all orders, using the mean of each context for its Rice parameter,
 *  and use the best.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_max_residual_partition_order(FLAC__StreamEncoder *encoder, unsigned value);

/** Deprecated.  Setting this value has no effect.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_rice_parameter_search_dist(FLAC__StreamEncoder *encoder, unsigned value);

/** Set an estimate of the total samples that will be encoded.
 *  This is merely an estimate and may be set to \c 0 if unknown.
 *  This value will be written to the STREAMINFO block before encoding,
 *  and can remove the need for the caller to rewrite the value later
 *  if the value is known before encoding.
 *
 * \default \c 0
 * \param  encoder  An encoder instance to set.
 * \param  value    See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_total_samples_estimate(FLAC__StreamEncoder *encoder, FLAC__uint64 value);

/** Set the metadata blocks to be emitted to the stream before encoding.
 *  A value of \c NULL, \c 0 implies no metadata; otherwise, supply an
 *  array of pointers to metadata blocks.  The array is non-const since
 *  the encoder may need to change the \a is_last flag inside them, and
 *  in some cases update seek point offsets.  Otherwise, the encoder will
 *  not modify or free the blocks.  It is up to the caller to free the
 *  metadata blocks after encoding finishes.
 *
 * \note
 * The encoder stores only copies of the pointers in the \a metadata array;
 * the metadata blocks themselves must survive at least until after
 * FLAC__stream_encoder_finish() returns.  Do not free the blocks until then.
 *
 * \note
 * The STREAMINFO block is always written and no STREAMINFO block may
 * occur in the supplied array.
 *
 * \note
 * By default the encoder does not create a SEEKTABLE.  If one is supplied
 * in the \a metadata array, but the client has specified that it does not
 * support seeking, then the SEEKTABLE will be written verbatim.  However
 * by itself this is not very useful as the client will not know the stream
 * offsets for the seekpoints ahead of time.  In order to get a proper
 * seektable the client must support seeking.  See next note.
 *
 * \note
 * SEEKTABLE blocks are handled specially.  Since you will not know
 * the values for the seek point stream offsets, you should pass in
 * a SEEKTABLE 'template', that is, a SEEKTABLE object with the
 * required sample numbers (or placeholder points), with \c 0 for the
 * \a frame_samples and \a stream_offset fields for each point.  If the
 * client has specified that it supports seeking by providing a seek
 * callback to FLAC__stream_encoder_init_stream() or both seek AND read
 * callback to FLAC__stream_encoder_init_ogg_stream() (or by using
 * FLAC__stream_encoder_init*_file() or FLAC__stream_encoder_init*_FILE()),
 * then while it is encoding the encoder will fill the stream offsets in
 * for you and when encoding is finished, it will seek back and write the
 * real values into the SEEKTABLE block in the stream.  There are helper
 * routines for manipulating seektable template blocks; see metadata.h:
 * FLAC__metadata_object_seektable_template_*().  If the client does
 * not support seeking, the SEEKTABLE will have inaccurate offsets which
 * will slow down or remove the ability to seek in the FLAC stream.
 *
 * \note
 * The encoder instance \b will modify the first \c SEEKTABLE block
 * as it transforms the template to a valid seektable while encoding,
 * but it is still up to the caller to free all metadata blocks after
 * encoding.
 *
 * \note
 * A VORBIS_COMMENT block may be supplied.  The vendor string in it
 * will be ignored.  libFLAC will use it's own vendor string. libFLAC
 * will not modify the passed-in VORBIS_COMMENT's vendor string, it
 * will simply write it's own into the stream.  If no VORBIS_COMMENT
 * block is present in the \a metadata array, libFLAC will write an
 * empty one, containing only the vendor string.
 *
 * \note The Ogg FLAC mapping requires that the VORBIS_COMMENT block be
 * the second metadata block of the stream.  The encoder already supplies
 * the STREAMINFO block automatically.  If \a metadata does not contain a
 * VORBIS_COMMENT block, the encoder will supply that too.  Otherwise, if
 * \a metadata does contain a VORBIS_COMMENT block and it is not the
 * first, the init function will reorder \a metadata by moving the
 * VORBIS_COMMENT block to the front; the relative ordering of the other
 * blocks will remain as they were.
 *
 * \note The Ogg FLAC mapping limits the number of metadata blocks per
 * stream to \c 65535.  If \a num_blocks exceeds this the function will
 * return \c false.
 *
 * \default \c NULL, 0
 * \param  encoder     An encoder instance to set.
 * \param  metadata    See above.
 * \param  num_blocks  See above.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if the encoder is already initialized, else \c true.
 *    \c false if the encoder is already initialized, or if
 *    \a num_blocks > 65535 if encoding to Ogg FLAC, else \c true.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_set_metadata(FLAC__StreamEncoder *encoder, FLAC__StreamMetadata **metadata, unsigned num_blocks);

/** Get the current encoder state.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__StreamEncoderState
 *    The current encoder state.
 */
FLAC_API FLAC__StreamEncoderState FLAC__stream_encoder_get_state(const FLAC__StreamEncoder *encoder);

/** Get the state of the verify stream decoder.
 *  Useful when the stream encoder state is
 *  \c FLAC__STREAM_ENCODER_VERIFY_DECODER_ERROR.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__StreamDecoderState
 *    The verify stream decoder state.
 */
FLAC_API FLAC__StreamDecoderState FLAC__stream_encoder_get_verify_decoder_state(const FLAC__StreamEncoder *encoder);

/** Get the current encoder state as a C string.
 *  This version automatically resolves
 *  \c FLAC__STREAM_ENCODER_VERIFY_DECODER_ERROR by getting the
 *  verify decoder's state.
 *
 * \param  encoder  A encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval const char *
 *    The encoder state as a C string.  Do not modify the contents.
 */
FLAC_API const char *FLAC__stream_encoder_get_resolved_state_string(const FLAC__StreamEncoder *encoder);

/** Get relevant values about the nature of a verify decoder error.
 *  Useful when the stream encoder state is
 *  \c FLAC__STREAM_ENCODER_VERIFY_DECODER_ERROR.  The arguments should
 *  be addresses in which the stats will be returned, or NULL if value
 *  is not desired.
 *
 * \param  encoder  An encoder instance to query.
 * \param  absolute_sample  The absolute sample number of the mismatch.
 * \param  frame_number  The number of the frame in which the mismatch occurred.
 * \param  channel       The channel in which the mismatch occurred.
 * \param  sample        The number of the sample (relative to the frame) in
 *                       which the mismatch occurred.
 * \param  expected      The expected value for the sample in question.
 * \param  got           The actual value returned by the decoder.
 * \assert
 *    \code encoder != NULL \endcode
 */
FLAC_API void FLAC__stream_encoder_get_verify_decoder_error_stats(const FLAC__StreamEncoder *encoder, FLAC__uint64 *absolute_sample, unsigned *frame_number, unsigned *channel, unsigned *sample, FLAC__int32 *expected, FLAC__int32 *got);

/** Get the "verify" flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_set_verify().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_verify(const FLAC__StreamEncoder *encoder);

/** Get the <A HREF="../format.html#subset>Subset</A> flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_set_streamable_subset().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_streamable_subset(const FLAC__StreamEncoder *encoder);

/** Get the number of input channels being processed.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_channels().
 */
FLAC_API unsigned FLAC__stream_encoder_get_channels(const FLAC__StreamEncoder *encoder);

/** Get the input sample resolution setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_bits_per_sample().
 */
FLAC_API unsigned FLAC__stream_encoder_get_bits_per_sample(const FLAC__StreamEncoder *encoder);

/** Get the input sample rate setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_sample_rate().
 */
FLAC_API unsigned FLAC__stream_encoder_get_sample_rate(const FLAC__StreamEncoder *encoder);

/** Get the blocksize setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_blocksize().
 */
FLAC_API unsigned FLAC__stream_encoder_get_blocksize(const FLAC__StreamEncoder *encoder);

/** Get the "mid/side stereo coding" flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_get_do_mid_side_stereo().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_do_mid_side_stereo(const FLAC__StreamEncoder *encoder);

/** Get the "adaptive mid/side switching" flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_set_loose_mid_side_stereo().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_loose_mid_side_stereo(const FLAC__StreamEncoder *encoder);

/** Get the maximum LPC order setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_max_lpc_order().
 */
FLAC_API unsigned FLAC__stream_encoder_get_max_lpc_order(const FLAC__StreamEncoder *encoder);

/** Get the quantized linear predictor coefficient precision setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_qlp_coeff_precision().
 */
FLAC_API unsigned FLAC__stream_encoder_get_qlp_coeff_precision(const FLAC__StreamEncoder *encoder);

/** Get the qlp coefficient precision search flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_set_do_qlp_coeff_prec_search().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_do_qlp_coeff_prec_search(const FLAC__StreamEncoder *encoder);

/** Get the "escape coding" flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_set_do_escape_coding().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_do_escape_coding(const FLAC__StreamEncoder *encoder);

/** Get the exhaustive model search flag.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    See FLAC__stream_encoder_set_do_exhaustive_model_search().
 */
FLAC_API FLAC__bool FLAC__stream_encoder_get_do_exhaustive_model_search(const FLAC__StreamEncoder *encoder);

/** Get the minimum residual partition order setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_min_residual_partition_order().
 */
FLAC_API unsigned FLAC__stream_encoder_get_min_residual_partition_order(const FLAC__StreamEncoder *encoder);

/** Get maximum residual partition order setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_max_residual_partition_order().
 */
FLAC_API unsigned FLAC__stream_encoder_get_max_residual_partition_order(const FLAC__StreamEncoder *encoder);

/** Get the Rice parameter search distance setting.
 *
 * \param  encoder  An encoder instance to query.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval unsigned
 *    See FLAC__stream_encoder_set_rice_parameter_search_dist().
 */
FLAC_API unsigned FLAC__stream_encoder_get_rice_parameter_search_dist(const FLAC__StreamEncoder *encoder);

/** Get the previously set estimate of the total samples to be encoded.
 *  The encoder merely mimics back the value given to
 *  FLAC__stream_encoder_set_total_samples_estimate() since it has no
 *  other way of knowing how many samples the client will encode.
 *
 * \param  encoder  An encoder instance to set.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__uint64
 *    See FLAC__stream_encoder_get_total_samples_estimate().
 */
FLAC_API FLAC__uint64 FLAC__stream_encoder_get_total_samples_estimate(const FLAC__StreamEncoder *encoder);

/** Initialize the encoder instance to encode native FLAC streams.
 *
 *  This flavor of initialization sets up the encoder to encode to a
 *  native FLAC stream. I/O is performed via callbacks to the client.
 *  For encoding to a plain file via filename or open \c FILE*,
 *  FLAC__stream_encoder_init_file() and FLAC__stream_encoder_init_FILE()
 *  provide a simpler interface.
 *
 *  This function should be called after FLAC__stream_encoder_new() and
 *  FLAC__stream_encoder_set_*() but before FLAC__stream_encoder_process()
 *  or FLAC__stream_encoder_process_interleaved().
 *  initialization succeeded.
 *
 *  The call to FLAC__stream_encoder_init_stream() currently will also
 *  immediately call the write callback several times, once with the \c fLaC
 *  signature, and once for each encoded metadata block.
 *
 * \param  encoder            An uninitialized encoder instance.
 * \param  write_callback     See FLAC__StreamEncoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  seek_callback      See FLAC__StreamEncoderSeekCallback.  This
 *                            pointer may be \c NULL if seeking is not
 *                            supported.  The encoder uses seeking to go back
 *                            and write some some stream statistics to the
 *                            STREAMINFO block; this is recommended but not
 *                            necessary to create a valid FLAC stream.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a tell_callback must also be supplied.
 *                            Alternatively, a dummy seek callback that just
 *                            returns \c FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the encoder.
 * \param  tell_callback      See FLAC__StreamEncoderTellCallback.  This
 *                            pointer may be \c NULL if seeking is not
 *                            supported.  If \a seek_callback is \c NULL then
 *                            this argument will be ignored.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a tell_callback must also be supplied.
 *                            Alternatively, a dummy tell callback that just
 *                            returns \c FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the encoder.
 * \param  metadata_callback  See FLAC__StreamEncoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.  If the client provides a seek callback,
 *                            this function is not necessary as the encoder
 *                            will automatically seek back and update the
 *                            STREAMINFO block.  It may also be \c NULL if the
 *                            client does not support seeking, since it will
 *                            have no way of going back to update the
 *                            STREAMINFO.  However the client can still supply
 *                            a callback if it would like to know the details
 *                            from the STREAMINFO.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__StreamEncoderInitStatus
 *    \c FLAC__STREAM_ENCODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamEncoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamEncoderInitStatus FLAC__stream_encoder_init_stream(FLAC__StreamEncoder *encoder, FLAC__StreamEncoderWriteCallback write_callback, FLAC__StreamEncoderSeekCallback seek_callback, FLAC__StreamEncoderTellCallback tell_callback, FLAC__StreamEncoderMetadataCallback metadata_callback, void *client_data);

/** Initialize the encoder instance to encode Ogg FLAC streams.
 *
 *  This flavor of initialization sets up the encoder to encode to a FLAC
 *  stream in an Ogg container.  I/O is performed via callbacks to the
 *  client.  For encoding to a plain file via filename or open \c FILE*,
 *  FLAC__stream_encoder_init_ogg_file() and FLAC__stream_encoder_init_ogg_FILE()
 *  provide a simpler interface.
 *
 *  This function should be called after FLAC__stream_encoder_new() and
 *  FLAC__stream_encoder_set_*() but before FLAC__stream_encoder_process()
 *  or FLAC__stream_encoder_process_interleaved().
 *  initialization succeeded.
 *
 *  The call to FLAC__stream_encoder_init_ogg_stream() currently will also
 *  immediately call the write callback several times to write the metadata
 *  packets.
 *
 * \param  encoder            An uninitialized encoder instance.
 * \param  read_callback      See FLAC__StreamEncoderReadCallback.  This
 *                            pointer must not be \c NULL if \a seek_callback
 *                            is non-NULL since they are both needed to be
 *                            able to write data back to the Ogg FLAC stream
 *                            in the post-encode phase.
 * \param  write_callback     See FLAC__StreamEncoderWriteCallback.  This
 *                            pointer must not be \c NULL.
 * \param  seek_callback      See FLAC__StreamEncoderSeekCallback.  This
 *                            pointer may be \c NULL if seeking is not
 *                            supported.  The encoder uses seeking to go back
 *                            and write some some stream statistics to the
 *                            STREAMINFO block; this is recommended but not
 *                            necessary to create a valid FLAC stream.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a tell_callback must also be supplied.
 *                            Alternatively, a dummy seek callback that just
 *                            returns \c FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the encoder.
 * \param  tell_callback      See FLAC__StreamEncoderTellCallback.  This
 *                            pointer may be \c NULL if seeking is not
 *                            supported.  If \a seek_callback is \c NULL then
 *                            this argument will be ignored.  If
 *                            \a seek_callback is not \c NULL then a
 *                            \a tell_callback must also be supplied.
 *                            Alternatively, a dummy tell callback that just
 *                            returns \c FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED
 *                            may also be supplied, all though this is slightly
 *                            less efficient for the encoder.
 * \param  metadata_callback  See FLAC__StreamEncoderMetadataCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.  If the client provides a seek callback,
 *                            this function is not necessary as the encoder
 *                            will automatically seek back and update the
 *                            STREAMINFO block.  It may also be \c NULL if the
 *                            client does not support seeking, since it will
 *                            have no way of going back to update the
 *                            STREAMINFO.  However the client can still supply
 *                            a callback if it would like to know the details
 *                            from the STREAMINFO.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__StreamEncoderInitStatus
 *    \c FLAC__STREAM_ENCODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamEncoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamEncoderInitStatus FLAC__stream_encoder_init_ogg_stream(FLAC__StreamEncoder *encoder, FLAC__StreamEncoderReadCallback read_callback, FLAC__StreamEncoderWriteCallback write_callback, FLAC__StreamEncoderSeekCallback seek_callback, FLAC__StreamEncoderTellCallback tell_callback, FLAC__StreamEncoderMetadataCallback metadata_callback, void *client_data);

/** Initialize the encoder instance to encode native FLAC files.
 *
 *  This flavor of initialization sets up the encoder to encode to a
 *  plain native FLAC file.  For non-stdio streams, you must use
 *  FLAC__stream_encoder_init_stream() and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_encoder_new() and
 *  FLAC__stream_encoder_set_*() but before FLAC__stream_encoder_process()
 *  or FLAC__stream_encoder_process_interleaved().
 *  initialization succeeded.
 *
 * \param  encoder            An uninitialized encoder instance.
 * \param  file               An open file.  The file should have been opened
 *                            with mode \c "w+b" and rewound.  The file
 *                            becomes owned by the encoder and should not be
 *                            manipulated by the client while encoding.
 *                            Unless \a file is \c stdout, it will be closed
 *                            when FLAC__stream_encoder_finish() is called.
 *                            Note however that a proper SEEKTABLE cannot be
 *                            created when encoding to \c stdout since it is
 *                            not seekable.
 * \param  progress_callback  See FLAC__StreamEncoderProgressCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code encoder != NULL \endcode
 *    \code file != NULL \endcode
 * \retval FLAC__StreamEncoderInitStatus
 *    \c FLAC__STREAM_ENCODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamEncoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamEncoderInitStatus FLAC__stream_encoder_init_FILE(FLAC__StreamEncoder *encoder, FILE *file, FLAC__StreamEncoderProgressCallback progress_callback, void *client_data);

/** Initialize the encoder instance to encode Ogg FLAC files.
 *
 *  This flavor of initialization sets up the encoder to encode to a
 *  plain Ogg FLAC file.  For non-stdio streams, you must use
 *  FLAC__stream_encoder_init_ogg_stream() and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_encoder_new() and
 *  FLAC__stream_encoder_set_*() but before FLAC__stream_encoder_process()
 *  or FLAC__stream_encoder_process_interleaved().
 *  initialization succeeded.
 *
 * \param  encoder            An uninitialized encoder instance.
 * \param  file               An open file.  The file should have been opened
 *                            with mode \c "w+b" and rewound.  The file
 *                            becomes owned by the encoder and should not be
 *                            manipulated by the client while encoding.
 *                            Unless \a file is \c stdout, it will be closed
 *                            when FLAC__stream_encoder_finish() is called.
 *                            Note however that a proper SEEKTABLE cannot be
 *                            created when encoding to \c stdout since it is
 *                            not seekable.
 * \param  progress_callback  See FLAC__StreamEncoderProgressCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code encoder != NULL \endcode
 *    \code file != NULL \endcode
 * \retval FLAC__StreamEncoderInitStatus
 *    \c FLAC__STREAM_ENCODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamEncoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamEncoderInitStatus FLAC__stream_encoder_init_ogg_FILE(FLAC__StreamEncoder *encoder, FILE *file, FLAC__StreamEncoderProgressCallback progress_callback, void *client_data);

/** Initialize the encoder instance to encode native FLAC files.
 *
 *  This flavor of initialization sets up the encoder to encode to a plain
 *  FLAC file.  If POSIX fopen() semantics are not sufficient (for example,
 *  with Unicode filenames on Windows), you must use
 *  FLAC__stream_encoder_init_FILE(), or FLAC__stream_encoder_init_stream()
 *  and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_encoder_new() and
 *  FLAC__stream_encoder_set_*() but before FLAC__stream_encoder_process()
 *  or FLAC__stream_encoder_process_interleaved().
 *  initialization succeeded.
 *
 * \param  encoder            An uninitialized encoder instance.
 * \param  filename           The name of the file to encode to.  The file will
 *                            be opened with fopen().  Use \c NULL to encode to
 *                            \c stdout.  Note however that a proper SEEKTABLE
 *                            cannot be created when encoding to \c stdout since
 *                            it is not seekable.
 * \param  progress_callback  See FLAC__StreamEncoderProgressCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__StreamEncoderInitStatus
 *    \c FLAC__STREAM_ENCODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamEncoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamEncoderInitStatus FLAC__stream_encoder_init_file(FLAC__StreamEncoder *encoder, const char *filename, FLAC__StreamEncoderProgressCallback progress_callback, void *client_data);

/** Initialize the encoder instance to encode Ogg FLAC files.
 *
 *  This flavor of initialization sets up the encoder to encode to a plain
 *  Ogg FLAC file.  If POSIX fopen() semantics are not sufficient (for example,
 *  with Unicode filenames on Windows), you must use
 *  FLAC__stream_encoder_init_ogg_FILE(), or FLAC__stream_encoder_init_ogg_stream()
 *  and provide callbacks for the I/O.
 *
 *  This function should be called after FLAC__stream_encoder_new() and
 *  FLAC__stream_encoder_set_*() but before FLAC__stream_encoder_process()
 *  or FLAC__stream_encoder_process_interleaved().
 *  initialization succeeded.
 *
 * \param  encoder            An uninitialized encoder instance.
 * \param  filename           The name of the file to encode to.  The file will
 *                            be opened with fopen().  Use \c NULL to encode to
 *                            \c stdout.  Note however that a proper SEEKTABLE
 *                            cannot be created when encoding to \c stdout since
 *                            it is not seekable.
 * \param  progress_callback  See FLAC__StreamEncoderProgressCallback.  This
 *                            pointer may be \c NULL if the callback is not
 *                            desired.
 * \param  client_data        This value will be supplied to callbacks in their
 *                            \a client_data argument.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__StreamEncoderInitStatus
 *    \c FLAC__STREAM_ENCODER_INIT_STATUS_OK if initialization was successful;
 *    see FLAC__StreamEncoderInitStatus for the meanings of other return values.
 */
FLAC_API FLAC__StreamEncoderInitStatus FLAC__stream_encoder_init_ogg_file(FLAC__StreamEncoder *encoder, const char *filename, FLAC__StreamEncoderProgressCallback progress_callback, void *client_data);

/** Finish the encoding process.
 *  Flushes the encoding buffer, releases resources, resets the encoder
 *  settings to their defaults, and returns the encoder state to
 *  FLAC__STREAM_ENCODER_UNINITIALIZED.  Note that this can generate
 *  one or more write callbacks before returning, and will generate
 *  a metadata callback.
 *
 *  Note that in the course of processing the last frame, errors can
 *  occur, so the caller should be sure to check the return value to
 *  ensure the file was encoded properly.
 *
 *  In the event of a prematurely-terminated encode, it is not strictly
 *  necessary to call this immediately before FLAC__stream_encoder_delete()
 *  but it is good practice to match every FLAC__stream_encoder_init_*()
 *  with a FLAC__stream_encoder_finish().
 *
 * \param  encoder  An uninitialized encoder instance.
 * \assert
 *    \code encoder != NULL \endcode
 * \retval FLAC__bool
 *    \c false if an error occurred processing the last frame; or if verify
 *    mode is set (see FLAC__stream_encoder_set_verify()), there was a
 *    verify mismatch; else \c true.  If \c false, caller should check the
 *    state with FLAC__stream_encoder_get_state() for more information
 *    about the error.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_finish(FLAC__StreamEncoder *encoder);

/** Submit data for encoding.
 *  This version allows you to supply the input data via an array of
 *  pointers, each pointer pointing to an array of \a samples samples
 *  representing one channel.  The samples need not be block-aligned,
 *  but each channel should have the same number of samples.  Each sample
 *  should be a signed integer, right-justified to the resolution set by
 *  FLAC__stream_encoder_set_bits_per_sample().  For example, if the
 *  resolution is 16 bits per sample, the samples should all be in the
 *  range [-32768,32767].
 *
 *  For applications where channel order is important, channels must
 *  follow the order as described in the
 *  <A HREF="../format.html#frame_header">frame header</A>.
 *
 * \param  encoder  An initialized encoder instance in the OK state.
 * \param  buffer   An array of pointers to each channel's signal.
 * \param  samples  The number of samples in one channel.
 * \assert
 *    \code encoder != NULL \endcode
 *    \code FLAC__stream_encoder_get_state(encoder) == FLAC__STREAM_ENCODER_OK \endcode
 * \retval FLAC__bool
 *    \c true if successful, else \c false; in this case, check the
 *    encoder state with FLAC__stream_encoder_get_state() to see what
 *    went wrong.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_process(FLAC__StreamEncoder *encoder, const FLAC__int32 * const buffer[], unsigned samples);

/** Submit data for encoding.
 *  This version allows you to supply the input data where the channels
 *  are interleaved into a single array (i.e. channel0_sample0,
 *  channel1_sample0, ... , channelN_sample0, channel0_sample1, ...).
 *  The samples need not be block-aligned but they must be
 *  sample-aligned, i.e. the first value should be channel0_sample0
 *  and the last value channelN_sampleM.  Each sample should be a signed
 *  integer, right-justified to the resolution set by
 *  FLAC__stream_encoder_set_bits_per_sample().  For example, if the
 *  resolution is 16 bits per sample, the samples should all be in the
 *  range [-32768,32767].
 *
 *  For applications where channel order is important, channels must
 *  follow the order as described in the
 *  <A HREF="../format.html#frame_header">frame header</A>.
 *
 * \param  encoder  An initialized encoder instance in the OK state.
 * \param  buffer   An array of channel-interleaved data (see above).
 * \param  samples  The number of samples in one channel, the same as for
 *                  FLAC__stream_encoder_process().  For example, if
 *                  encoding two channels, \c 1000 \a samples corresponds
 *                  to a \a buffer of 2000 values.
 * \assert
 *    \code encoder != NULL \endcode
 *    \code FLAC__stream_encoder_get_state(encoder) == FLAC__STREAM_ENCODER_OK \endcode
 * \retval FLAC__bool
 *    \c true if successful, else \c false; in this case, check the
 *    encoder state with FLAC__stream_encoder_get_state() to see what
 *    went wrong.
 */
FLAC_API FLAC__bool FLAC__stream_encoder_process_interleaved(FLAC__StreamEncoder *encoder, const FLAC__int32 buffer[], unsigned samples);

/* \} */

#ifdef __cplusplus
}
#endif

#endif
/* (C) COPYRIGHT 1994-2002 Xiph.Org Foundation */
/* Modified by Jean-Marc Valin */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/* opus_types.h based on ogg_types.h from libogg */

/**
   @file opus_types.h
   @brief Opus reference implementation types
*/
#ifndef OPUS_TYPES_H
#define OPUS_TYPES_H

/* Use the real stdint.h if it's there (taken from Paul Hsieh's pstdint.h) */
#if (defined(__STDC__) && __STDC__ && __STDC_VERSION__ >= 199901L) || (defined(__GNUC__) && (defined(_STDINT_H) || defined(_STDINT_H_)) || defined (HAVE_STDINT_H))
#include <stdint.h>

   typedef int16_t opus_int16;
   typedef uint16_t opus_uint16;
   typedef int32_t opus_int32;
   typedef uint32_t opus_uint32;
#elif defined(_WIN32)

#  if defined(__CYGWIN__)
#    include <_G_config.h>
     typedef _G_int32_t opus_int32;
     typedef _G_uint32_t opus_uint32;
     typedef _G_int16 opus_int16;
     typedef _G_uint16 opus_uint16;
#  elif defined(__MINGW32__)
     typedef short opus_int16;
     typedef unsigned short opus_uint16;
     typedef int opus_int32;
     typedef unsigned int opus_uint32;
#  elif defined(__MWERKS__)
     typedef int opus_int32;
     typedef unsigned int opus_uint32;
     typedef short opus_int16;
     typedef unsigned short opus_uint16;
#  else
     /* MSVC/Borland */
     typedef __int32 opus_int32;
     typedef unsigned __int32 opus_uint32;
     typedef __int16 opus_int16;
     typedef unsigned __int16 opus_uint16;
#  endif

#elif defined(__MACOS__)

#  include <sys/types.h>
   typedef SInt16 opus_int16;
   typedef UInt16 opus_uint16;
   typedef SInt32 opus_int32;
   typedef UInt32 opus_uint32;

#elif (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */

#  include <sys/types.h>
   typedef int16_t opus_int16;
   typedef u_int16_t opus_uint16;
   typedef int32_t opus_int32;
   typedef u_int32_t opus_uint32;

#elif defined(__BEOS__)

   /* Be */
#  include <inttypes.h>
   typedef int16 opus_int16;
   typedef u_int16 opus_uint16;
   typedef int32_t opus_int32;
   typedef u_int32_t opus_uint32;

#elif defined (__EMX__)

   /* OS/2 GCC */
   typedef short opus_int16;
   typedef unsigned short opus_uint16;
   typedef int opus_int32;
   typedef unsigned int opus_uint32;

#elif defined (DJGPP)

   /* DJGPP */
   typedef short opus_int16;
   typedef unsigned short opus_uint16;
   typedef int opus_int32;
   typedef unsigned int opus_uint32;

#elif defined(R5900)

   /* PS2 EE */
   typedef int opus_int32;
   typedef unsigned opus_uint32;
   typedef short opus_int16;
   typedef unsigned short opus_uint16;

#elif defined(__SYMBIAN32__)

   /* Symbian GCC */
   typedef signed short opus_int16;
   typedef unsigned short opus_uint16;
   typedef signed int opus_int32;
   typedef unsigned int opus_uint32;

#elif defined(CONFIG_TI_C54X) || defined (CONFIG_TI_C55X)

   typedef short opus_int16;
   typedef unsigned short opus_uint16;
   typedef long opus_int32;
   typedef unsigned long opus_uint32;

#elif defined(CONFIG_TI_C6X)

   typedef short opus_int16;
   typedef unsigned short opus_uint16;
   typedef int opus_int32;
   typedef unsigned int opus_uint32;

#else

   /* Give up, take a reasonable guess */
   typedef short opus_int16;
   typedef unsigned short opus_uint16;
   typedef int opus_int32;
   typedef unsigned int opus_uint32;

#endif

#define opus_int         int                     /* used for counters etc; at least 16 bits */
#define opus_int64       long long
#define opus_int8        signed char

#define opus_uint        unsigned int            /* used for counters etc; at least 16 bits */
#define opus_uint64      unsigned long long
#define opus_uint8       unsigned char

#endif  /* OPUS_TYPES_H */
/* Copyright (c) 2010-2011 Xiph.Org Foundation, Skype Limited
   Written by Jean-Marc Valin and Koen Vos */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file opus_defines.h
 * @brief Opus reference implementation constants
 */

#ifndef OPUS_DEFINES_H
#define OPUS_DEFINES_H


#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup opus_errorcodes Error codes
 * @{
 */
/** No error @hideinitializer*/
#define OPUS_OK                0
/** One or more invalid/out of range arguments @hideinitializer*/
#define OPUS_BAD_ARG          -1
/** The mode struct passed is invalid @hideinitializer*/
#define OPUS_BUFFER_TOO_SMALL -2
/** An internal error was detected @hideinitializer*/
#define OPUS_INTERNAL_ERROR   -3
/** The compressed data passed is corrupted @hideinitializer*/
#define OPUS_INVALID_PACKET   -4
/** Invalid/unsupported request number @hideinitializer*/
#define OPUS_UNIMPLEMENTED    -5
/** An encoder or decoder structure is invalid or already freed @hideinitializer*/
#define OPUS_INVALID_STATE    -6
/** Memory allocation has failed @hideinitializer*/
#define OPUS_ALLOC_FAIL       -7
/**@}*/

/** @cond OPUS_INTERNAL_DOC */
/**Export control for opus functions */

#ifndef OPUS_EXPORT
# if defined(WIN32)
#  ifdef OPUS_BUILD
#   define OPUS_EXPORT __declspec(dllexport)
#  else
#   define OPUS_EXPORT
#  endif
# elif defined(__GNUC__) && defined(OPUS_BUILD)
#  define OPUS_EXPORT __attribute__ ((visibility ("default")))
# else
#  define OPUS_EXPORT
# endif
#endif

# if !defined(OPUS_GNUC_PREREQ)
#  if defined(__GNUC__)&&defined(__GNUC_MINOR__)
#   define OPUS_GNUC_PREREQ(_maj,_min) \
 ((__GNUC__<<16)+__GNUC_MINOR__>=((_maj)<<16)+(_min))
#  else
#   define OPUS_GNUC_PREREQ(_maj,_min) 0
#  endif
# endif

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L) )
# if OPUS_GNUC_PREREQ(3,0)
#  define OPUS_RESTRICT __restrict__
# elif (defined(_MSC_VER) && _MSC_VER >= 1400)
#  define OPUS_RESTRICT __restrict
# else
#  define OPUS_RESTRICT
# endif
#else
# define OPUS_RESTRICT restrict
#endif

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L) )
# if OPUS_GNUC_PREREQ(2,7)
#  define OPUS_INLINE __inline__
# elif (defined(_MSC_VER))
#  define OPUS_INLINE __inline
# else
#  define OPUS_INLINE
# endif
#else
# define OPUS_INLINE inline
#endif

/**Warning attributes for opus functions
  * NONNULL is not used in OPUS_BUILD to avoid the compiler optimizing out
  * some paranoid null checks. */
#if defined(__GNUC__) && OPUS_GNUC_PREREQ(3, 4)
# define OPUS_WARN_UNUSED_RESULT __attribute__ ((__warn_unused_result__))
#else
# define OPUS_WARN_UNUSED_RESULT
#endif
#if !defined(OPUS_BUILD) && defined(__GNUC__) && OPUS_GNUC_PREREQ(3, 4)
# define OPUS_ARG_NONNULL(_x)  __attribute__ ((__nonnull__(_x)))
#else
# define OPUS_ARG_NONNULL(_x)
#endif

/** These are the actual Encoder CTL ID numbers.
  * They should not be used directly by applications.
  * In general, SETs should be even and GETs should be odd.*/
#define OPUS_SET_APPLICATION_REQUEST         4000
#define OPUS_GET_APPLICATION_REQUEST         4001
#define OPUS_SET_BITRATE_REQUEST             4002
#define OPUS_GET_BITRATE_REQUEST             4003
#define OPUS_SET_MAX_BANDWIDTH_REQUEST       4004
#define OPUS_GET_MAX_BANDWIDTH_REQUEST       4005
#define OPUS_SET_VBR_REQUEST                 4006
#define OPUS_GET_VBR_REQUEST                 4007
#define OPUS_SET_BANDWIDTH_REQUEST           4008
#define OPUS_GET_BANDWIDTH_REQUEST           4009
#define OPUS_SET_COMPLEXITY_REQUEST          4010
#define OPUS_GET_COMPLEXITY_REQUEST          4011
#define OPUS_SET_INBAND_FEC_REQUEST          4012
#define OPUS_GET_INBAND_FEC_REQUEST          4013
#define OPUS_SET_PACKET_LOSS_PERC_REQUEST    4014
#define OPUS_GET_PACKET_LOSS_PERC_REQUEST    4015
#define OPUS_SET_DTX_REQUEST                 4016
#define OPUS_GET_DTX_REQUEST                 4017
#define OPUS_SET_VBR_CONSTRAINT_REQUEST      4020
#define OPUS_GET_VBR_CONSTRAINT_REQUEST      4021
#define OPUS_SET_FORCE_CHANNELS_REQUEST      4022
#define OPUS_GET_FORCE_CHANNELS_REQUEST      4023
#define OPUS_SET_SIGNAL_REQUEST              4024
#define OPUS_GET_SIGNAL_REQUEST              4025
#define OPUS_GET_LOOKAHEAD_REQUEST           4027
/* #define OPUS_RESET_STATE 4028 */
#define OPUS_GET_SAMPLE_RATE_REQUEST         4029
#define OPUS_GET_FINAL_RANGE_REQUEST         4031
#define OPUS_GET_PITCH_REQUEST               4033
#define OPUS_SET_GAIN_REQUEST                4034
#define OPUS_GET_GAIN_REQUEST                4045 /* Should have been 4035 */
#define OPUS_SET_LSB_DEPTH_REQUEST           4036
#define OPUS_GET_LSB_DEPTH_REQUEST           4037
#define OPUS_GET_LAST_PACKET_DURATION_REQUEST 4039
#define OPUS_SET_EXPERT_FRAME_DURATION_REQUEST 4040
#define OPUS_GET_EXPERT_FRAME_DURATION_REQUEST 4041
#define OPUS_SET_PREDICTION_DISABLED_REQUEST 4042
#define OPUS_GET_PREDICTION_DISABLED_REQUEST 4043

/* Don't use 4045, it's already taken by OPUS_GET_GAIN_REQUEST */

/* Macros to trigger compilation errors when the wrong types are provided to a CTL */
#define __opus_check_int(x) (((void)((x) == (opus_int32)0)), (opus_int32)(x))
#define __opus_check_int_ptr(ptr) ((ptr) + ((ptr) - (opus_int32*)(ptr)))
#define __opus_check_uint_ptr(ptr) ((ptr) + ((ptr) - (opus_uint32*)(ptr)))
#define __opus_check_val16_ptr(ptr) ((ptr) + ((ptr) - (opus_val16*)(ptr)))
/** @endcond */

/** @defgroup opus_ctlvalues Pre-defined values for CTL interface
  * @see opus_genericctls, opus_encoderctls
  * @{
  */
/* Values for the various encoder CTLs */
#define OPUS_AUTO                           -1000 /**<Auto/default setting @hideinitializer*/
#define OPUS_BITRATE_MAX                       -1 /**<Maximum bitrate @hideinitializer*/

/** Best for most VoIP/videoconference applications where listening quality and intelligibility matter most
 * @hideinitializer */
#define OPUS_APPLICATION_VOIP                2048
/** Best for broadcast/high-fidelity application where the decoded audio should be as close as possible to the input
 * @hideinitializer */
#define OPUS_APPLICATION_AUDIO               2049
/** Only use when lowest-achievable latency is what matters most. Voice-optimized modes cannot be used.
 * @hideinitializer */
#define OPUS_APPLICATION_RESTRICTED_LOWDELAY 2051

#define OPUS_SIGNAL_VOICE                    3001 /**< Signal being encoded is voice */
#define OPUS_SIGNAL_MUSIC                    3002 /**< Signal being encoded is music */
#define OPUS_BANDWIDTH_NARROWBAND            1101 /**< 4 kHz bandpass @hideinitializer*/
#define OPUS_BANDWIDTH_MEDIUMBAND            1102 /**< 6 kHz bandpass @hideinitializer*/
#define OPUS_BANDWIDTH_WIDEBAND              1103 /**< 8 kHz bandpass @hideinitializer*/
#define OPUS_BANDWIDTH_SUPERWIDEBAND         1104 /**<12 kHz bandpass @hideinitializer*/
#define OPUS_BANDWIDTH_FULLBAND              1105 /**<20 kHz bandpass @hideinitializer*/

#define OPUS_FRAMESIZE_ARG                   5000 /**< Select frame size from the argument (default) */
#define OPUS_FRAMESIZE_2_5_MS                5001 /**< Use 2.5 ms frames */
#define OPUS_FRAMESIZE_5_MS                  5002 /**< Use 5 ms frames */
#define OPUS_FRAMESIZE_10_MS                 5003 /**< Use 10 ms frames */
#define OPUS_FRAMESIZE_20_MS                 5004 /**< Use 20 ms frames */
#define OPUS_FRAMESIZE_40_MS                 5005 /**< Use 40 ms frames */
#define OPUS_FRAMESIZE_60_MS                 5006 /**< Use 60 ms frames */

/**@}*/


/** @defgroup opus_encoderctls Encoder related CTLs
  *
  * These are convenience macros for use with the \c opus_encode_ctl
  * interface. They are used to generate the appropriate series of
  * arguments for that call, passing the correct type, size and so
  * on as expected for each particular request.
  *
  * Some usage examples:
  *
  * @code
  * int ret;
  * ret = opus_encoder_ctl(enc_ctx, OPUS_SET_BANDWIDTH(OPUS_AUTO));
  * if (ret != OPUS_OK) return ret;
  *
  * opus_int32 rate;
  * opus_encoder_ctl(enc_ctx, OPUS_GET_BANDWIDTH(&rate));
  *
  * opus_encoder_ctl(enc_ctx, OPUS_RESET_STATE);
  * @endcode
  *
  * @see opus_genericctls, opus_encoder
  * @{
  */

/** Configures the encoder's computational complexity.
  * The supported range is 0-10 inclusive with 10 representing the highest complexity.
  * @see OPUS_GET_COMPLEXITY
  * @param[in] x <tt>opus_int32</tt>: Allowed values: 0-10, inclusive.
  *
  * @hideinitializer */
#define OPUS_SET_COMPLEXITY(x) OPUS_SET_COMPLEXITY_REQUEST, __opus_check_int(x)
/** Gets the encoder's complexity configuration.
  * @see OPUS_SET_COMPLEXITY
  * @param[out] x <tt>opus_int32 *</tt>: Returns a value in the range 0-10,
  *                                      inclusive.
  * @hideinitializer */
#define OPUS_GET_COMPLEXITY(x) OPUS_GET_COMPLEXITY_REQUEST, __opus_check_int_ptr(x)

/** Configures the bitrate in the encoder.
  * Rates from 500 to 512000 bits per second are meaningful, as well as the
  * special values #OPUS_AUTO and #OPUS_BITRATE_MAX.
  * The value #OPUS_BITRATE_MAX can be used to cause the codec to use as much
  * rate as it can, which is useful for controlling the rate by adjusting the
  * output buffer size.
  * @see OPUS_GET_BITRATE
  * @param[in] x <tt>opus_int32</tt>: Bitrate in bits per second. The default
  *                                   is determined based on the number of
  *                                   channels and the input sampling rate.
  * @hideinitializer */
#define OPUS_SET_BITRATE(x) OPUS_SET_BITRATE_REQUEST, __opus_check_int(x)
/** Gets the encoder's bitrate configuration.
  * @see OPUS_SET_BITRATE
  * @param[out] x <tt>opus_int32 *</tt>: Returns the bitrate in bits per second.
  *                                      The default is determined based on the
  *                                      number of channels and the input
  *                                      sampling rate.
  * @hideinitializer */
#define OPUS_GET_BITRATE(x) OPUS_GET_BITRATE_REQUEST, __opus_check_int_ptr(x)

/** Enables or disables variable bitrate (VBR) in the encoder.
  * The configured bitrate may not be met exactly because frames must
  * be an integer number of bytes in length.
  * @warning Only the MDCT mode of Opus can provide hard CBR behavior.
  * @see OPUS_GET_VBR
  * @see OPUS_SET_VBR_CONSTRAINT
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>0</dt><dd>Hard CBR. For LPC/hybrid modes at very low bit-rate, this can
  *               cause noticeable quality degradation.</dd>
  * <dt>1</dt><dd>VBR (default). The exact type of VBR is controlled by
  *               #OPUS_SET_VBR_CONSTRAINT.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_VBR(x) OPUS_SET_VBR_REQUEST, __opus_check_int(x)
/** Determine if variable bitrate (VBR) is enabled in the encoder.
  * @see OPUS_SET_VBR
  * @see OPUS_GET_VBR_CONSTRAINT
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>0</dt><dd>Hard CBR.</dd>
  * <dt>1</dt><dd>VBR (default). The exact type of VBR may be retrieved via
  *               #OPUS_GET_VBR_CONSTRAINT.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_VBR(x) OPUS_GET_VBR_REQUEST, __opus_check_int_ptr(x)

/** Enables or disables constrained VBR in the encoder.
  * This setting is ignored when the encoder is in CBR mode.
  * @warning Only the MDCT mode of Opus currently heeds the constraint.
  *  Speech mode ignores it completely, hybrid mode may fail to obey it
  *  if the LPC layer uses more bitrate than the constraint would have
  *  permitted.
  * @see OPUS_GET_VBR_CONSTRAINT
  * @see OPUS_SET_VBR
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>0</dt><dd>Unconstrained VBR.</dd>
  * <dt>1</dt><dd>Constrained VBR (default). This creates a maximum of one
  *               frame of buffering delay assuming a transport with a
  *               serialization speed of the nominal bitrate.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_VBR_CONSTRAINT(x) OPUS_SET_VBR_CONSTRAINT_REQUEST, __opus_check_int(x)
/** Determine if constrained VBR is enabled in the encoder.
  * @see OPUS_SET_VBR_CONSTRAINT
  * @see OPUS_GET_VBR
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>0</dt><dd>Unconstrained VBR.</dd>
  * <dt>1</dt><dd>Constrained VBR (default).</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_VBR_CONSTRAINT(x) OPUS_GET_VBR_CONSTRAINT_REQUEST, __opus_check_int_ptr(x)

/** Configures mono/stereo forcing in the encoder.
  * This can force the encoder to produce packets encoded as either mono or
  * stereo, regardless of the format of the input audio. This is useful when
  * the caller knows that the input signal is currently a mono source embedded
  * in a stereo stream.
  * @see OPUS_GET_FORCE_CHANNELS
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>#OPUS_AUTO</dt><dd>Not forced (default)</dd>
  * <dt>1</dt>         <dd>Forced mono</dd>
  * <dt>2</dt>         <dd>Forced stereo</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_FORCE_CHANNELS(x) OPUS_SET_FORCE_CHANNELS_REQUEST, __opus_check_int(x)
/** Gets the encoder's forced channel configuration.
  * @see OPUS_SET_FORCE_CHANNELS
  * @param[out] x <tt>opus_int32 *</tt>:
  * <dl>
  * <dt>#OPUS_AUTO</dt><dd>Not forced (default)</dd>
  * <dt>1</dt>         <dd>Forced mono</dd>
  * <dt>2</dt>         <dd>Forced stereo</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_FORCE_CHANNELS(x) OPUS_GET_FORCE_CHANNELS_REQUEST, __opus_check_int_ptr(x)

/** Configures the maximum bandpass that the encoder will select automatically.
  * Applications should normally use this instead of #OPUS_SET_BANDWIDTH
  * (leaving that set to the default, #OPUS_AUTO). This allows the
  * application to set an upper bound based on the type of input it is
  * providing, but still gives the encoder the freedom to reduce the bandpass
  * when the bitrate becomes too low, for better overall quality.
  * @see OPUS_GET_MAX_BANDWIDTH
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>OPUS_BANDWIDTH_NARROWBAND</dt>    <dd>4 kHz passband</dd>
  * <dt>OPUS_BANDWIDTH_MEDIUMBAND</dt>    <dd>6 kHz passband</dd>
  * <dt>OPUS_BANDWIDTH_WIDEBAND</dt>      <dd>8 kHz passband</dd>
  * <dt>OPUS_BANDWIDTH_SUPERWIDEBAND</dt><dd>12 kHz passband</dd>
  * <dt>OPUS_BANDWIDTH_FULLBAND</dt>     <dd>20 kHz passband (default)</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_MAX_BANDWIDTH(x) OPUS_SET_MAX_BANDWIDTH_REQUEST, __opus_check_int(x)

/** Gets the encoder's configured maximum allowed bandpass.
  * @see OPUS_SET_MAX_BANDWIDTH
  * @param[out] x <tt>opus_int32 *</tt>: Allowed values:
  * <dl>
  * <dt>#OPUS_BANDWIDTH_NARROWBAND</dt>    <dd>4 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_MEDIUMBAND</dt>    <dd>6 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_WIDEBAND</dt>      <dd>8 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_SUPERWIDEBAND</dt><dd>12 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_FULLBAND</dt>     <dd>20 kHz passband (default)</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_MAX_BANDWIDTH(x) OPUS_GET_MAX_BANDWIDTH_REQUEST, __opus_check_int_ptr(x)

/** Sets the encoder's bandpass to a specific value.
  * This prevents the encoder from automatically selecting the bandpass based
  * on the available bitrate. If an application knows the bandpass of the input
  * audio it is providing, it should normally use #OPUS_SET_MAX_BANDWIDTH
  * instead, which still gives the encoder the freedom to reduce the bandpass
  * when the bitrate becomes too low, for better overall quality.
  * @see OPUS_GET_BANDWIDTH
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>#OPUS_AUTO</dt>                    <dd>(default)</dd>
  * <dt>#OPUS_BANDWIDTH_NARROWBAND</dt>    <dd>4 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_MEDIUMBAND</dt>    <dd>6 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_WIDEBAND</dt>      <dd>8 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_SUPERWIDEBAND</dt><dd>12 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_FULLBAND</dt>     <dd>20 kHz passband</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_BANDWIDTH(x) OPUS_SET_BANDWIDTH_REQUEST, __opus_check_int(x)

/** Configures the type of signal being encoded.
  * This is a hint which helps the encoder's mode selection.
  * @see OPUS_GET_SIGNAL
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>#OPUS_AUTO</dt>        <dd>(default)</dd>
  * <dt>#OPUS_SIGNAL_VOICE</dt><dd>Bias thresholds towards choosing LPC or Hybrid modes.</dd>
  * <dt>#OPUS_SIGNAL_MUSIC</dt><dd>Bias thresholds towards choosing MDCT modes.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_SIGNAL(x) OPUS_SET_SIGNAL_REQUEST, __opus_check_int(x)
/** Gets the encoder's configured signal type.
  * @see OPUS_SET_SIGNAL
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>#OPUS_AUTO</dt>        <dd>(default)</dd>
  * <dt>#OPUS_SIGNAL_VOICE</dt><dd>Bias thresholds towards choosing LPC or Hybrid modes.</dd>
  * <dt>#OPUS_SIGNAL_MUSIC</dt><dd>Bias thresholds towards choosing MDCT modes.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_SIGNAL(x) OPUS_GET_SIGNAL_REQUEST, __opus_check_int_ptr(x)


/** Configures the encoder's intended application.
  * The initial value is a mandatory argument to the encoder_create function.
  * @see OPUS_GET_APPLICATION
  * @param[in] x <tt>opus_int32</tt>: Returns one of the following values:
  * <dl>
  * <dt>#OPUS_APPLICATION_VOIP</dt>
  * <dd>Process signal for improved speech intelligibility.</dd>
  * <dt>#OPUS_APPLICATION_AUDIO</dt>
  * <dd>Favor faithfulness to the original input.</dd>
  * <dt>#OPUS_APPLICATION_RESTRICTED_LOWDELAY</dt>
  * <dd>Configure the minimum possible coding delay by disabling certain modes
  * of operation.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_APPLICATION(x) OPUS_SET_APPLICATION_REQUEST, __opus_check_int(x)
/** Gets the encoder's configured application.
  * @see OPUS_SET_APPLICATION
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>#OPUS_APPLICATION_VOIP</dt>
  * <dd>Process signal for improved speech intelligibility.</dd>
  * <dt>#OPUS_APPLICATION_AUDIO</dt>
  * <dd>Favor faithfulness to the original input.</dd>
  * <dt>#OPUS_APPLICATION_RESTRICTED_LOWDELAY</dt>
  * <dd>Configure the minimum possible coding delay by disabling certain modes
  * of operation.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_APPLICATION(x) OPUS_GET_APPLICATION_REQUEST, __opus_check_int_ptr(x)

/** Gets the sampling rate the encoder or decoder was initialized with.
  * This simply returns the <code>Fs</code> value passed to opus_encoder_init()
  * or opus_decoder_init().
  * @param[out] x <tt>opus_int32 *</tt>: Sampling rate of encoder or decoder.
  * @hideinitializer
  */
#define OPUS_GET_SAMPLE_RATE(x) OPUS_GET_SAMPLE_RATE_REQUEST, __opus_check_int_ptr(x)

/** Gets the total samples of delay added by the entire codec.
  * This can be queried by the encoder and then the provided number of samples can be
  * skipped on from the start of the decoder's output to provide time aligned input
  * and output. From the perspective of a decoding application the real data begins this many
  * samples late.
  *
  * The decoder contribution to this delay is identical for all decoders, but the
  * encoder portion of the delay may vary from implementation to implementation,
  * version to version, or even depend on the encoder's initial configuration.
  * Applications needing delay compensation should call this CTL rather than
  * hard-coding a value.
  * @param[out] x <tt>opus_int32 *</tt>:   Number of lookahead samples
  * @hideinitializer */
#define OPUS_GET_LOOKAHEAD(x) OPUS_GET_LOOKAHEAD_REQUEST, __opus_check_int_ptr(x)

/** Configures the encoder's use of inband forward error correction (FEC).
  * @note This is only applicable to the LPC layer
  * @see OPUS_GET_INBAND_FEC
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>0</dt><dd>Disable inband FEC (default).</dd>
  * <dt>1</dt><dd>Enable inband FEC.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_INBAND_FEC(x) OPUS_SET_INBAND_FEC_REQUEST, __opus_check_int(x)
/** Gets encoder's configured use of inband forward error correction.
  * @see OPUS_SET_INBAND_FEC
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>0</dt><dd>Inband FEC disabled (default).</dd>
  * <dt>1</dt><dd>Inband FEC enabled.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_INBAND_FEC(x) OPUS_GET_INBAND_FEC_REQUEST, __opus_check_int_ptr(x)

/** Configures the encoder's expected packet loss percentage.
  * Higher values with trigger progressively more loss resistant behavior in the encoder
  * at the expense of quality at a given bitrate in the lossless case, but greater quality
  * under loss.
  * @see OPUS_GET_PACKET_LOSS_PERC
  * @param[in] x <tt>opus_int32</tt>:   Loss percentage in the range 0-100, inclusive (default: 0).
  * @hideinitializer */
#define OPUS_SET_PACKET_LOSS_PERC(x) OPUS_SET_PACKET_LOSS_PERC_REQUEST, __opus_check_int(x)
/** Gets the encoder's configured packet loss percentage.
  * @see OPUS_SET_PACKET_LOSS_PERC
  * @param[out] x <tt>opus_int32 *</tt>: Returns the configured loss percentage
  *                                      in the range 0-100, inclusive (default: 0).
  * @hideinitializer */
#define OPUS_GET_PACKET_LOSS_PERC(x) OPUS_GET_PACKET_LOSS_PERC_REQUEST, __opus_check_int_ptr(x)

/** Configures the encoder's use of discontinuous transmission (DTX).
  * @note This is only applicable to the LPC layer
  * @see OPUS_GET_DTX
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>0</dt><dd>Disable DTX (default).</dd>
  * <dt>1</dt><dd>Enabled DTX.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_DTX(x) OPUS_SET_DTX_REQUEST, __opus_check_int(x)
/** Gets encoder's configured use of discontinuous transmission.
  * @see OPUS_SET_DTX
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>0</dt><dd>DTX disabled (default).</dd>
  * <dt>1</dt><dd>DTX enabled.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_DTX(x) OPUS_GET_DTX_REQUEST, __opus_check_int_ptr(x)
/** Configures the depth of signal being encoded.
  * This is a hint which helps the encoder identify silence and near-silence.
  * @see OPUS_GET_LSB_DEPTH
  * @param[in] x <tt>opus_int32</tt>: Input precision in bits, between 8 and 24
  *                                   (default: 24).
  * @hideinitializer */
#define OPUS_SET_LSB_DEPTH(x) OPUS_SET_LSB_DEPTH_REQUEST, __opus_check_int(x)
/** Gets the encoder's configured signal depth.
  * @see OPUS_SET_LSB_DEPTH
  * @param[out] x <tt>opus_int32 *</tt>: Input precision in bits, between 8 and
  *                                      24 (default: 24).
  * @hideinitializer */
#define OPUS_GET_LSB_DEPTH(x) OPUS_GET_LSB_DEPTH_REQUEST, __opus_check_int_ptr(x)

/** Gets the duration (in samples) of the last packet successfully decoded or concealed.
  * @param[out] x <tt>opus_int32 *</tt>: Number of samples (at current sampling rate).
  * @hideinitializer */
#define OPUS_GET_LAST_PACKET_DURATION(x) OPUS_GET_LAST_PACKET_DURATION_REQUEST, __opus_check_int_ptr(x)

/** Configures the encoder's use of variable duration frames.
  * When variable duration is enabled, the encoder is free to use a shorter frame
  * size than the one requested in the opus_encode*() call.
  * It is then the user's responsibility
  * to verify how much audio was encoded by checking the ToC byte of the encoded
  * packet. The part of the audio that was not encoded needs to be resent to the
  * encoder for the next call. Do not use this option unless you <b>really</b>
  * know what you are doing.
  * @see OPUS_GET_EXPERT_VARIABLE_DURATION
  * @param[in] x <tt>opus_int32</tt>: Allowed values:
  * <dl>
  * <dt>OPUS_FRAMESIZE_ARG</dt><dd>Select frame size from the argument (default).</dd>
  * <dt>OPUS_FRAMESIZE_2_5_MS</dt><dd>Use 2.5 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_5_MS</dt><dd>Use 2.5 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_10_MS</dt><dd>Use 10 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_20_MS</dt><dd>Use 20 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_40_MS</dt><dd>Use 40 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_60_MS</dt><dd>Use 60 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_VARIABLE</dt><dd>Optimize the frame size dynamically.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_SET_EXPERT_FRAME_DURATION(x) OPUS_SET_EXPERT_FRAME_DURATION_REQUEST, __opus_check_int(x)
/** Gets the encoder's configured use of variable duration frames.
  * @see OPUS_SET_EXPERT_VARIABLE_DURATION
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>OPUS_FRAMESIZE_ARG</dt><dd>Select frame size from the argument (default).</dd>
  * <dt>OPUS_FRAMESIZE_2_5_MS</dt><dd>Use 2.5 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_5_MS</dt><dd>Use 2.5 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_10_MS</dt><dd>Use 10 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_20_MS</dt><dd>Use 20 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_40_MS</dt><dd>Use 40 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_60_MS</dt><dd>Use 60 ms frames.</dd>
  * <dt>OPUS_FRAMESIZE_VARIABLE</dt><dd>Optimize the frame size dynamically.</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_EXPERT_FRAME_DURATION(x) OPUS_GET_EXPERT_FRAME_DURATION_REQUEST, __opus_check_int_ptr(x)

/** If set to 1, disables almost all use of prediction, making frames almost
    completely independent. This reduces quality. (default : 0)
  * @hideinitializer */
#define OPUS_SET_PREDICTION_DISABLED(x) OPUS_SET_PREDICTION_DISABLED_REQUEST, __opus_check_int(x)
/** Gets the encoder's configured prediction status.
  * @hideinitializer */
#define OPUS_GET_PREDICTION_DISABLED(x) OPUS_GET_PREDICTION_DISABLED_REQUEST, __opus_check_int_ptr(x)

/**@}*/

/** @defgroup opus_genericctls Generic CTLs
  *
  * These macros are used with the \c opus_decoder_ctl and
  * \c opus_encoder_ctl calls to generate a particular
  * request.
  *
  * When called on an \c OpusDecoder they apply to that
  * particular decoder instance. When called on an
  * \c OpusEncoder they apply to the corresponding setting
  * on that encoder instance, if present.
  *
  * Some usage examples:
  *
  * @code
  * int ret;
  * opus_int32 pitch;
  * ret = opus_decoder_ctl(dec_ctx, OPUS_GET_PITCH(&pitch));
  * if (ret == OPUS_OK) return ret;
  *
  * opus_encoder_ctl(enc_ctx, OPUS_RESET_STATE);
  * opus_decoder_ctl(dec_ctx, OPUS_RESET_STATE);
  *
  * opus_int32 enc_bw, dec_bw;
  * opus_encoder_ctl(enc_ctx, OPUS_GET_BANDWIDTH(&enc_bw));
  * opus_decoder_ctl(dec_ctx, OPUS_GET_BANDWIDTH(&dec_bw));
  * if (enc_bw != dec_bw) {
  *   printf("packet bandwidth mismatch!\n");
  * }
  * @endcode
  *
  * @see opus_encoder, opus_decoder_ctl, opus_encoder_ctl, opus_decoderctls, opus_encoderctls
  * @{
  */

/** Resets the codec state to be equivalent to a freshly initialized state.
  * This should be called when switching streams in order to prevent
  * the back to back decoding from giving different results from
  * one at a time decoding.
  * @hideinitializer */
#define OPUS_RESET_STATE 4028

/** Gets the final state of the codec's entropy coder.
  * This is used for testing purposes,
  * The encoder and decoder state should be identical after coding a payload
  * (assuming no data corruption or software bugs)
  *
  * @param[out] x <tt>opus_uint32 *</tt>: Entropy coder state
  *
  * @hideinitializer */
#define OPUS_GET_FINAL_RANGE(x) OPUS_GET_FINAL_RANGE_REQUEST, __opus_check_uint_ptr(x)

/** Gets the pitch of the last decoded frame, if available.
  * This can be used for any post-processing algorithm requiring the use of pitch,
  * e.g. time stretching/shortening. If the last frame was not voiced, or if the
  * pitch was not coded in the frame, then zero is returned.
  *
  * This CTL is only implemented for decoder instances.
  *
  * @param[out] x <tt>opus_int32 *</tt>: pitch period at 48 kHz (or 0 if not available)
  *
  * @hideinitializer */
#define OPUS_GET_PITCH(x) OPUS_GET_PITCH_REQUEST, __opus_check_int_ptr(x)

/** Gets the encoder's configured bandpass or the decoder's last bandpass.
  * @see OPUS_SET_BANDWIDTH
  * @param[out] x <tt>opus_int32 *</tt>: Returns one of the following values:
  * <dl>
  * <dt>#OPUS_AUTO</dt>                    <dd>(default)</dd>
  * <dt>#OPUS_BANDWIDTH_NARROWBAND</dt>    <dd>4 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_MEDIUMBAND</dt>    <dd>6 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_WIDEBAND</dt>      <dd>8 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_SUPERWIDEBAND</dt><dd>12 kHz passband</dd>
  * <dt>#OPUS_BANDWIDTH_FULLBAND</dt>     <dd>20 kHz passband</dd>
  * </dl>
  * @hideinitializer */
#define OPUS_GET_BANDWIDTH(x) OPUS_GET_BANDWIDTH_REQUEST, __opus_check_int_ptr(x)

/**@}*/

/** @defgroup opus_decoderctls Decoder related CTLs
  * @see opus_genericctls, opus_encoderctls, opus_decoder
  * @{
  */

/** Configures decoder gain adjustment.
  * Scales the decoded output by a factor specified in Q8 dB units.
  * This has a maximum range of -32768 to 32767 inclusive, and returns
  * OPUS_BAD_ARG otherwise. The default is zero indicating no adjustment.
  * This setting survives decoder reset.
  *
  * gain = pow(10, x/(20.0*256))
  *
  * @param[in] x <tt>opus_int32</tt>:   Amount to scale PCM signal by in Q8 dB units.
  * @hideinitializer */
#define OPUS_SET_GAIN(x) OPUS_SET_GAIN_REQUEST, __opus_check_int(x)
/** Gets the decoder's configured gain adjustment. @see OPUS_SET_GAIN
  *
  * @param[out] x <tt>opus_int32 *</tt>: Amount to scale PCM signal by in Q8 dB units.
  * @hideinitializer */
#define OPUS_GET_GAIN(x) OPUS_GET_GAIN_REQUEST, __opus_check_int_ptr(x)

/**@}*/

/** @defgroup opus_libinfo Opus library information functions
  * @{
  */

/** Converts an opus error code into a human readable string.
  *
  * @param[in] error <tt>int</tt>: Error number
  * @returns Error string
  */
OPUS_EXPORT const char *opus_strerror(int error);

/** Gets the libopus version string.
  *
  * @returns Version string
  */
OPUS_EXPORT const char *opus_get_version_string(void);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* OPUS_DEFINES_H */
/* Copyright (c) 2010-2011 Xiph.Org Foundation, Skype Limited
   Written by Jean-Marc Valin and Koen Vos */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file opus.h
 * @brief Opus reference implementation API
 */

#ifndef OPUS_H
#define OPUS_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @mainpage Opus
 *
 * The Opus codec is designed for interactive speech and audio transmission over the Internet.
 * It is designed by the IETF Codec Working Group and incorporates technology from
 * Skype's SILK codec and Xiph.Org's CELT codec.
 *
 * The Opus codec is designed to handle a wide range of interactive audio applications,
 * including Voice over IP, videoconferencing, in-game chat, and even remote live music
 * performances. It can scale from low bit-rate narrowband speech to very high quality
 * stereo music. Its main features are:

 * @li Sampling rates from 8 to 48 kHz
 * @li Bit-rates from 6 kb/s to 510 kb/s
 * @li Support for both constant bit-rate (CBR) and variable bit-rate (VBR)
 * @li Audio bandwidth from narrowband to full-band
 * @li Support for speech and music
 * @li Support for mono and stereo
 * @li Support for multichannel (up to 255 channels)
 * @li Frame sizes from 2.5 ms to 60 ms
 * @li Good loss robustness and packet loss concealment (PLC)
 * @li Floating point and fixed-point implementation
 *
 * Documentation sections:
 * @li @ref opus_encoder
 * @li @ref opus_decoder
 * @li @ref opus_repacketizer
 * @li @ref opus_multistream
 * @li @ref opus_libinfo
 * @li @ref opus_custom
 */

/** @defgroup opus_encoder Opus Encoder
  * @{
  *
  * @brief This page describes the process and functions used to encode Opus.
  *
  * Since Opus is a stateful codec, the encoding process starts with creating an encoder
  * state. This can be done with:
  *
  * @code
  * int          error;
  * OpusEncoder *enc;
  * enc = opus_encoder_create(Fs, channels, application, &error);
  * @endcode
  *
  * From this point, @c enc can be used for encoding an audio stream. An encoder state
  * @b must @b not be used for more than one stream at the same time. Similarly, the encoder
  * state @b must @b not be re-initialized for each frame.
  *
  * While opus_encoder_create() allocates memory for the state, it's also possible
  * to initialize pre-allocated memory:
  *
  * @code
  * int          size;
  * int          error;
  * OpusEncoder *enc;
  * size = opus_encoder_get_size(channels);
  * enc = malloc(size);
  * error = opus_encoder_init(enc, Fs, channels, application);
  * @endcode
  *
  * where opus_encoder_get_size() returns the required size for the encoder state. Note that
  * future versions of this code may change the size, so no assuptions should be made about it.
  *
  * The encoder state is always continuous in memory and only a shallow copy is sufficient
  * to copy it (e.g. memcpy())
  *
  * It is possible to change some of the encoder's settings using the opus_encoder_ctl()
  * interface. All these settings already default to the recommended value, so they should
  * only be changed when necessary. The most common settings one may want to change are:
  *
  * @code
  * opus_encoder_ctl(enc, OPUS_SET_BITRATE(bitrate));
  * opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(complexity));
  * opus_encoder_ctl(enc, OPUS_SET_SIGNAL(signal_type));
  * @endcode
  *
  * where
  *
  * @arg bitrate is in bits per second (b/s)
  * @arg complexity is a value from 1 to 10, where 1 is the lowest complexity and 10 is the highest
  * @arg signal_type is either OPUS_AUTO (default), OPUS_SIGNAL_VOICE, or OPUS_SIGNAL_MUSIC
  *
  * See @ref opus_encoderctls and @ref opus_genericctls for a complete list of parameters that can be set or queried. Most parameters can be set or changed at any time during a stream.
  *
  * To encode a frame, opus_encode() or opus_encode_float() must be called with exactly one frame (2.5, 5, 10, 20, 40 or 60 ms) of audio data:
  * @code
  * len = opus_encode(enc, audio_frame, frame_size, packet, max_packet);
  * @endcode
  *
  * where
  * <ul>
  * <li>audio_frame is the audio data in opus_int16 (or float for opus_encode_float())</li>
  * <li>frame_size is the duration of the frame in samples (per channel)</li>
  * <li>packet is the byte array to which the compressed data is written</li>
  * <li>max_packet is the maximum number of bytes that can be written in the packet (4000 bytes is recommended).
  *     Do not use max_packet to control VBR target bitrate, instead use the #OPUS_SET_BITRATE CTL.</li>
  * </ul>
  *
  * opus_encode() and opus_encode_float() return the number of bytes actually written to the packet.
  * The return value <b>can be negative</b>, which indicates that an error has occurred. If the return value
  * is 1 byte, then the packet does not need to be transmitted (DTX).
  *
  * Once the encoder state if no longer needed, it can be destroyed with
  *
  * @code
  * opus_encoder_destroy(enc);
  * @endcode
  *
  * If the encoder was created with opus_encoder_init() rather than opus_encoder_create(),
  * then no action is required aside from potentially freeing the memory that was manually
  * allocated for it (calling free(enc) for the example above)
  *
  */

/** Opus encoder state.
  * This contains the complete state of an Opus encoder.
  * It is position independent and can be freely copied.
  * @see opus_encoder_create,opus_encoder_init
  */
typedef struct OpusEncoder OpusEncoder;

/** Gets the size of an <code>OpusEncoder</code> structure.
  * @param[in] channels <tt>int</tt>: Number of channels.
  *                                   This must be 1 or 2.
  * @returns The size in bytes.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_encoder_get_size(int channels);

/**
 */

/** Allocates and initializes an encoder state.
 * There are three coding modes:
 *
 * @ref OPUS_APPLICATION_VOIP gives best quality at a given bitrate for voice
 *    signals. It enhances the  input signal by high-pass filtering and
 *    emphasizing formants and harmonics. Optionally  it includes in-band
 *    forward error correction to protect against packet loss. Use this
 *    mode for typical VoIP applications. Because of the enhancement,
 *    even at high bitrates the output may sound different from the input.
 *
 * @ref OPUS_APPLICATION_AUDIO gives best quality at a given bitrate for most
 *    non-voice signals like music. Use this mode for music and mixed
 *    (music/voice) content, broadcast, and applications requiring less
 *    than 15 ms of coding delay.
 *
 * @ref OPUS_APPLICATION_RESTRICTED_LOWDELAY configures low-delay mode that
 *    disables the speech-optimized mode in exchange for slightly reduced delay.
 *    This mode can only be set on an newly initialized or freshly reset encoder
 *    because it changes the codec delay.
 *
 * This is useful when the caller knows that the speech-optimized modes will not be needed (use with caution).
 * @param [in] Fs <tt>opus_int32</tt>: Sampling rate of input signal (Hz)
 *                                     This must be one of 8000, 12000, 16000,
 *                                     24000, or 48000.
 * @param [in] channels <tt>int</tt>: Number of channels (1 or 2) in input signal
 * @param [in] application <tt>int</tt>: Coding mode (@ref OPUS_APPLICATION_VOIP/@ref OPUS_APPLICATION_AUDIO/@ref OPUS_APPLICATION_RESTRICTED_LOWDELAY)
 * @param [out] error <tt>int*</tt>: @ref opus_errorcodes
 * @note Regardless of the sampling rate and number channels selected, the Opus encoder
 * can switch to a lower audio bandwidth or number of channels if the bitrate
 * selected is too low. This also means that it is safe to always use 48 kHz stereo input
 * and let the encoder optimize the encoding.
 */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT OpusEncoder *opus_encoder_create(
    opus_int32 Fs,
    int channels,
    int application,
    int *error
);

/** Initializes a previously allocated encoder state
  * The memory pointed to by st must be at least the size returned by opus_encoder_get_size().
  * This is intended for applications which use their own allocator instead of malloc.
  * @see opus_encoder_create(),opus_encoder_get_size()
  * To reset a previously initialized state, use the #OPUS_RESET_STATE CTL.
  * @param [in] st <tt>OpusEncoder*</tt>: Encoder state
  * @param [in] Fs <tt>opus_int32</tt>: Sampling rate of input signal (Hz)
 *                                      This must be one of 8000, 12000, 16000,
 *                                      24000, or 48000.
  * @param [in] channels <tt>int</tt>: Number of channels (1 or 2) in input signal
  * @param [in] application <tt>int</tt>: Coding mode (OPUS_APPLICATION_VOIP/OPUS_APPLICATION_AUDIO/OPUS_APPLICATION_RESTRICTED_LOWDELAY)
  * @retval #OPUS_OK Success or @ref opus_errorcodes
  */
OPUS_EXPORT int opus_encoder_init(
    OpusEncoder *st,
    opus_int32 Fs,
    int channels,
    int application
) OPUS_ARG_NONNULL(1);

/** Encodes an Opus frame.
  * @param [in] st <tt>OpusEncoder*</tt>: Encoder state
  * @param [in] pcm <tt>opus_int16*</tt>: Input signal (interleaved if 2 channels). length is frame_size*channels*sizeof(opus_int16)
  * @param [in] frame_size <tt>int</tt>: Number of samples per channel in the
  *                                      input signal.
  *                                      This must be an Opus frame size for
  *                                      the encoder's sampling rate.
  *                                      For example, at 48 kHz the permitted
  *                                      values are 120, 240, 480, 960, 1920,
  *                                      and 2880.
  *                                      Passing in a duration of less than
  *                                      10 ms (480 samples at 48 kHz) will
  *                                      prevent the encoder from using the LPC
  *                                      or hybrid modes.
  * @param [out] data <tt>unsigned char*</tt>: Output payload.
  *                                            This must contain storage for at
  *                                            least \a max_data_bytes.
  * @param [in] max_data_bytes <tt>opus_int32</tt>: Size of the allocated
  *                                                 memory for the output
  *                                                 payload. This may be
  *                                                 used to impose an upper limit on
  *                                                 the instant bitrate, but should
  *                                                 not be used as the only bitrate
  *                                                 control. Use #OPUS_SET_BITRATE to
  *                                                 control the bitrate.
  * @returns The length of the encoded packet (in bytes) on success or a
  *          negative error code (see @ref opus_errorcodes) on failure.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_encode(
    OpusEncoder *st,
    const opus_int16 *pcm,
    int frame_size,
    unsigned char *data,
    opus_int32 max_data_bytes
) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(2) OPUS_ARG_NONNULL(4);

/** Encodes an Opus frame from floating point input.
  * @param [in] st <tt>OpusEncoder*</tt>: Encoder state
  * @param [in] pcm <tt>float*</tt>: Input in float format (interleaved if 2 channels), with a normal range of +/-1.0.
  *          Samples with a range beyond +/-1.0 are supported but will
  *          be clipped by decoders using the integer API and should
  *          only be used if it is known that the far end supports
  *          extended dynamic range.
  *          length is frame_size*channels*sizeof(float)
  * @param [in] frame_size <tt>int</tt>: Number of samples per channel in the
  *                                      input signal.
  *                                      This must be an Opus frame size for
  *                                      the encoder's sampling rate.
  *                                      For example, at 48 kHz the permitted
  *                                      values are 120, 240, 480, 960, 1920,
  *                                      and 2880.
  *                                      Passing in a duration of less than
  *                                      10 ms (480 samples at 48 kHz) will
  *                                      prevent the encoder from using the LPC
  *                                      or hybrid modes.
  * @param [out] data <tt>unsigned char*</tt>: Output payload.
  *                                            This must contain storage for at
  *                                            least \a max_data_bytes.
  * @param [in] max_data_bytes <tt>opus_int32</tt>: Size of the allocated
  *                                                 memory for the output
  *                                                 payload. This may be
  *                                                 used to impose an upper limit on
  *                                                 the instant bitrate, but should
  *                                                 not be used as the only bitrate
  *                                                 control. Use #OPUS_SET_BITRATE to
  *                                                 control the bitrate.
  * @returns The length of the encoded packet (in bytes) on success or a
  *          negative error code (see @ref opus_errorcodes) on failure.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_encode_float(
    OpusEncoder *st,
    const float *pcm,
    int frame_size,
    unsigned char *data,
    opus_int32 max_data_bytes
) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(2) OPUS_ARG_NONNULL(4);

/** Frees an <code>OpusEncoder</code> allocated by opus_encoder_create().
  * @param[in] st <tt>OpusEncoder*</tt>: State to be freed.
  */
OPUS_EXPORT void opus_encoder_destroy(OpusEncoder *st);

/** Perform a CTL function on an Opus encoder.
  *
  * Generally the request and subsequent arguments are generated
  * by a convenience macro.
  * @param st <tt>OpusEncoder*</tt>: Encoder state.
  * @param request This and all remaining parameters should be replaced by one
  *                of the convenience macros in @ref opus_genericctls or
  *                @ref opus_encoderctls.
  * @see opus_genericctls
  * @see opus_encoderctls
  */
OPUS_EXPORT int opus_encoder_ctl(OpusEncoder *st, int request, ...) OPUS_ARG_NONNULL(1);
/**@}*/

/** @defgroup opus_decoder Opus Decoder
  * @{
  *
  * @brief This page describes the process and functions used to decode Opus.
  *
  * The decoding process also starts with creating a decoder
  * state. This can be done with:
  * @code
  * int          error;
  * OpusDecoder *dec;
  * dec = opus_decoder_create(Fs, channels, &error);
  * @endcode
  * where
  * @li Fs is the sampling rate and must be 8000, 12000, 16000, 24000, or 48000
  * @li channels is the number of channels (1 or 2)
  * @li error will hold the error code in case of failure (or #OPUS_OK on success)
  * @li the return value is a newly created decoder state to be used for decoding
  *
  * While opus_decoder_create() allocates memory for the state, it's also possible
  * to initialize pre-allocated memory:
  * @code
  * int          size;
  * int          error;
  * OpusDecoder *dec;
  * size = opus_decoder_get_size(channels);
  * dec = malloc(size);
  * error = opus_decoder_init(dec, Fs, channels);
  * @endcode
  * where opus_decoder_get_size() returns the required size for the decoder state. Note that
  * future versions of this code may change the size, so no assuptions should be made about it.
  *
  * The decoder state is always continuous in memory and only a shallow copy is sufficient
  * to copy it (e.g. memcpy())
  *
  * To decode a frame, opus_decode() or opus_decode_float() must be called with a packet of compressed audio data:
  * @code
  * frame_size = opus_decode(dec, packet, len, decoded, max_size, 0);
  * @endcode
  * where
  *
  * @li packet is the byte array containing the compressed data
  * @li len is the exact number of bytes contained in the packet
  * @li decoded is the decoded audio data in opus_int16 (or float for opus_decode_float())
  * @li max_size is the max duration of the frame in samples (per channel) that can fit into the decoded_frame array
  *
  * opus_decode() and opus_decode_float() return the number of samples (per channel) decoded from the packet.
  * If that value is negative, then an error has occurred. This can occur if the packet is corrupted or if the audio
  * buffer is too small to hold the decoded audio.
  *
  * Opus is a stateful codec with overlapping blocks and as a result Opus
  * packets are not coded independently of each other. Packets must be
  * passed into the decoder serially and in the correct order for a correct
  * decode. Lost packets can be replaced with loss concealment by calling
  * the decoder with a null pointer and zero length for the missing packet.
  *
  * A single codec state may only be accessed from a single thread at
  * a time and any required locking must be performed by the caller. Separate
  * streams must be decoded with separate decoder states and can be decoded
  * in parallel unless the library was compiled with NONTHREADSAFE_PSEUDOSTACK
  * defined.
  *
  */

/** Opus decoder state.
  * This contains the complete state of an Opus decoder.
  * It is position independent and can be freely copied.
  * @see opus_decoder_create,opus_decoder_init
  */
typedef struct OpusDecoder OpusDecoder;

/** Gets the size of an <code>OpusDecoder</code> structure.
  * @param [in] channels <tt>int</tt>: Number of channels.
  *                                    This must be 1 or 2.
  * @returns The size in bytes.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_decoder_get_size(int channels);

/** Allocates and initializes a decoder state.
  * @param [in] Fs <tt>opus_int32</tt>: Sample rate to decode at (Hz).
  *                                     This must be one of 8000, 12000, 16000,
  *                                     24000, or 48000.
  * @param [in] channels <tt>int</tt>: Number of channels (1 or 2) to decode
  * @param [out] error <tt>int*</tt>: #OPUS_OK Success or @ref opus_errorcodes
  *
  * Internally Opus stores data at 48000 Hz, so that should be the default
  * value for Fs. However, the decoder can efficiently decode to buffers
  * at 8, 12, 16, and 24 kHz so if for some reason the caller cannot use
  * data at the full sample rate, or knows the compressed data doesn't
  * use the full frequency range, it can request decoding at a reduced
  * rate. Likewise, the decoder is capable of filling in either mono or
  * interleaved stereo pcm buffers, at the caller's request.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT OpusDecoder *opus_decoder_create(
    opus_int32 Fs,
    int channels,
    int *error
);

/** Initializes a previously allocated decoder state.
  * The state must be at least the size returned by opus_decoder_get_size().
  * This is intended for applications which use their own allocator instead of malloc. @see opus_decoder_create,opus_decoder_get_size
  * To reset a previously initialized state, use the #OPUS_RESET_STATE CTL.
  * @param [in] st <tt>OpusDecoder*</tt>: Decoder state.
  * @param [in] Fs <tt>opus_int32</tt>: Sampling rate to decode to (Hz).
  *                                     This must be one of 8000, 12000, 16000,
  *                                     24000, or 48000.
  * @param [in] channels <tt>int</tt>: Number of channels (1 or 2) to decode
  * @retval #OPUS_OK Success or @ref opus_errorcodes
  */
OPUS_EXPORT int opus_decoder_init(
    OpusDecoder *st,
    opus_int32 Fs,
    int channels
) OPUS_ARG_NONNULL(1);

/** Decode an Opus packet.
  * @param [in] st <tt>OpusDecoder*</tt>: Decoder state
  * @param [in] data <tt>char*</tt>: Input payload. Use a NULL pointer to indicate packet loss
  * @param [in] len <tt>opus_int32</tt>: Number of bytes in payload*
  * @param [out] pcm <tt>opus_int16*</tt>: Output signal (interleaved if 2 channels). length
  *  is frame_size*channels*sizeof(opus_int16)
  * @param [in] frame_size Number of samples per channel of available space in \a pcm.
  *  If this is less than the maximum packet duration (120ms; 5760 for 48kHz), this function will
  *  not be capable of decoding some packets. In the case of PLC (data==NULL) or FEC (decode_fec=1),
  *  then frame_size needs to be exactly the duration of audio that is missing, otherwise the
  *  decoder will not be in the optimal state to decode the next incoming packet. For the PLC and
  *  FEC cases, frame_size <b>must</b> be a multiple of 2.5 ms.
  * @param [in] decode_fec <tt>int</tt>: Flag (0 or 1) to request that any in-band forward error correction data be
  *  decoded. If no such data is available, the frame is decoded as if it were lost.
  * @returns Number of decoded samples or @ref opus_errorcodes
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_decode(
    OpusDecoder *st,
    const unsigned char *data,
    opus_int32 len,
    opus_int16 *pcm,
    int frame_size,
    int decode_fec
) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(4);

/** Decode an Opus packet with floating point output.
  * @param [in] st <tt>OpusDecoder*</tt>: Decoder state
  * @param [in] data <tt>char*</tt>: Input payload. Use a NULL pointer to indicate packet loss
  * @param [in] len <tt>opus_int32</tt>: Number of bytes in payload
  * @param [out] pcm <tt>float*</tt>: Output signal (interleaved if 2 channels). length
  *  is frame_size*channels*sizeof(float)
  * @param [in] frame_size Number of samples per channel of available space in \a pcm.
  *  If this is less than the maximum packet duration (120ms; 5760 for 48kHz), this function will
  *  not be capable of decoding some packets. In the case of PLC (data==NULL) or FEC (decode_fec=1),
  *  then frame_size needs to be exactly the duration of audio that is missing, otherwise the
  *  decoder will not be in the optimal state to decode the next incoming packet. For the PLC and
  *  FEC cases, frame_size <b>must</b> be a multiple of 2.5 ms.
  * @param [in] decode_fec <tt>int</tt>: Flag (0 or 1) to request that any in-band forward error correction data be
  *  decoded. If no such data is available the frame is decoded as if it were lost.
  * @returns Number of decoded samples or @ref opus_errorcodes
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_decode_float(
    OpusDecoder *st,
    const unsigned char *data,
    opus_int32 len,
    float *pcm,
    int frame_size,
    int decode_fec
) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(4);

/** Perform a CTL function on an Opus decoder.
  *
  * Generally the request and subsequent arguments are generated
  * by a convenience macro.
  * @param st <tt>OpusDecoder*</tt>: Decoder state.
  * @param request This and all remaining parameters should be replaced by one
  *                of the convenience macros in @ref opus_genericctls or
  *                @ref opus_decoderctls.
  * @see opus_genericctls
  * @see opus_decoderctls
  */
OPUS_EXPORT int opus_decoder_ctl(OpusDecoder *st, int request, ...) OPUS_ARG_NONNULL(1);

/** Frees an <code>OpusDecoder</code> allocated by opus_decoder_create().
  * @param[in] st <tt>OpusDecoder*</tt>: State to be freed.
  */
OPUS_EXPORT void opus_decoder_destroy(OpusDecoder *st);

/** Parse an opus packet into one or more frames.
  * Opus_decode will perform this operation internally so most applications do
  * not need to use this function.
  * This function does not copy the frames, the returned pointers are pointers into
  * the input packet.
  * @param [in] data <tt>char*</tt>: Opus packet to be parsed
  * @param [in] len <tt>opus_int32</tt>: size of data
  * @param [out] out_toc <tt>char*</tt>: TOC pointer
  * @param [out] frames <tt>char*[48]</tt> encapsulated frames
  * @param [out] size <tt>opus_int16[48]</tt> sizes of the encapsulated frames
  * @param [out] payload_offset <tt>int*</tt>: returns the position of the payload within the packet (in bytes)
  * @returns number of frames
  */
OPUS_EXPORT int opus_packet_parse(
   const unsigned char *data,
   opus_int32 len,
   unsigned char *out_toc,
   const unsigned char *frames[48],
   opus_int16 size[48],
   int *payload_offset
) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(4);

/** Gets the bandwidth of an Opus packet.
  * @param [in] data <tt>char*</tt>: Opus packet
  * @retval OPUS_BANDWIDTH_NARROWBAND Narrowband (4kHz bandpass)
  * @retval OPUS_BANDWIDTH_MEDIUMBAND Mediumband (6kHz bandpass)
  * @retval OPUS_BANDWIDTH_WIDEBAND Wideband (8kHz bandpass)
  * @retval OPUS_BANDWIDTH_SUPERWIDEBAND Superwideband (12kHz bandpass)
  * @retval OPUS_BANDWIDTH_FULLBAND Fullband (20kHz bandpass)
  * @retval OPUS_INVALID_PACKET The compressed data passed is corrupted or of an unsupported type
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_packet_get_bandwidth(const unsigned char *data) OPUS_ARG_NONNULL(1);

/** Gets the number of samples per frame from an Opus packet.
  * @param [in] data <tt>char*</tt>: Opus packet.
  *                                  This must contain at least one byte of
  *                                  data.
  * @param [in] Fs <tt>opus_int32</tt>: Sampling rate in Hz.
  *                                     This must be a multiple of 400, or
  *                                     inaccurate results will be returned.
  * @returns Number of samples per frame.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_packet_get_samples_per_frame(const unsigned char *data, opus_int32 Fs) OPUS_ARG_NONNULL(1);

/** Gets the number of channels from an Opus packet.
  * @param [in] data <tt>char*</tt>: Opus packet
  * @returns Number of channels
  * @retval OPUS_INVALID_PACKET The compressed data passed is corrupted or of an unsupported type
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_packet_get_nb_channels(const unsigned char *data) OPUS_ARG_NONNULL(1);

/** Gets the number of frames in an Opus packet.
  * @param [in] packet <tt>char*</tt>: Opus packet
  * @param [in] len <tt>opus_int32</tt>: Length of packet
  * @returns Number of frames
  * @retval OPUS_BAD_ARG Insufficient data was passed to the function
  * @retval OPUS_INVALID_PACKET The compressed data passed is corrupted or of an unsupported type
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_packet_get_nb_frames(const unsigned char packet[], opus_int32 len) OPUS_ARG_NONNULL(1);

/** Gets the number of samples of an Opus packet.
  * @param [in] packet <tt>char*</tt>: Opus packet
  * @param [in] len <tt>opus_int32</tt>: Length of packet
  * @param [in] Fs <tt>opus_int32</tt>: Sampling rate in Hz.
  *                                     This must be a multiple of 400, or
  *                                     inaccurate results will be returned.
  * @returns Number of samples
  * @retval OPUS_BAD_ARG Insufficient data was passed to the function
  * @retval OPUS_INVALID_PACKET The compressed data passed is corrupted or of an unsupported type
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_packet_get_nb_samples(const unsigned char packet[], opus_int32 len, opus_int32 Fs) OPUS_ARG_NONNULL(1);

/** Gets the number of samples of an Opus packet.
  * @param [in] dec <tt>OpusDecoder*</tt>: Decoder state
  * @param [in] packet <tt>char*</tt>: Opus packet
  * @param [in] len <tt>opus_int32</tt>: Length of packet
  * @returns Number of samples
  * @retval OPUS_BAD_ARG Insufficient data was passed to the function
  * @retval OPUS_INVALID_PACKET The compressed data passed is corrupted or of an unsupported type
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_decoder_get_nb_samples(const OpusDecoder *dec, const unsigned char packet[], opus_int32 len) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(2);

/** Applies soft-clipping to bring a float signal within the [-1,1] range. If
  * the signal is already in that range, nothing is done. If there are values
  * outside of [-1,1], then the signal is clipped as smoothly as possible to
  * both fit in the range and avoid creating excessive distortion in the
  * process.
  * @param [in,out] pcm <tt>float*</tt>: Input PCM and modified PCM
  * @param [in] frame_size <tt>int</tt> Number of samples per channel to process
  * @param [in] channels <tt>int</tt>: Number of channels
  * @param [in,out] softclip_mem <tt>float*</tt>: State memory for the soft clipping process (one float per channel, initialized to zero)
  */
OPUS_EXPORT void opus_pcm_soft_clip(float *pcm, int frame_size, int channels, float *softclip_mem);


/**@}*/

/** @defgroup opus_repacketizer Repacketizer
  * @{
  *
  * The repacketizer can be used to merge multiple Opus packets into a single
  * packet or alternatively to split Opus packets that have previously been
  * merged. Splitting valid Opus packets is always guaranteed to succeed,
  * whereas merging valid packets only succeeds if all frames have the same
  * mode, bandwidth, and frame size, and when the total duration of the merged
  * packet is no more than 120 ms.
  * The repacketizer currently only operates on elementary Opus
  * streams. It will not manipualte multistream packets successfully, except in
  * the degenerate case where they consist of data from a single stream.
  *
  * The repacketizing process starts with creating a repacketizer state, either
  * by calling opus_repacketizer_create() or by allocating the memory yourself,
  * e.g.,
  * @code
  * OpusRepacketizer *rp;
  * rp = (OpusRepacketizer*)malloc(opus_repacketizer_get_size());
  * if (rp != NULL)
  *     opus_repacketizer_init(rp);
  * @endcode
  *
  * Then the application should submit packets with opus_repacketizer_cat(),
  * extract new packets with opus_repacketizer_out() or
  * opus_repacketizer_out_range(), and then reset the state for the next set of
  * input packets via opus_repacketizer_init().
  *
  * For example, to split a sequence of packets into individual frames:
  * @code
  * unsigned char *data;
  * int len;
  * while (get_next_packet(&data, &len))
  * {
  *   unsigned char out[1276];
  *   opus_int32 out_len;
  *   int nb_frames;
  *   int err;
  *   int i;
  *   err = opus_repacketizer_cat(rp, data, len);
  *   if (err != OPUS_OK)
  *   {
  *     release_packet(data);
  *     return err;
  *   }
  *   nb_frames = opus_repacketizer_get_nb_frames(rp);
  *   for (i = 0; i < nb_frames; i++)
  *   {
  *     out_len = opus_repacketizer_out_range(rp, i, i+1, out, sizeof(out));
  *     if (out_len < 0)
  *     {
  *        release_packet(data);
  *        return (int)out_len;
  *     }
  *     output_next_packet(out, out_len);
  *   }
  *   opus_repacketizer_init(rp);
  *   release_packet(data);
  * }
  * @endcode
  *
  * Alternatively, to combine a sequence of frames into packets that each
  * contain up to <code>TARGET_DURATION_MS</code> milliseconds of data:
  * @code
  * // The maximum number of packets with duration TARGET_DURATION_MS occurs
  * // when the frame size is 2.5 ms, for a total of (TARGET_DURATION_MS*2/5)
  * // packets.
  * unsigned char *data[(TARGET_DURATION_MS*2/5)+1];
  * opus_int32 len[(TARGET_DURATION_MS*2/5)+1];
  * int nb_packets;
  * unsigned char out[1277*(TARGET_DURATION_MS*2/2)];
  * opus_int32 out_len;
  * int prev_toc;
  * nb_packets = 0;
  * while (get_next_packet(data+nb_packets, len+nb_packets))
  * {
  *   int nb_frames;
  *   int err;
  *   nb_frames = opus_packet_get_nb_frames(data[nb_packets], len[nb_packets]);
  *   if (nb_frames < 1)
  *   {
  *     release_packets(data, nb_packets+1);
  *     return nb_frames;
  *   }
  *   nb_frames += opus_repacketizer_get_nb_frames(rp);
  *   // If adding the next packet would exceed our target, or it has an
  *   // incompatible TOC sequence, output the packets we already have before
  *   // submitting it.
  *   // N.B., The nb_packets > 0 check ensures we've submitted at least one
  *   // packet since the last call to opus_repacketizer_init(). Otherwise a
  *   // single packet longer than TARGET_DURATION_MS would cause us to try to
  *   // output an (invalid) empty packet. It also ensures that prev_toc has
  *   // been set to a valid value. Additionally, len[nb_packets] > 0 is
  *   // guaranteed by the call to opus_packet_get_nb_frames() above, so the
  *   // reference to data[nb_packets][0] should be valid.
  *   if (nb_packets > 0 && (
  *       ((prev_toc & 0xFC) != (data[nb_packets][0] & 0xFC)) ||
  *       opus_packet_get_samples_per_frame(data[nb_packets], 48000)*nb_frames >
  *       TARGET_DURATION_MS*48))
  *   {
  *     out_len = opus_repacketizer_out(rp, out, sizeof(out));
  *     if (out_len < 0)
  *     {
  *        release_packets(data, nb_packets+1);
  *        return (int)out_len;
  *     }
  *     output_next_packet(out, out_len);
  *     opus_repacketizer_init(rp);
  *     release_packets(data, nb_packets);
  *     data[0] = data[nb_packets];
  *     len[0] = len[nb_packets];
  *     nb_packets = 0;
  *   }
  *   err = opus_repacketizer_cat(rp, data[nb_packets], len[nb_packets]);
  *   if (err != OPUS_OK)
  *   {
  *     release_packets(data, nb_packets+1);
  *     return err;
  *   }
  *   prev_toc = data[nb_packets][0];
  *   nb_packets++;
  * }
  * // Output the final, partial packet.
  * if (nb_packets > 0)
  * {
  *   out_len = opus_repacketizer_out(rp, out, sizeof(out));
  *   release_packets(data, nb_packets);
  *   if (out_len < 0)
  *     return (int)out_len;
  *   output_next_packet(out, out_len);
  * }
  * @endcode
  *
  * An alternate way of merging packets is to simply call opus_repacketizer_cat()
  * unconditionally until it fails. At that point, the merged packet can be
  * obtained with opus_repacketizer_out() and the input packet for which
  * opus_repacketizer_cat() needs to be re-added to a newly reinitialized
  * repacketizer state.
  */

typedef struct OpusRepacketizer OpusRepacketizer;

/** Gets the size of an <code>OpusRepacketizer</code> structure.
  * @returns The size in bytes.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_repacketizer_get_size(void);

/** (Re)initializes a previously allocated repacketizer state.
  * The state must be at least the size returned by opus_repacketizer_get_size().
  * This can be used for applications which use their own allocator instead of
  * malloc().
  * It must also be called to reset the queue of packets waiting to be
  * repacketized, which is necessary if the maximum packet duration of 120 ms
  * is reached or if you wish to submit packets with a different Opus
  * configuration (coding mode, audio bandwidth, frame size, or channel count).
  * Failure to do so will prevent a new packet from being added with
  * opus_repacketizer_cat().
  * @see opus_repacketizer_create
  * @see opus_repacketizer_get_size
  * @see opus_repacketizer_cat
  * @param rp <tt>OpusRepacketizer*</tt>: The repacketizer state to
  *                                       (re)initialize.
  * @returns A pointer to the same repacketizer state that was passed in.
  */
OPUS_EXPORT OpusRepacketizer *opus_repacketizer_init(OpusRepacketizer *rp) OPUS_ARG_NONNULL(1);

/** Allocates memory and initializes the new repacketizer with
 * opus_repacketizer_init().
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT OpusRepacketizer *opus_repacketizer_create(void);

/** Frees an <code>OpusRepacketizer</code> allocated by
  * opus_repacketizer_create().
  * @param[in] rp <tt>OpusRepacketizer*</tt>: State to be freed.
  */
OPUS_EXPORT void opus_repacketizer_destroy(OpusRepacketizer *rp);

/** Add a packet to the current repacketizer state.
  * This packet must match the configuration of any packets already submitted
  * for repacketization since the last call to opus_repacketizer_init().
  * This means that it must have the same coding mode, audio bandwidth, frame
  * size, and channel count.
  * This can be checked in advance by examining the top 6 bits of the first
  * byte of the packet, and ensuring they match the top 6 bits of the first
  * byte of any previously submitted packet.
  * The total duration of audio in the repacketizer state also must not exceed
  * 120 ms, the maximum duration of a single packet, after adding this packet.
  *
  * The contents of the current repacketizer state can be extracted into new
  * packets using opus_repacketizer_out() or opus_repacketizer_out_range().
  *
  * In order to add a packet with a different configuration or to add more
  * audio beyond 120 ms, you must clear the repacketizer state by calling
  * opus_repacketizer_init().
  * If a packet is too large to add to the current repacketizer state, no part
  * of it is added, even if it contains multiple frames, some of which might
  * fit.
  * If you wish to be able to add parts of such packets, you should first use
  * another repacketizer to split the packet into pieces and add them
  * individually.
  * @see opus_repacketizer_out_range
  * @see opus_repacketizer_out
  * @see opus_repacketizer_init
  * @param rp <tt>OpusRepacketizer*</tt>: The repacketizer state to which to
  *                                       add the packet.
  * @param[in] data <tt>const unsigned char*</tt>: The packet data.
  *                                                The application must ensure
  *                                                this pointer remains valid
  *                                                until the next call to
  *                                                opus_repacketizer_init() or
  *                                                opus_repacketizer_destroy().
  * @param len <tt>opus_int32</tt>: The number of bytes in the packet data.
  * @returns An error code indicating whether or not the operation succeeded.
  * @retval #OPUS_OK The packet's contents have been added to the repacketizer
  *                  state.
  * @retval #OPUS_INVALID_PACKET The packet did not have a valid TOC sequence,
  *                              the packet's TOC sequence was not compatible
  *                              with previously submitted packets (because
  *                              the coding mode, audio bandwidth, frame size,
  *                              or channel count did not match), or adding
  *                              this packet would increase the total amount of
  *                              audio stored in the repacketizer state to more
  *                              than 120 ms.
  */
OPUS_EXPORT int opus_repacketizer_cat(OpusRepacketizer *rp, const unsigned char *data, opus_int32 len) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(2);


/** Construct a new packet from data previously submitted to the repacketizer
  * state via opus_repacketizer_cat().
  * @param rp <tt>OpusRepacketizer*</tt>: The repacketizer state from which to
  *                                       construct the new packet.
  * @param begin <tt>int</tt>: The index of the first frame in the current
  *                            repacketizer state to include in the output.
  * @param end <tt>int</tt>: One past the index of the last frame in the
  *                          current repacketizer state to include in the
  *                          output.
  * @param[out] data <tt>const unsigned char*</tt>: The buffer in which to
  *                                                 store the output packet.
  * @param maxlen <tt>opus_int32</tt>: The maximum number of bytes to store in
  *                                    the output buffer. In order to guarantee
  *                                    success, this should be at least
  *                                    <code>1276</code> for a single frame,
  *                                    or for multiple frames,
  *                                    <code>1277*(end-begin)</code>.
  *                                    However, <code>1*(end-begin)</code> plus
  *                                    the size of all packet data submitted to
  *                                    the repacketizer since the last call to
  *                                    opus_repacketizer_init() or
  *                                    opus_repacketizer_create() is also
  *                                    sufficient, and possibly much smaller.
  * @returns The total size of the output packet on success, or an error code
  *          on failure.
  * @retval #OPUS_BAD_ARG <code>[begin,end)</code> was an invalid range of
  *                       frames (begin < 0, begin >= end, or end >
  *                       opus_repacketizer_get_nb_frames()).
  * @retval #OPUS_BUFFER_TOO_SMALL \a maxlen was insufficient to contain the
  *                                complete output packet.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_repacketizer_out_range(OpusRepacketizer *rp, int begin, int end, unsigned char *data, opus_int32 maxlen) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(4);

/** Return the total number of frames contained in packet data submitted to
  * the repacketizer state so far via opus_repacketizer_cat() since the last
  * call to opus_repacketizer_init() or opus_repacketizer_create().
  * This defines the valid range of packets that can be extracted with
  * opus_repacketizer_out_range() or opus_repacketizer_out().
  * @param rp <tt>OpusRepacketizer*</tt>: The repacketizer state containing the
  *                                       frames.
  * @returns The total number of frames contained in the packet data submitted
  *          to the repacketizer state.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_repacketizer_get_nb_frames(OpusRepacketizer *rp) OPUS_ARG_NONNULL(1);

/** Construct a new packet from data previously submitted to the repacketizer
  * state via opus_repacketizer_cat().
  * This is a convenience routine that returns all the data submitted so far
  * in a single packet.
  * It is equivalent to calling
  * @code
  * opus_repacketizer_out_range(rp, 0, opus_repacketizer_get_nb_frames(rp),
  *                             data, maxlen)
  * @endcode
  * @param rp <tt>OpusRepacketizer*</tt>: The repacketizer state from which to
  *                                       construct the new packet.
  * @param[out] data <tt>const unsigned char*</tt>: The buffer in which to
  *                                                 store the output packet.
  * @param maxlen <tt>opus_int32</tt>: The maximum number of bytes to store in
  *                                    the output buffer. In order to guarantee
  *                                    success, this should be at least
  *                                    <code>1277*opus_repacketizer_get_nb_frames(rp)</code>.
  *                                    However,
  *                                    <code>1*opus_repacketizer_get_nb_frames(rp)</code>
  *                                    plus the size of all packet data
  *                                    submitted to the repacketizer since the
  *                                    last call to opus_repacketizer_init() or
  *                                    opus_repacketizer_create() is also
  *                                    sufficient, and possibly much smaller.
  * @returns The total size of the output packet on success, or an error code
  *          on failure.
  * @retval #OPUS_BUFFER_TOO_SMALL \a maxlen was insufficient to contain the
  *                                complete output packet.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_repacketizer_out(OpusRepacketizer *rp, unsigned char *data, opus_int32 maxlen) OPUS_ARG_NONNULL(1);

/** Pads a given Opus packet to a larger size (possibly changing the TOC sequence).
  * @param[in,out] data <tt>const unsigned char*</tt>: The buffer containing the
  *                                                   packet to pad.
  * @param len <tt>opus_int32</tt>: The size of the packet.
  *                                 This must be at least 1.
  * @param new_len <tt>opus_int32</tt>: The desired size of the packet after padding.
  *                                 This must be at least as large as len.
  * @returns an error code
  * @retval #OPUS_OK \a on success.
  * @retval #OPUS_BAD_ARG \a len was less than 1 or new_len was less than len.
  * @retval #OPUS_INVALID_PACKET \a data did not contain a valid Opus packet.
  */
OPUS_EXPORT int opus_packet_pad(unsigned char *data, opus_int32 len, opus_int32 new_len);

/** Remove all padding from a given Opus packet and rewrite the TOC sequence to
  * minimize space usage.
  * @param[in,out] data <tt>const unsigned char*</tt>: The buffer containing the
  *                                                   packet to strip.
  * @param len <tt>opus_int32</tt>: The size of the packet.
  *                                 This must be at least 1.
  * @returns The new size of the output packet on success, or an error code
  *          on failure.
  * @retval #OPUS_BAD_ARG \a len was less than 1.
  * @retval #OPUS_INVALID_PACKET \a data did not contain a valid Opus packet.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_packet_unpad(unsigned char *data, opus_int32 len);

/** Pads a given Opus multi-stream packet to a larger size (possibly changing the TOC sequence).
  * @param[in,out] data <tt>const unsigned char*</tt>: The buffer containing the
  *                                                   packet to pad.
  * @param len <tt>opus_int32</tt>: The size of the packet.
  *                                 This must be at least 1.
  * @param new_len <tt>opus_int32</tt>: The desired size of the packet after padding.
  *                                 This must be at least 1.
  * @param nb_streams <tt>opus_int32</tt>: The number of streams (not channels) in the packet.
  *                                 This must be at least as large as len.
  * @returns an error code
  * @retval #OPUS_OK \a on success.
  * @retval #OPUS_BAD_ARG \a len was less than 1.
  * @retval #OPUS_INVALID_PACKET \a data did not contain a valid Opus packet.
  */
OPUS_EXPORT int opus_multistream_packet_pad(unsigned char *data, opus_int32 len, opus_int32 new_len, int nb_streams);

/** Remove all padding from a given Opus multi-stream packet and rewrite the TOC sequence to
  * minimize space usage.
  * @param[in,out] data <tt>const unsigned char*</tt>: The buffer containing the
  *                                                   packet to strip.
  * @param len <tt>opus_int32</tt>: The size of the packet.
  *                                 This must be at least 1.
  * @param nb_streams <tt>opus_int32</tt>: The number of streams (not channels) in the packet.
  *                                 This must be at least 1.
  * @returns The new size of the output packet on success, or an error code
  *          on failure.
  * @retval #OPUS_BAD_ARG \a len was less than 1 or new_len was less than len.
  * @retval #OPUS_INVALID_PACKET \a data did not contain a valid Opus packet.
  */
OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_multistream_packet_unpad(unsigned char *data, opus_int32 len, int nb_streams);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* OPUS_H */
/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2001             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *

 ********************************************************************

 function: libvorbis codec headers
 last mod: $Id: codec.h 17021 2010-03-24 09:29:41Z xiphmont $

 ********************************************************************/

#ifndef _vorbis_codec_h_
#define _vorbis_codec_h_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


typedef struct vorbis_info{
  int version;
  int channels;
  long rate;

  /* The below bitrate declarations are *hints*.
     Combinations of the three values carry the following implications:

     all three set to the same value:
       implies a fixed rate bitstream
     only nominal set:
       implies a VBR stream that averages the nominal bitrate.  No hard
       upper/lower limit
     upper and or lower set:
       implies a VBR bitstream that obeys the bitrate limits. nominal
       may also be set to give a nominal rate.
     none set:
       the coder does not care to speculate.
  */

  long bitrate_upper;
  long bitrate_nominal;
  long bitrate_lower;
  long bitrate_window;

  void *codec_setup;
} vorbis_info;

/* vorbis_dsp_state buffers the current vorbis audio
   analysis/synthesis state.  The DSP state belongs to a specific
   logical bitstream ****************************************************/
typedef struct vorbis_dsp_state{
  int analysisp;
  vorbis_info *vi;

  float **pcm;
  float **pcmret;
  int      pcm_storage;
  int      pcm_current;
  int      pcm_returned;

  int  preextrapolate;
  int  eofflag;

  long lW;
  long W;
  long nW;
  long centerW;

  ogg_int64_t granulepos;
  ogg_int64_t sequence;

  ogg_int64_t glue_bits;
  ogg_int64_t time_bits;
  ogg_int64_t floor_bits;
  ogg_int64_t res_bits;

  void       *backend_state;
} vorbis_dsp_state;

typedef struct vorbis_block{
  /* necessary stream state for linking to the framing abstraction */
  float  **pcm;       /* this is a pointer into local storage */
  oggpack_buffer opb;

  long  lW;
  long  W;
  long  nW;
  int   pcmend;
  int   mode;

  int         eofflag;
  ogg_int64_t granulepos;
  ogg_int64_t sequence;
  vorbis_dsp_state *vd; /* For read-only access of configuration */

  /* local storage to avoid remallocing; it's up to the mapping to
     structure it */
  void               *localstore;
  long                localtop;
  long                localalloc;
  long                totaluse;
  struct alloc_chain *reap;

  /* bitmetrics for the frame */
  long glue_bits;
  long time_bits;
  long floor_bits;
  long res_bits;

  void *internal;

} vorbis_block;

/* vorbis_block is a single block of data to be processed as part of
the analysis/synthesis stream; it belongs to a specific logical
bitstream, but is independent from other vorbis_blocks belonging to
that logical bitstream. *************************************************/

struct alloc_chain{
  void *ptr;
  struct alloc_chain *next;
};

/* vorbis_info contains all the setup information specific to the
   specific compression/decompression mode in progress (eg,
   psychoacoustic settings, channel setup, options, codebook
   etc). vorbis_info and substructures are in backends.h.
*********************************************************************/

/* the comments are not part of vorbis_info so that vorbis_info can be
   static storage */
typedef struct vorbis_comment{
  /* unlimited user comment fields.  libvorbis writes 'libvorbis'
     whatever vendor is set to in encode */
  char **user_comments;
  int   *comment_lengths;
  int    comments;
  char  *vendor;

} vorbis_comment;


/* libvorbis encodes in two abstraction layers; first we perform DSP
   and produce a packet (see docs/analysis.txt).  The packet is then
   coded into a framed OggSquish bitstream by the second layer (see
   docs/framing.txt).  Decode is the reverse process; we sync/frame
   the bitstream and extract individual packets, then decode the
   packet back into PCM audio.

   The extra framing/packetizing is used in streaming formats, such as
   files.  Over the net (such as with UDP), the framing and
   packetization aren't necessary as they're provided by the transport
   and the streaming layer is not used */

/* Vorbis PRIMITIVES: general ***************************************/

extern void     vorbis_info_init(vorbis_info *vi);
extern void     vorbis_info_clear(vorbis_info *vi);
extern int      vorbis_info_blocksize(vorbis_info *vi,int zo);
extern void     vorbis_comment_init(vorbis_comment *vc);
extern void     vorbis_comment_add(vorbis_comment *vc, const char *comment);
extern void     vorbis_comment_add_tag(vorbis_comment *vc,
                                       const char *tag, const char *contents);
extern char    *vorbis_comment_query(vorbis_comment *vc, const char *tag, int count);
extern int      vorbis_comment_query_count(vorbis_comment *vc, const char *tag);
extern void     vorbis_comment_clear(vorbis_comment *vc);

extern int      vorbis_block_init(vorbis_dsp_state *v, vorbis_block *vb);
extern int      vorbis_block_clear(vorbis_block *vb);
extern void     vorbis_dsp_clear(vorbis_dsp_state *v);
extern double   vorbis_granule_time(vorbis_dsp_state *v,
                                    ogg_int64_t granulepos);

extern const char *vorbis_version_string(void);

/* Vorbis PRIMITIVES: analysis/DSP layer ****************************/

extern int      vorbis_analysis_init(vorbis_dsp_state *v,vorbis_info *vi);
extern int      vorbis_commentheader_out(vorbis_comment *vc, ogg_packet *op);
extern int      vorbis_analysis_headerout(vorbis_dsp_state *v,
                                          vorbis_comment *vc,
                                          ogg_packet *op,
                                          ogg_packet *op_comm,
                                          ogg_packet *op_code);
extern float  **vorbis_analysis_buffer(vorbis_dsp_state *v,int vals);
extern int      vorbis_analysis_wrote(vorbis_dsp_state *v,int vals);
extern int      vorbis_analysis_blockout(vorbis_dsp_state *v,vorbis_block *vb);
extern int      vorbis_analysis(vorbis_block *vb,ogg_packet *op);

extern int      vorbis_bitrate_addblock(vorbis_block *vb);
extern int      vorbis_bitrate_flushpacket(vorbis_dsp_state *vd,
                                           ogg_packet *op);

/* Vorbis PRIMITIVES: synthesis layer *******************************/
extern int      vorbis_synthesis_idheader(ogg_packet *op);
extern int      vorbis_synthesis_headerin(vorbis_info *vi,vorbis_comment *vc,
                                          ogg_packet *op);

extern int      vorbis_synthesis_init(vorbis_dsp_state *v,vorbis_info *vi);
extern int      vorbis_synthesis_restart(vorbis_dsp_state *v);
extern int      vorbis_synthesis(vorbis_block *vb,ogg_packet *op);
extern int      vorbis_synthesis_trackonly(vorbis_block *vb,ogg_packet *op);
extern int      vorbis_synthesis_blockin(vorbis_dsp_state *v,vorbis_block *vb);
extern int      vorbis_synthesis_pcmout(vorbis_dsp_state *v,float ***pcm);
extern int      vorbis_synthesis_lapout(vorbis_dsp_state *v,float ***pcm);
extern int      vorbis_synthesis_read(vorbis_dsp_state *v,int samples);
extern long     vorbis_packet_blocksize(vorbis_info *vi,ogg_packet *op);

extern int      vorbis_synthesis_halfrate(vorbis_info *v,int flag);
extern int      vorbis_synthesis_halfrate_p(vorbis_info *v);

/* Vorbis ERRORS and return codes ***********************************/

#define OV_FALSE      -1
#define OV_EOF        -2
#define OV_HOLE       -3

#define OV_EREAD      -128
#define OV_EFAULT     -129
#define OV_EIMPL      -130
#define OV_EINVAL     -131
#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133
#define OV_EVERSION   -134
#define OV_ENOTAUDIO  -135
#define OV_EBADPACKET -136
#define OV_EBADLINK   -137
#define OV_ENOSEEK    -138

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2001             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: vorbis encode-engine setup
 last mod: $Id: vorbisenc.h 17021 2010-03-24 09:29:41Z xiphmont $

 ********************************************************************/

/** \file
 * Libvorbisenc is a convenient API for setting up an encoding
 * environment using libvorbis. Libvorbisenc encapsulates the
 * actions needed to set up the encoder properly.
 */

#ifndef _OV_ENC_H_
#define _OV_ENC_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
 * This is the primary function within libvorbisenc for setting up managed
 * bitrate modes.
 *
 * Before this function is called, the \ref vorbis_info
 * struct should be initialized by using vorbis_info_init() from the libvorbis
 * API.  After encoding, vorbis_info_clear() should be called.
 *
 * The max_bitrate, nominal_bitrate, and min_bitrate settings are used to set
 * constraints for the encoded file.  This function uses these settings to
 * select the appropriate encoding mode and set it up.
 *
 * \param vi               Pointer to an initialized \ref vorbis_info struct.
 * \param channels         The number of channels to be encoded.
 * \param rate             The sampling rate of the source audio.
 * \param max_bitrate      Desired maximum bitrate (limit). -1 indicates unset.
 * \param nominal_bitrate  Desired average, or central, bitrate. -1 indicates unset.
 * \param min_bitrate      Desired minimum bitrate. -1 indicates unset.
 *
 * \return Zero for success, and negative values for failure.
 *
 * \retval 0          Success.
 * \retval OV_EFAULT  Internal logic fault; indicates a bug or heap/stack corruption.
 * \retval OV_EINVAL  Invalid setup request, eg, out of range argument.
 * \retval OV_EIMPL   Unimplemented mode; unable to comply with bitrate request.
 */
extern int vorbis_encode_init(vorbis_info *vi,
                              long channels,
                              long rate,

                              long max_bitrate,
                              long nominal_bitrate,
                              long min_bitrate);

/**
 * This function performs step-one of a three-step bitrate-managed encode
 * setup.  It functions similarly to the one-step setup performed by \ref
 * vorbis_encode_init but allows an application to make further encode setup
 * tweaks using \ref vorbis_encode_ctl before finally calling \ref
 * vorbis_encode_setup_init to complete the setup process.
 *
 * Before this function is called, the \ref vorbis_info struct should be
 * initialized by using vorbis_info_init() from the libvorbis API.  After
 * encoding, vorbis_info_clear() should be called.
 *
 * The max_bitrate, nominal_bitrate, and min_bitrate settings are used to set
 * constraints for the encoded file.  This function uses these settings to
 * select the appropriate encoding mode and set it up.
 *
 * \param vi                Pointer to an initialized vorbis_info struct.
 * \param channels          The number of channels to be encoded.
 * \param rate              The sampling rate of the source audio.
 * \param max_bitrate       Desired maximum bitrate (limit). -1 indicates unset.
 * \param nominal_bitrate   Desired average, or central, bitrate. -1 indicates unset.
 * \param min_bitrate       Desired minimum bitrate. -1 indicates unset.
 *
 * \return Zero for success, and negative for failure.
 *
 * \retval 0           Success
 * \retval OV_EFAULT   Internal logic fault; indicates a bug or heap/stack corruption.
 * \retval OV_EINVAL   Invalid setup request, eg, out of range argument.
 * \retval OV_EIMPL    Unimplemented mode; unable to comply with bitrate request.
 */
extern int vorbis_encode_setup_managed(vorbis_info *vi,
                                       long channels,
                                       long rate,

                                       long max_bitrate,
                                       long nominal_bitrate,
                                       long min_bitrate);

/**
 * This function performs step-one of a three-step variable bitrate
 * (quality-based) encode setup.  It functions similarly to the one-step setup
 * performed by \ref vorbis_encode_init_vbr() but allows an application to
 * make further encode setup tweaks using \ref vorbis_encode_ctl() before
 * finally calling \ref vorbis_encode_setup_init to complete the setup
 * process.
 *
 * Before this function is called, the \ref vorbis_info struct should be
 * initialized by using \ref vorbis_info_init() from the libvorbis API.  After
 * encoding, vorbis_info_clear() should be called.
 *
 * \param vi        Pointer to an initialized vorbis_info struct.
 * \param channels  The number of channels to be encoded.
 * \param rate      The sampling rate of the source audio.
 * \param quality   Desired quality level, currently from -0.1 to 1.0 (lo to hi).
 *
 * \return Zero for success, and negative values for failure.
 *
 * \retval  0          Success
 * \retval  OV_EFAULT  Internal logic fault; indicates a bug or heap/stack corruption.
 * \retval  OV_EINVAL  Invalid setup request, eg, out of range argument.
 * \retval  OV_EIMPL   Unimplemented mode; unable to comply with quality level request.
 */
extern int vorbis_encode_setup_vbr(vorbis_info *vi,
                                  long channels,
                                  long rate,

                                  float quality
                                  );

/**
 * This is the primary function within libvorbisenc for setting up variable
 * bitrate ("quality" based) modes.
 *
 *
 * Before this function is called, the vorbis_info struct should be
 * initialized by using vorbis_info_init() from the libvorbis API. After
 * encoding, vorbis_info_clear() should be called.
 *
 * \param vi           Pointer to an initialized vorbis_info struct.
 * \param channels     The number of channels to be encoded.
 * \param rate         The sampling rate of the source audio.
 * \param base_quality Desired quality level, currently from -0.1 to 1.0 (lo to hi).
 *
 *
 * \return Zero for success, or a negative number for failure.
 *
 * \retval 0           Success
 * \retval OV_EFAULT   Internal logic fault; indicates a bug or heap/stack corruption.
 * \retval OV_EINVAL   Invalid setup request, eg, out of range argument.
 * \retval OV_EIMPL    Unimplemented mode; unable to comply with quality level request.
 */
extern int vorbis_encode_init_vbr(vorbis_info *vi,
                                  long channels,
                                  long rate,

                                  float base_quality
                                  );

/**
 * This function performs the last stage of three-step encoding setup, as
 * described in the API overview under managed bitrate modes.
 *
 * Before this function is called, the \ref vorbis_info struct should be
 * initialized by using vorbis_info_init() from the libvorbis API, one of
 * \ref vorbis_encode_setup_managed() or \ref vorbis_encode_setup_vbr() called to
 * initialize the high-level encoding setup, and \ref vorbis_encode_ctl()
 * called if necessary to make encoding setup changes.
 * vorbis_encode_setup_init() finalizes the highlevel encoding structure into
 * a complete encoding setup after which the application may make no further
 * setup changes.
 *
 * After encoding, vorbis_info_clear() should be called.
 *
 * \param vi Pointer to an initialized \ref vorbis_info struct.
 *
 * \return Zero for success, and negative values for failure.
 *
 * \retval  0           Success.
 * \retval  OV_EFAULT  Internal logic fault; indicates a bug or heap/stack corruption.
 *
 * \retval OV_EINVAL   Attempt to use vorbis_encode_setup_init() without first
 * calling one of vorbis_encode_setup_managed() or vorbis_encode_setup_vbr() to
 * initialize the high-level encoding setup
 *
 */
extern int vorbis_encode_setup_init(vorbis_info *vi);

/**
 * This function implements a generic interface to miscellaneous encoder
 * settings similar to the classic UNIX 'ioctl()' system call.  Applications
 * may use vorbis_encode_ctl() to query or set bitrate management or quality
 * mode details by using one of several \e request arguments detailed below.
 * vorbis_encode_ctl() must be called after one of
 * vorbis_encode_setup_managed() or vorbis_encode_setup_vbr().  When used
 * to modify settings, \ref vorbis_encode_ctl() must be called before \ref
 * vorbis_encode_setup_init().
 *
 * \param vi      Pointer to an initialized vorbis_info struct.
 *
 * \param number Specifies the desired action; See \ref encctlcodes "the list
 * of available requests".
 *
 * \param arg void * pointing to a data structure matching the request
 * argument.
 *
 * \retval 0          Success. Any further return information (such as the result of a
 * query) is placed into the storage pointed to by *arg.
 *
 * \retval OV_EINVAL  Invalid argument, or an attempt to modify a setting after
 * calling vorbis_encode_setup_init().
 *
 * \retval OV_EIMPL   Unimplemented or unknown request
 */
extern int vorbis_encode_ctl(vorbis_info *vi,int number,void *arg);

/**
 * \deprecated This is a deprecated interface. Please use vorbis_encode_ctl()
 * with the \ref ovectl_ratemanage2_arg struct and \ref
 * OV_ECTL_RATEMANAGE2_GET and \ref OV_ECTL_RATEMANAGE2_SET calls in new code.
 *
 * The \ref ovectl_ratemanage_arg structure is used with vorbis_encode_ctl()
 * and the \ref OV_ECTL_RATEMANAGE_GET, \ref OV_ECTL_RATEMANAGE_SET, \ref
 * OV_ECTL_RATEMANAGE_AVG, \ref OV_ECTL_RATEMANAGE_HARD calls in order to
 * query and modify specifics of the encoder's bitrate management
 * configuration.
*/
struct ovectl_ratemanage_arg {
  int    management_active; /**< nonzero if bitrate management is active*/
/** hard lower limit (in kilobits per second) below which the stream bitrate
    will never be allowed for any given bitrate_hard_window seconds of time.*/
  long   bitrate_hard_min;
/** hard upper limit (in kilobits per second) above which the stream bitrate
    will never be allowed for any given bitrate_hard_window seconds of time.*/
  long   bitrate_hard_max;
/** the window period (in seconds) used to regulate the hard bitrate minimum
    and maximum*/
  double bitrate_hard_window;
/** soft lower limit (in kilobits per second) below which the average bitrate
    tracker will start nudging the bitrate higher.*/
  long   bitrate_av_lo;
/** soft upper limit (in kilobits per second) above which the average bitrate
    tracker will start nudging the bitrate lower.*/
  long   bitrate_av_hi;
/** the window period (in seconds) used to regulate the average bitrate
    minimum and maximum.*/
  double bitrate_av_window;
/** Regulates the relative centering of the average and hard windows; in
    libvorbis 1.0 and 1.0.1, the hard window regulation overlapped but
    followed the average window regulation. In libvorbis 1.1 a bit-reservoir
    interface replaces the old windowing interface; the older windowing
    interface is simulated and this field has no effect.*/
  double bitrate_av_window_center;
};

/**
 * \name struct ovectl_ratemanage2_arg
 *
 * The ovectl_ratemanage2_arg structure is used with vorbis_encode_ctl() and
 * the OV_ECTL_RATEMANAGE2_GET and OV_ECTL_RATEMANAGE2_SET calls in order to
 * query and modify specifics of the encoder's bitrate management
 * configuration.
 *
*/
struct ovectl_ratemanage2_arg {
  int    management_active; /**< nonzero if bitrate management is active */
/** Lower allowed bitrate limit in kilobits per second */
  long   bitrate_limit_min_kbps;
/** Upper allowed bitrate limit in kilobits per second */
  long   bitrate_limit_max_kbps;
  long   bitrate_limit_reservoir_bits; /**<Size of the bitrate reservoir in bits */
/** Regulates the bitrate reservoir's preferred fill level in a range from 0.0
 * to 1.0; 0.0 tries to bank bits to buffer against future bitrate spikes, 1.0
 * buffers against future sudden drops in instantaneous bitrate. Default is
 * 0.1
 */
  double bitrate_limit_reservoir_bias;
/** Average bitrate setting in kilobits per second */
  long   bitrate_average_kbps;
/** Slew rate limit setting for average bitrate adjustment; sets the minimum
 *  time in seconds the bitrate tracker may swing from one extreme to the
 *  other when boosting or damping average bitrate.
 */
  double bitrate_average_damping;
};


/**
 * \name vorbis_encode_ctl() codes
 *
 * \anchor encctlcodes
 *
 * These values are passed as the \c number parameter of vorbis_encode_ctl().
 * The type of the referent of that function's \c arg pointer depends on these
 * codes.
 */
/*@{*/

/**
 * Query the current encoder bitrate management setting.
 *
 *Argument: <tt>struct ovectl_ratemanage2_arg *</tt>
 *
 * Used to query the current encoder bitrate management setting. Also used to
 * initialize fields of an ovectl_ratemanage2_arg structure for use with
 * \ref OV_ECTL_RATEMANAGE2_SET.
 */
#define OV_ECTL_RATEMANAGE2_GET      0x14

/**
 * Set the current encoder bitrate management settings.
 *
 * Argument: <tt>struct ovectl_ratemanage2_arg *</tt>
 *
 * Used to set the current encoder bitrate management settings to the values
 * listed in the ovectl_ratemanage2_arg. Passing a NULL pointer will disable
 * bitrate management.
*/
#define OV_ECTL_RATEMANAGE2_SET      0x15

/**
 * Returns the current encoder hard-lowpass setting (kHz) in the double
 * pointed to by arg.
 *
 * Argument: <tt>double *</tt>
*/
#define OV_ECTL_LOWPASS_GET          0x20

/**
 *  Sets the encoder hard-lowpass to the value (kHz) pointed to by arg. Valid
 *  lowpass settings range from 2 to 99.
 *
 * Argument: <tt>double *</tt>
*/
#define OV_ECTL_LOWPASS_SET          0x21

/**
 *  Returns the current encoder impulse block setting in the double pointed
 *  to by arg.
 *
 * Argument: <tt>double *</tt>
*/
#define OV_ECTL_IBLOCK_GET           0x30

/**
 *  Sets the impulse block bias to the the value pointed to by arg.
 *
 * Argument: <tt>double *</tt>
 *
 *  Valid range is -15.0 to 0.0 [default]. A negative impulse block bias will
 *  direct to encoder to use more bits when incoding short blocks that contain
 *  strong impulses, thus improving the accuracy of impulse encoding.
 */
#define OV_ECTL_IBLOCK_SET           0x31

/**
 *  Returns the current encoder coupling setting in the int pointed
 *  to by arg.
 *
 * Argument: <tt>int *</tt>
*/
#define OV_ECTL_COUPLING_GET         0x40

/**
 *  Enables/disables channel coupling in multichannel encoding according to arg.
 *
 * Argument: <tt>int *</tt>
 *
 *  Zero disables channel coupling for multichannel inputs, nonzer enables
 *  channel coupling.  Setting has no effect on monophonic encoding or
 *  multichannel counts that do not offer coupling.  At present, coupling is
 *  available for stereo and 5.1 encoding.
 */
#define OV_ECTL_COUPLING_SET         0x41

  /* deprecated rate management supported only for compatibility */

/**
 * Old interface to querying bitrate management settings.
 *
 * Deprecated after move to bit-reservoir style management in 1.1 rendered
 * this interface partially obsolete.

 * \deprecated Please use \ref OV_ECTL_RATEMANAGE2_GET instead.
 *
 * Argument: <tt>struct ovectl_ratemanage_arg *</tt>
 */
#define OV_ECTL_RATEMANAGE_GET       0x10
/**
 * Old interface to modifying bitrate management settings.
 *
 *  deprecated after move to bit-reservoir style management in 1.1 rendered
 *  this interface partially obsolete.
 *
 * \deprecated Please use \ref OV_ECTL_RATEMANAGE2_SET instead.
 *
 * Argument: <tt>struct ovectl_ratemanage_arg *</tt>
 */
#define OV_ECTL_RATEMANAGE_SET       0x11
/**
 * Old interface to setting average-bitrate encoding mode.
 *
 * Deprecated after move to bit-reservoir style management in 1.1 rendered
 * this interface partially obsolete.
 *
 *  \deprecated Please use \ref OV_ECTL_RATEMANAGE2_SET instead.
 *
 * Argument: <tt>struct ovectl_ratemanage_arg *</tt>
 */
#define OV_ECTL_RATEMANAGE_AVG       0x12
/**
 * Old interface to setting bounded-bitrate encoding modes.
 *
 * deprecated after move to bit-reservoir style management in 1.1 rendered
 * this interface partially obsolete.
 *
 *  \deprecated Please use \ref OV_ECTL_RATEMANAGE2_SET instead.
 *
 * Argument: <tt>struct ovectl_ratemanage_arg *</tt>
 */
#define OV_ECTL_RATEMANAGE_HARD      0x13

/*@}*/



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: stdio-based convenience library for opening/seeking/decoding
 last mod: $Id: vorbisfile.h 17182 2010-04-29 03:48:32Z xiphmont $

 ********************************************************************/

#ifndef _OV_FILE_H_
#define _OV_FILE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdio.h>

/* The function prototypes for the callbacks are basically the same as for
 * the stdio functions fread, fseek, fclose, ftell.
 * The one difference is that the FILE * arguments have been replaced with
 * a void * - this is to be used as a pointer to whatever internal data these
 * functions might need. In the stdio case, it's just a FILE * cast to a void *
 *
 * If you use other functions, check the docs for these functions and return
 * the right values. For seek_func(), you *MUST* return -1 if the stream is
 * unseekable
 */
typedef struct {
  size_t (*read_func)  (void *ptr, size_t size, size_t nmemb, void *datasource);
  int    (*seek_func)  (void *datasource, ogg_int64_t offset, int whence);
  int    (*close_func) (void *datasource);
  long   (*tell_func)  (void *datasource);
} ov_callbacks;

#ifndef OV_EXCLUDE_STATIC_CALLBACKS

/* a few sets of convenient callbacks, especially for use under
 * Windows where ov_open_callbacks() should always be used instead of
 * ov_open() to avoid problems with incompatible crt.o version linking
 * issues. */

static int _ov_header_fseek_wrap(FILE *f,ogg_int64_t off,int whence){
  if(f==NULL)return(-1);

#ifdef __MINGW32__
  return fseeko64(f,off,whence);
#elif defined (_WIN32)
  return _fseeki64(f,off,whence);
#else
  return fseek(f,off,whence);
#endif
}

/* These structs below (OV_CALLBACKS_DEFAULT etc) are defined here as
 * static data. That means that every file which includes this header
 * will get its own copy of these structs whether it uses them or
 * not unless it #defines OV_EXCLUDE_STATIC_CALLBACKS.
 * These static symbols are essential on platforms such as Windows on
 * which several different versions of stdio support may be linked to
 * by different DLLs, and we need to be certain we know which one
 * we're using (the same one as the main application).
 */

static ov_callbacks OV_CALLBACKS_DEFAULT = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           _ov_header_fseek_wrap,
  (int (*)(void *))                             fclose,
  (long (*)(void *))                            ftell
};

static ov_callbacks OV_CALLBACKS_NOCLOSE = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           _ov_header_fseek_wrap,
  (int (*)(void *))                             NULL,
  (long (*)(void *))                            ftell
};

static ov_callbacks OV_CALLBACKS_STREAMONLY = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           NULL,
  (int (*)(void *))                             fclose,
  (long (*)(void *))                            NULL
};

static ov_callbacks OV_CALLBACKS_STREAMONLY_NOCLOSE = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           NULL,
  (int (*)(void *))                             NULL,
  (long (*)(void *))                            NULL
};

#endif

#define  NOTOPEN   0
#define  PARTOPEN  1
#define  OPENED    2
#define  STREAMSET 3
#define  INITSET   4

typedef struct OggVorbis_File {
  void            *datasource; /* Pointer to a FILE *, etc. */
  int              seekable;
  ogg_int64_t      offset;
  ogg_int64_t      end;
  ogg_sync_state   oy;

  /* If the FILE handle isn't seekable (eg, a pipe), only the current
     stream appears */
  int              links;
  ogg_int64_t     *offsets;
  ogg_int64_t     *dataoffsets;
  long            *serialnos;
  ogg_int64_t     *pcmlengths; /* overloaded to maintain binary
                                  compatibility; x2 size, stores both
                                  beginning and end values */
  vorbis_info     *vi;
  vorbis_comment  *vc;

  /* Decoding working state local storage */
  ogg_int64_t      pcm_offset;
  int              ready_state;
  long             current_serialno;
  int              current_link;

  double           bittrack;
  double           samptrack;

  ogg_stream_state os; /* take physical pages, weld into a logical
                          stream of packets */
  vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
  vorbis_block     vb; /* local working space for packet->PCM decode */

  ov_callbacks callbacks;

} OggVorbis_File;


extern int ov_clear(OggVorbis_File *vf);
extern int ov_fopen(const char *path,OggVorbis_File *vf);
extern int ov_open(FILE *f,OggVorbis_File *vf,const char *initial,long ibytes);
extern int ov_open_callbacks(void *datasource, OggVorbis_File *vf,
                const char *initial, long ibytes, ov_callbacks callbacks);

extern int ov_test(FILE *f,OggVorbis_File *vf,const char *initial,long ibytes);
extern int ov_test_callbacks(void *datasource, OggVorbis_File *vf,
                const char *initial, long ibytes, ov_callbacks callbacks);
extern int ov_test_open(OggVorbis_File *vf);

extern long ov_bitrate(OggVorbis_File *vf,int i);
extern long ov_bitrate_instant(OggVorbis_File *vf);
extern long ov_streams(OggVorbis_File *vf);
extern long ov_seekable(OggVorbis_File *vf);
extern long ov_serialnumber(OggVorbis_File *vf,int i);

extern ogg_int64_t ov_raw_total(OggVorbis_File *vf,int i);
extern ogg_int64_t ov_pcm_total(OggVorbis_File *vf,int i);
extern double ov_time_total(OggVorbis_File *vf,int i);

extern int ov_raw_seek(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek_page(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_time_seek(OggVorbis_File *vf,double pos);
extern int ov_time_seek_page(OggVorbis_File *vf,double pos);

extern int ov_raw_seek_lap(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek_lap(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek_page_lap(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_time_seek_lap(OggVorbis_File *vf,double pos);
extern int ov_time_seek_page_lap(OggVorbis_File *vf,double pos);

extern ogg_int64_t ov_raw_tell(OggVorbis_File *vf);
extern ogg_int64_t ov_pcm_tell(OggVorbis_File *vf);
extern double ov_time_tell(OggVorbis_File *vf);

extern vorbis_info *ov_info(OggVorbis_File *vf,int link);
extern vorbis_comment *ov_comment(OggVorbis_File *vf,int link);

extern long ov_read_float(OggVorbis_File *vf,float ***pcm_channels,int samples,
                          int *bitstream);
extern long ov_read_filter(OggVorbis_File *vf,char *buffer,int length,
                          int bigendianp,int word,int sgned,int *bitstream,
                          void (*filter)(float **pcm,long channels,long samples,void *filter_param),void *filter_param);
extern long ov_read(OggVorbis_File *vf,char *buffer,int length,
                    int bigendianp,int word,int sgned,int *bitstream);
extern int ov_crosslap(OggVorbis_File *vf1,OggVorbis_File *vf2);

extern int ov_halfrate(OggVorbis_File *vf,int flag);
extern int ov_halfrate_p(OggVorbis_File *vf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

