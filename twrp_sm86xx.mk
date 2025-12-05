#
# Copyright (C) 2025 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

DEVICE_PATH := device/oplus/sm86xx

# Inherit from munch device
$(call inherit-product, $(DEVICE_PATH)/device.mk)
  
## Device identifier
PRODUCT_DEVICE  := sm86xx
PRODUCT_NAME    := orangefox_sm86xx
PRODUCT_BRAND := OnePlus
PRODUCT_MANUFACTURER := OnePlus
# Theme
TW_STATUS_ICONS_ALIGN   := center
TW_Y_OFFSET             := 116
TW_H_OFFSET             := -116
