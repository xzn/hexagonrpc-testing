/*
 * FastRPC memory mapping interface implementation
 *
 * Copyright (C) 2024 The Sensor Shell Contributors
 *
 * This file is part of sensh.
 *
 * Sensh is free software: you can redistribute it and/or modify
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <misc/fastrpc.h>
#include <sys/ioctl.h>

#include "aee_error.h"
#include "sns_registry.h"
#include "interfaces/sns_registry.def"
#include "listener.h"

static uint32_t sns_registry_get_property(void *data,
					  const struct fastrpc_io_buffer *inbufs,
					  struct fastrpc_io_buffer *outbufs)
{
	if (((const char *) inbufs[1].p)[inbufs[1].s - 1] != 0) {
		fprintf(stderr, "Property not nul-terminated: %u\n", inbufs[1].s);
		return AEE_EBADPARM;
	}

	fprintf(stderr, "Property: %s\n", (const char *) inbufs[1].p);

	memset(outbufs[0].p, 0, outbufs[0].s);

	return 0;
}

struct fastrpc_interface *fastrpc_sns_registry_init(void)
{
	struct fastrpc_interface *iface;

	iface = malloc(sizeof(*iface));
	if (iface == NULL)
		return NULL;

	memcpy(iface, &sns_registry_interface, sizeof(*iface));

	return iface;
}

void fastrpc_sns_registry_deinit(struct fastrpc_interface *iface)
{
}

static const struct fastrpc_function_impl sns_registry_procs[] = {
	{ .def = &sns_registry_get_property_def, .impl = sns_registry_get_property, },
};

const struct fastrpc_interface sns_registry_interface = {
	.name = "sns_registry",
	.n_procs = 1,
	.procs = sns_registry_procs,
};
