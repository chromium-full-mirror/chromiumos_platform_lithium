/* Copyright 2020 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "osutils/file.h"
#include "unit.h"

#define TEST_DATA_FILE_CONTENTS "abcdefghijklmnopqrstuvwxyz1234567890"

DEFTEST("lithium.osutils.file.make_tmpfile_and_read", {})
{
	char *contents = NULL;
	char *file_path = NULL;
	ssize_t size_read;

	if (!EXPECT(li_make_tmpfile(TEST_DATA_FILE_CONTENTS,
				    sizeof(TEST_DATA_FILE_CONTENTS),
				    &file_path) == 0))
		goto exit;

	/* The file path should start with P_tmpdir (usually /tmp). */
	EXPECT(!strncmp(file_path, P_tmpdir, strlen(P_tmpdir)));

	/* We should read the number of bytes we wrote. */
	size_read = li_read_file(file_path, &contents);
	if (!EXPECT(size_read == sizeof(TEST_DATA_FILE_CONTENTS)))
		goto exit;

	/* The contents should match. */
	EXPECT(!memcmp(contents, TEST_DATA_FILE_CONTENTS, size_read));

exit:
	/* We should be able to delete the temporary file when done. */
	if (file_path)
		EXPECT(unlink(file_path) == 0);

	free(file_path);
	free(contents);
}
