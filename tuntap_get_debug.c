/*
 * Copyright (c) 2012-2013 Tristan Le Guern <leguern AT medu DOT se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "tuntap.h"

#include <sys/types.h>
#include <sys/ioctl.h>

#if defined Linux
# include <linux/if_tun.h>
#else
# include <net/if.h>
# if defined DragonFly
#  include <net/tun/if_tun.h>
# elif !defined Darwin
#  include <net/if_tun.h>
# endif
#endif

#include <stdlib.h>
#include <string.h>

#include "tuntap_private.h"

static int
tuntap_sys_get_debug(struct device *dev) {
#if defined TUNGDEBUG
	int get;

	get = 0;
	if (ioctl(dev->tun_fd, TUNGDEBUG, &get) == -1) {
		tuntap_log(TUNTAP_LOG_ERR, "Can't get debug status");
		return -1;
	}
	return get;
#else
	(void)dev;
	tuntap_log(TUNTAP_LOG_NOTICE,
	    "Your system does not support tuntap_get_debug()");
	return -1;
#endif
}

int
tuntap_get_debug(struct device *dev) {
	/* Only accept started device */
	if (dev->tun_fd == -1) {
		tuntap_log(TUNTAP_LOG_NOTICE, "Device is not started");
		return 0;
	}

	return tuntap_sys_get_debug(dev);
}
