#
# Copyright (C) 2025 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from the common Open Source product configuration
$(call inherit-product, $(SRC_TARGET_DIR)/product/base.mk)

# Inherit from this product for devices that support only 64-bit apps using:
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)

# Configure virtual_ab compression.mk
$(call inherit-product, $(SRC_TARGET_DIR)/product/virtual_ab_ota/compression.mk)

# Enable Project Quotas and Case Folding
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulated_storage.mk)

# Inherit some common twrp stuff.
$(call inherit-product, vendor/twrp/config/common.mk)

# some OrangeFox-specific settings
$(call inherit-product, $(LOCAL_PATH)/fox_sm86xx.mk)

# Installs gsi keys into ramdisk, to boot a developer GSI with verified boot.
$(call inherit-product, $(SRC_TARGET_DIR)/product/gsi_keys.mk)

# Shipping API level
BOARD_SHIPPING_API_LEVEL    := 32
PRODUCT_SHIPPING_API_LEVEL  := 32
PRODUCT_TARGET_VNDK_VERSION := 32

# Dynamic partitions
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# Kernel
PRODUCT_OTA_ENFORCE_VINTF_KERNEL_REQUIREMENTS   := false
PRODUCT_ENABLE_UFFD_GC                          := true

# OTA certs
PRODUCT_EXTRA_RECOVERY_KEYS += \
	$(DEVICE_PATH)/security/local_OTA \
	$(DEVICE_PATH)/security/special_OTA

# Soong namespaces
PRODUCT_SOONG_NAMESPACES += $(DEVICE_PATH)

LOCAL_STATIC_LIBRARIES := libfs_mgr liblog libpropertyutils libbase libcutils libutils libc++