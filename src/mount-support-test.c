/*
 * Copyright (C) 2015 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mount-support.h"
#include "mount-support.c"
#include "mount-support-nvidia.h"
#include "mount-support-nvidia.c"

#include <glib.h>

static void replace_slashes_with_NUL(char *path, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		if (path[i] == '/')
			path[i] = '\0';
	}
}

static void test_get_nextpath__typical()
{
	char path[] = "/some/path";
	int offset = 0;
	int fulllen = strlen(path);

	// Prepare path for useage with get_nextpath() by replacing
	// all path separators with the NUL byte.
	replace_slashes_with_NUL(path, fulllen);

	// Run get_nextpath a few times to see what happens.
	char *result;
	result = get_nextpath(path, &offset, fulllen);
	g_assert_cmpstr(result, ==, "some");
	result = get_nextpath(path, &offset, fulllen);
	g_assert_cmpstr(result, ==, "path");
	result = get_nextpath(path, &offset, fulllen);
	g_assert_cmpstr(result, ==, NULL);
}

static void test_get_nextpath__weird()
{
	char path[] = "..///path";
	int offset = 0;
	int fulllen = strlen(path);

	// Prepare path for useage with get_nextpath() by replacing
	// all path separators with the NUL byte.
	replace_slashes_with_NUL(path, fulllen);

	// Run get_nextpath a few times to see what happens.
	char *result;
	result = get_nextpath(path, &offset, fulllen);
	g_assert_cmpstr(result, ==, "path");
	result = get_nextpath(path, &offset, fulllen);
	g_assert_cmpstr(result, ==, NULL);
}

static void __attribute__ ((constructor)) init()
{
	g_test_add_func("/mount/get_nextpath/typical",
			test_get_nextpath__typical);
	g_test_add_func("/mount/get_nextpath/weird", test_get_nextpath__weird);
}
