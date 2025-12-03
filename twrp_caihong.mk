#
# Copyright (C) 2025 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

DEVICE_PATH := device/oplus/caihong

# Inherit from device.mk configuration
$(call inherit-product, $(DEVICE_PATH)/device.mk)

## Device identifier
PRODUCT_DEVICE  := caihong
PRODUCT_NAME    := twrp_caihong
PRODUCT_BRAND   := oplus

# Theme
TW_STATUS_ICONS_ALIGN   := center
TW_Y_OFFSET             := 116
TW_H_OFFSET             := -116
