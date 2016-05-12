/*
 *	Dynamic Link Library by Parra Studios
 *	Copyright (C) 2009 - 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A library for dynamic loading and linking shared objects at run-time.
 *
 */

/* -- Headers -- */

#include <metacall/metacall-version.h>

#include <dynlink/dynlink.h>
#include <dynlink/dynlink_impl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* -- Definitions -- */

#define DYNLINK_NAME_SIZE			0xFF					/**< Dynamically linked shared object name size */

/* -- Type definitions -- */

typedef char dynlink_name_impl[DYNLINK_NAME_SIZE];			/**< Allocated copy of dynamically linked shared object name */

/* -- Member data -- */

typedef struct dynlink_type
{
	dynlink_name_impl				name;					/**< Dynamically linked shared object name */
	dynlink_flags					flags;					/**< Dynamically linked shared object flags */
	dynlink_impl					impl;					/**< Dynamically linked shared object loader implementation */

} * dynlink;

/* -- Methods -- */

const char * dynlink_extension(void)
{
	return dynlink_impl_extension();
}

dynlink dynlink_load(dynlink_name name, dynlink_flags flags)
{
	if (name != NULL)
	{
		dynlink handle = malloc(sizeof(struct dynlink_type));

		if (handle != NULL)
		{
			strncpy(handle->name, name, DYNLINK_NAME_SIZE);

			strncat(handle->name, dynlink_impl_extension(), DYNLINK_NAME_SIZE);

			handle->flags = flags;

			handle->impl = dynlink_impl_load(handle);

			if (handle->impl != NULL)
			{
				return handle;
			}

			free(handle);
		}
	}

	return NULL;
}

dynlink_name dynlink_get_name(dynlink handle)
{
	if (handle != NULL)
	{
		return handle->name;
	}

	return NULL;
}

dynlink_flags dynlink_get_flags(dynlink handle)
{
	if (handle != NULL)
	{
		return handle->flags;
	}

	return 0;
}

int dynlink_symbol(dynlink handle, dynlink_symbol_name symbol_name, dynlink_symbol_addr * symbol_address)
{
	if (handle != NULL && handle->impl != NULL && symbol_name != NULL && symbol_address != NULL)
	{
		return dynlink_impl_symbol(handle, handle->impl, symbol_name, symbol_address);
	}

	return 1;
}

void dynlink_unload(dynlink handle)
{
	if (handle != NULL)
	{
		dynlink_impl_unload(handle, handle->impl);

		free(handle);
	}
}

void dynlink_print_info(void)
{
	printf("Dynamic Link Library " METACALL_VERSION "\n");
	printf("Copyright (c) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>\n");

	#ifdef DYNLINK_STATIC_DEFINE
		printf("Compiled as static library type\n");
	#else
		printf("Compiled as shared library type\n");
	#endif
}