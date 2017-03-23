// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// for S_IF*
#define _XOPEN_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/time.h>

#include <mxio/vfs.h>

#define FN(n) __wrap_##n
#define FL(n) __real_##n

int status_to_errno(mx_status_t status) {
    switch (status) {
    case NO_ERROR:
        return 0;
    default:
        return EIO;
    }
}

#define FAIL(err)            \
    do {                     \
        return err ? -1 : 0; \
    } while (0)
#define STATUS(status) \
    FAIL(status_to_errno(status))
#define PATH_WRAP(path_in, path_out)      \
    do {                                  \
        if (wrap_path(path_in, path_out)) \
            FAIL(EINVAL);                 \
    } while (0)
#define DO_REAL(name, args...)            \
    do {                                  \
        int status = __real_##name(args); \
        if (status < 0)                   \
            STATUS(status);               \
        return status;                    \
    } while (0)

#define PATH_PREFIX "::"
#define PREFIX_SIZE 2

// Occasionally, we test with paths that push against PATH_MAX.
// Our 'wrap' functions should *not* be the cause of error; rather, we should see errors emerging
// from either libc or our filesystems themselves.
#define WPATH_MAX (PATH_MAX * 2)

extern char* test_root_path;

static inline int wrap_path(const char* path_in, char* path_out) {
    path_out[0] = '\0';
    int bytes_left = WPATH_MAX - 1;
    if (strncmp(path_in, PATH_PREFIX, PREFIX_SIZE) || (test_root_path == NULL)) {
        // Unfiltered path
        strncat(path_out, path_in, bytes_left);
        return 0;
    }
    // Remove the "::" prefix, and substitute the actual path
    strncat(path_out, test_root_path, bytes_left);
    bytes_left -= strlen(test_root_path);
    strncat(path_out, "/", bytes_left);
    bytes_left -= 1;
    strncat(path_out, path_in + PREFIX_SIZE, bytes_left);

    return 0;
}

int FL(open)(const char* path, int flags, mode_t mode);
int FN(open)(const char* path, int flags, mode_t mode) {
    char real_path[WPATH_MAX];
    PATH_WRAP(path, real_path);
    DO_REAL(open, real_path, flags, mode);
}

int FL(opendir)(const char* path);
int FN(opendir)(const char* path) {
    char real_path[WPATH_MAX];
    PATH_WRAP(path, real_path);
    DO_REAL(opendir, real_path);
}

int FL(mkdir)(const char* path, mode_t mode);
int FN(mkdir)(const char* path, mode_t mode) {
    char real_path[WPATH_MAX];
    PATH_WRAP(path, real_path);
    DO_REAL(mkdir, real_path, mode);
}

int FL(unlink)(const char* path);
int FN(unlink)(const char* path) {
    char real_path[WPATH_MAX];
    PATH_WRAP(path, real_path);
    DO_REAL(unlink, real_path);
}

int FL(rmdir)(const char* path);
int FN(rmdir)(const char* path) {
    char real_path[WPATH_MAX];
    PATH_WRAP(path, real_path);
    DO_REAL(rmdir, real_path);
}

int FL(truncate)(const char* path, off_t len);
int FN(truncate)(const char* path, off_t len) {
    char real_path[WPATH_MAX];
    PATH_WRAP(path, real_path);
    DO_REAL(truncate, real_path, len);
}

int FL(rename)(const char* oldpath, const char* newpath);
int FN(rename)(const char* oldpath, const char* newpath) {
    char real_oldpath[WPATH_MAX];
    char real_newpath[WPATH_MAX];
    PATH_WRAP(oldpath, real_oldpath);
    PATH_WRAP(newpath, real_newpath);
    DO_REAL(rename, real_oldpath, real_newpath);
}

int FL(link)(const char* oldpath, const char* newpath);
int FN(link)(const char* oldpath, const char* newpath) {
    char real_oldpath[WPATH_MAX];
    char real_newpath[WPATH_MAX];
    PATH_WRAP(oldpath, real_oldpath);
    PATH_WRAP(newpath, real_newpath);
    DO_REAL(link, real_oldpath, real_newpath);
}

int FL(symlink)(const char* oldpath, const char* newpath);
int FN(symlink)(const char* oldpath, const char* newpath) {
    char real_oldpath[WPATH_MAX];
    char real_newpath[WPATH_MAX];
    PATH_WRAP(oldpath, real_oldpath);
    PATH_WRAP(newpath, real_newpath);
    DO_REAL(symlink, real_oldpath, real_newpath);
}

int FL(stat)(const char* fn, struct stat* s);
int FN(stat)(const char* fn, struct stat* s) {
    char real_fn[WPATH_MAX];
    PATH_WRAP(fn, real_fn);
    DO_REAL(stat, real_fn, s);
}

int FL(utimes)(const char* fn, struct timeval t[2]);
int FN(utimes)(const char* fn, struct timeval t[2]) {
    char real_fn[PATH_MAX];
    PATH_WRAP(fn, real_fn);
    DO_REAL(utimes, real_fn, t);
}
