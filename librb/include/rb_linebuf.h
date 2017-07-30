/*
 *  ircd-ratbox: A slightly useful ircd.
 *  linebuf.h: A header for the linebuf code.
 *
 *  Copyright (C) 1990 Jarkko Oikarinen and University of Oulu, Co Center
 *  Copyright (C) 1996-2002 Hybrid Development Team
 *  Copyright (C) 2002-2005 ircd-ratbox development team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
 *
 */

#ifndef RB_LIB_H
# error "Do not use linebuf.h directly"
#endif

#ifndef __LINEBUF_H__
#define __LINEBUF_H__

#define LINEBUF_COMPLETE        0
#define LINEBUF_PARTIAL         1
#define LINEBUF_PARSED          0
#define LINEBUF_RAW             1

struct _buf_line;
struct _buf_head;

/* IRCv3 tags (512 bytes) + RFC1459 message (510 bytes) */
#define LINEBUF_TAGSLEN         512     /* IRCv3 message tags */
#define LINEBUF_DATALEN         510     /* RFC1459 message data */
#define LINEBUF_SIZE            (512 + 510)
#define CRLF_LEN                2

typedef struct _buf_line
{
	char buf[LINEBUF_SIZE + CRLF_LEN + 1];
	uint8_t terminated;	/* Whether we've terminated the buffer */
	uint8_t raw;		/* Whether this linebuf may hold 8-bit data */
	int len;		/* How much data we've got */
	int refcount;		/* how many linked lists are we in? */
} buf_line_t;

typedef struct _buf_head
{
	rb_dlink_list list;	/* the actual dlink list */
	int len;		/* length of all the data */
	int alloclen;		/* Actual allocated data length */
	int writeofs;		/* offset in the first line for the write */
	int numlines;		/* number of lines */
} buf_head_t;



/* they should be functions, but .. */
#define rb_linebuf_len(x)		((x)->len)
#define rb_linebuf_alloclen(x)	((x)->alloclen)
#define rb_linebuf_numlines(x)	((x)->numlines)

void rb_linebuf_init(size_t heap_size);
void rb_linebuf_newbuf(buf_head_t *);
void rb_linebuf_donebuf(buf_head_t *);
int rb_linebuf_parse(buf_head_t *, char *, int, int);
int rb_linebuf_get(buf_head_t *, char *, int, int, int);
/* "msg" is limited to RFC1459 message size */
void rb_linebuf_put_msgf(buf_head_t *, const char *, ...) AFP(2,3);
void rb_linebuf_put_vmsg(buf_head_t *, const char *, va_list *);
void rb_linebuf_put_vmsg_prefixf(buf_head_t *, const char *, va_list *, const char *, ...) AFP(4,5);
/* "tags" has a prefix that contains tags AND messages, it must
 * specify the correct total buffer length to enforce the limit
 * of the RFC1459 message size */
void rb_linebuf_put_vtags_prefix(buf_head_t *, const char *, va_list *, size_t, const char *);
void rb_linebuf_put_vtags_prefixf(buf_head_t *, const char *, va_list *, size_t, const char *, ...) AFP(5,6);
void rb_linebuf_attach(buf_head_t *, buf_head_t *);
void rb_count_rb_linebuf_memory(size_t *, size_t *);
int rb_linebuf_flush(rb_fde_t *F, buf_head_t *);


#endif
