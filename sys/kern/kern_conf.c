/*-
 * Parts Copyright (c) 1995 Terrence R. Lambert
 * Copyright (c) 1995 Julian R. Elischer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Terrence R. Lambert.
 * 4. The name Terrence R. Lambert may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Julian R. Elischer ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE TERRENCE R. LAMBERT BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: kern_lkm.c,v 1.15 1995/09/08 11:08:34 bde Exp $
 */

#include <sys/param.h>
#include <sys/conf.h>
extern d_open_t lkmenodev;

/*
 * (re)place an entry in the bdevsw or cdevsw table
 * return the slot used in MAJOR(*descrip)
 */
#define ADDENTRY(TTYPE,NXXXDEV) \
int TTYPE##_add(dev_t *descrip,						\
		struct TTYPE *cdeventry,				\
		cdevsw_t *oldentry)					\
{									\
	if ( (int)*decsrip == -1) {	/* auto (0 is valid) */		\
		/*							\
		 * Search the table looking for a slot...		\
		 */							\
		for (i = 0; i < NXXXDEV; i++)				\
			if (TTYPE[i].d_open == lkmenodev)		\
				break;		/* found it! */		\
		/* out of allocable slots? */				\
		if (i == NXXXDEV) {					\
			return ENFILE;					\
		}							\
	} else {				/* assign */		\
		if (i < 0 || i >= NXXXDEV) {				\
			return EINVAL;					\
		}							\
	}								\
									\
	/* save old */							\
        if (oldentry) {							\
		bcopy(&TTYPE[i], oldentry, sizeof(struct TTYPE));	\
	}								\
	/* replace with new */						\
	bcopy(cdeventry, &TTYPE[i], sizeof(struct TTYPE));		\
									\
	/* done! */							\
	*descrip = makedev(i,0);					\
	return 0							\
} \

ADDENTRY(cdevsw, nblkdev)
ADDENTRY(bdevsw, nchrdev)
