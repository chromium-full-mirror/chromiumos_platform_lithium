/* Copyright 2020 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#define POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "osutils/file.h"

ssize_t li_read_file(const char *path, char **data_ptr)
{
	ssize_t rv;
	int fd;

	if (!data_ptr) {
		errno = EFAULT;
		return -1;
	}

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		*data_ptr = NULL;
		return -1;
	}

	rv = li_read_file_fd(fd, data_ptr);
	if (close(fd) < 0)
		rv = -1;
	return rv;
}

ssize_t li_read_file_fd(int fd, char **data_ptr)
{
	ssize_t read_rv;
	size_t bytes_to_read;
	char *buf;
	char *bufptr;
	off_t file_size;

	if (!data_ptr) {
		errno = EFAULT;
		return -1;
	}

	*data_ptr = NULL;

	if ((file_size = lseek(fd, 0, SEEK_END)) < 0)
		return -1;

	if (lseek(fd, 0, SEEK_SET) < 0)
		return -1;

	buf = malloc(file_size);
	if (!buf)
		return -1;

	bytes_to_read = file_size;
	bufptr = buf;
	while (bytes_to_read) {
		read_rv = read(fd, bufptr, bytes_to_read);
		if (read_rv < 0)
			return -1;
		if (read_rv == 0)
			break;
		bytes_to_read -= read_rv;
		bufptr += read_rv;
	}

	*data_ptr = buf;
	return file_size;
}

int li_make_tmpfile(const void *buf, size_t buf_sz, char **path_out)
{
	int fd;
	int rv;

	if (!path_out) {
		errno = EFAULT;
		return -1;
	}

	*path_out = strdup(P_tmpdir "/li_tmp.XXXXXX");
	fd = mkstemp(*path_out);
	if (fd < 0)
		goto fail;

	rv = li_write_all(fd, buf, buf_sz);
	if (rv < 0)
		goto fail;
	return rv;

fail:
	free(*path_out);
	*path_out = NULL;
	return -1;
}

int li_write_all(int fd, const void *buf, size_t buf_sz)
{
	ssize_t write_rv;

	if (!buf && buf_sz > 0) {
		errno = EFAULT;
		return -1;
	}

	while (buf_sz > 0) {
		write_rv = write(fd, buf, buf_sz);
		if (write_rv < 0)
			return -1;
		buf_sz -= write_rv;
		buf += write_rv;
	}

	return 0;
}
