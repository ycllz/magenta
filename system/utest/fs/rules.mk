# Copyright 2016 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_TYPE := usertest

MODULE_NAME := fs-test

MODULE_SRCS := \
    $(LOCAL_DIR)/filesystems.c \
    $(LOCAL_DIR)/main.c \
    $(LOCAL_DIR)/misc.c \
    $(LOCAL_DIR)/wrap.c \
    $(LOCAL_DIR)/test-attr.c \
    $(LOCAL_DIR)/test-append.c \
    $(LOCAL_DIR)/test-basic.c \
    $(LOCAL_DIR)/test-directory.c \
    $(LOCAL_DIR)/test-link.c \
    $(LOCAL_DIR)/test-maxfile.c \
    $(LOCAL_DIR)/test-overflow.c \
    $(LOCAL_DIR)/test-persist.c \
    $(LOCAL_DIR)/test-rw-workers.c \
    $(LOCAL_DIR)/test-rename.c \
    $(LOCAL_DIR)/test-random-op.c \
    $(LOCAL_DIR)/test-sync.c \
    $(LOCAL_DIR)/test-truncate.c \
    $(LOCAL_DIR)/test-unlink.c \

MODULE_LDFLAGS := --wrap open --wrap unlink --wrap stat --wrap mkdir
MODULE_LDFLAGS += --wrap rename --wrap truncate --wrap opendir
MODULE_LDFLAGS += --wrap utimes --wrap link --wrap symlink --wrap rmdir

MODULE_LIBS := \
    ulib/mxio \
    ulib/c \
    ulib/fs-management \
    ulib/launchpad \
    ulib/magenta \
    ulib/unittest \

include make/module.mk
