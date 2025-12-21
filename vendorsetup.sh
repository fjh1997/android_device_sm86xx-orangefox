# Copyright (c) 2023 The Android Open Source Project
# Copyright (C) 2023 SebaUbuntu's TWRP device tree generator 4-#
# SPDX-License-Identifier: Apache-2.0
#
# 关于橙狐变量详见 fox_12.1/vendor/recovery/orangefox_build_vars.txt 
    echo -e "\x1b[96mAdontoo: 开始加载OrangeFox配置变量...\x1b[m"
 
    export TW_DEFAULT_LANGUAGE="zh_CN"  # 默认中文语言
    export LC_ALL="C"
    export OF_STATUS_H=120  # 状态栏高度（像素）
	export FOX_AB_DEVICE=1
	export FOX_USE_TAR_BINARY=1
	export FOX_USE_SED_BINARY=1
	export FOX_USE_LZ4_BINARY=1
	export FOX_USE_ZSTD_BINARY=1
	export FOX_USE_DATE_BINARY=1
	export FOX_DELETE_AROMAFM=1
	#export FOX_VANILLA_BUILD=1
    export FOX_MAINTAINER_PATCH_VERSION=$(date +%y%m%d)  # 维护者补丁版本（按日期生成）
    export OF_MAINTAINER="Nanya"  # 维护者名称
    export FOX_MOVE_MAGISK_INSTALLER_TO_RAMDISK=1  # 将Magisk安装包移至ramdisk
    #export OF_SKIP_FBE_DECRYPTION_SDKVERSION=32  # Android 12L及以上跳过FBE解密（避免卡LOGO）
    export OF_OPTIONS_LIST_NUM=9  # 安装选项列表最大数量（6个，超出显示滚动条）
	export FOX_USE_GREP_BINARY=1
	export FOX_USE_BUSYBOX_BINARY=1
	export FOX_USE_XZ_UTILS=1
	export FOX_VIRTUAL_AB_DEVICE=1
	export FOX_ALLOW_EARLY_SETTINGS_LOAD=1
	export FOX_USE_UPDATED_MAGISKBOOT=1
	export FOX_MOVE_MAGISK_INSTALLER_TO_RAMDISK=1
	export FOX_USE_FSCK_EROFS_BINARY=1
	export FOX_USE_PATCHELF_BINARY=1
	export OF_DYNAMIC_FULL_SIZE=14578294784  
    export FOX_VARIANT="OPLUS_PINEAPPLE"  
	export TARGET_DEVICE_ALT="OPD2404,OP5D77L1"
    export FOX_USE_DMSETUP=1
	export FOX_ENABLE_KERNELSU_SUPPORT=1
	export FOX_ENABLE_KERNELSU_NEXT_SUPPORT=1
	export FOX_ENABLE_SUKISU_SUPPORT=1
	export FOX_MAINTAINER_PATCH_VERSION=1
	export FOX_REPLACE_TOOLBOX_GETPROP=1 
    export FOX_USE_BASH_SHELL=1 
    export FOX_ENABLE_APP_MANAGER=1 
    export OF_ENABLE_LPTOOLS=1  # 启用逻辑分区工具
    export OF_ENABLE_ALL_PARTITION_TOOLS=1  # 启用所有分区管理工具
    export OF_ENABLE_FS_COMPRESSION=1  # 启用文件系统压缩
    export OF_QUICK_BACKUP_LIST="/boot;/data;"  # 快速备份默认分区（boot+data）
    export OF_FLASHLIGHT_ENABLE=0  # 关闭闪光灯功能（按需启用）
        
    F=$(find "device" -maxdepth 2 -name "sm86xx")
    \cp -fp bootable/recovery/gui/theme/portrait_hdpi/splash.xml "$F"/recovery/root/twres/splash.xml
    sed -i 's/value="#D34E38"/value="#000000"/g' "$F"/recovery/root/twres/splash.xml  # 替换橙色为黑色
    sed -i 's/value="#FF8038"/value="#000000"/g' "$F"/recovery/root/twres/splash.xml  # 替换浅橙色为黑色
 
    echo -e "\x1b[96mAdontoo: 所有OrangeFox配置变量加载完毕！\x1b[m"