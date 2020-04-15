/* Copyright 2020 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef LITHIUM_OSUTILS_FILE_H_
#define LITHIUM_OSUTILS_FILE_H_

/**
 * File Utilities
 * ==============
 */

#include <stddef.h>

/**
 * Read a regular file (by name) into a newly allocated buffer. The
 * buffer should be freed by the caller.
 *
 * :param path: The path to the file.
 * :param data_ptr: Output pointer for newly allocated buffer.
 *
 * :return: On success, the number of bytes read. -1 on failure.
 */
ssize_t li_read_file(const char *path, char **data_ptr);

/**
 * Read a regular file (by file descriptor) into a newly allocated
 * buffer. The buffer should be freed by the caller.
 *
 * :param fd: The file descriptor to read.
 * :param data_ptr: Output pointer for newly allocated buffer.
 *
 * :return: On success, the number of bytes read. -1 on failure.
 */
ssize_t li_read_file_fd(int fd, char **data_ptr);

/**
 * Write a temporary file with the specified contents.
 *
 * :param buf: The buffer to write.
 * :param buf_sz: The size of the buffer.
 * :param path_out: Output pointer which will be set to the name of
 * the file created. This parameter should be freed by the caller.
 *
 * :return: 0 on success, -1 on failure.
 */
int li_make_tmpfile(const void *buf, size_t buf_sz, char **path_out);

/**
 * Write all the bytes from a buffer into the file descriptor
 * specified.
 *
 * :param fd: The file descriptor to write to.
 * :param buf: The buffer to write.
 * :param buf_sz: The size of the buffer.
 *
 * :return: 0 on success, -1 on failure.
 */
int li_write_all(int fd, const void *buf, size_t buf_sz);

#endif /* LITHIUM_OSUTILS_FILE_H_ */
