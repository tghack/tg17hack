#include "libc_base.h"
#include <stdio.h>
#include <link.h>
#include <unistd.h>
#include <string.h>

struct addr_info {
	void *base;
	size_t size;
};

#define __unused __attribute__((unused))
/* round up n to nearest multiple of size */
#define round_up(n, size) (((size) - ((n) % (size))) + (n))

static inline int callback(struct dl_phdr_info *info, size_t __unused size, void *data)
{
	if (!strstr(info->dlpi_name, "libc"))
		return 0;

	struct addr_info *addr = (struct addr_info *)data;
	addr->base = (void *)info->dlpi_addr;
	uint64_t mem_size = 0;

	long page_size = sysconf(_SC_PAGESIZE);

	for (int i = 0; i < info->dlpi_phnum; i++) {
		/*
		 * XXX: we pick the first and best PT_LOAD section,
		 * is this correct?
		 */
		if (info->dlpi_phdr[i].p_type != PT_LOAD)
			continue;
		mem_size = round_up(info->dlpi_phdr[i].p_memsz, page_size);
		break;
	}

	addr->size = mem_size;

	return 0;
}

uint64_t get_libc_addr(void)
{
	struct addr_info info = { 0 };
	dl_iterate_phdr(callback, &info);
	return (uint64_t)info.base;
}
