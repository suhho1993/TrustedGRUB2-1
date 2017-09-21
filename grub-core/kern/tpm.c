/* Begin TCG Extension */

/* Common implementations for i386-pc and efi */

/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2014,2015  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/tpm.h>
#include <grub/err.h>
#include <grub/i18n.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/tpm.h>
#include <grub/term.h>
void
print_sha1( grub_uint8_t *inDigest ) {

	/* print SHA1 hash of input */
	unsigned int j;
	for( j = 0; j < SHA1_DIGEST_SIZE; j++ ) {
		grub_printf( "%02x", inDigest[j] );
	}
}
grub_err_t

grub_tpm_measure (unsigned char *buf, grub_size_t size, grub_uint8_t pcr,
		  const char *kind, const char *description)
{
  grub_err_t ret;
  char *desc = grub_xasprintf("%s %s", kind, description);
  if (!desc)
    return GRUB_ERR_OUT_OF_MEMORY;
  ret = grub_tpm_log_event(buf, size, pcr, desc);
  grub_free(desc);
  return ret;
}
