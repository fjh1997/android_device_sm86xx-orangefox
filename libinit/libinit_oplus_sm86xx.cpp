//
// Copyright (C) 2025 The Android Open Source Project
//
// SPDX-License-Identifier: Apache-2.0
//

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <fs_mgr.h>
#include <unordered_map>

using android::base::GetProperty;
using android::base::Split;
using android::base::Trim;

const std::unordered_map<int, std::string> kRegionSuffixMap = {
    {27,    "IN"},
    {55,    "RU"},
    {68,    "EEA"},
    {151,   ""},    // CN
    {161,   "NA"},
    {0,     ""},    // Default
};

struct ModelInfo {
    const char* brand;              // ro.product.brand
    const char* device;             // ro.product.device
    const char* manufacturer;       // ro.product.manufacturer
    const char* model;              // ro.product.model
    const char* base_name;          // ro.product.name  w/o region suffix
    const char* twversion;         // vendor.display.enable_spr
    const char* supportSpr;         // vendor.display.enable_spr
};

const std::unordered_map<int, ModelInfo> kModelInfoMap = {
    {23851, {"OnePlus", "OP5D2BL1", "OnePlus", "PKG110",    "PKG110",  "OnePlus-ACE-5",         "0"}},    
    {23867, {"OnePlus", "OP5D3BL1", "OnePlus", "CPH2691",   "CPH2691", "OnePlus-13-R",          "0"}},
    {23868, {"OnePlus", "OP5D3BL1", "OnePlus", "CPH2645",   "CPH2645", "OnePlus-13-R",          "0"}},
    {23869, {"OnePlus", "OP5D3BL1", "OnePlus", "CPH2647",   "CPH2647", "OnePlus-13-R",          "0"}},
    {23927, {"OnePlus", "OP5D77L1", "OnePlus", "OPD2404",   "OPD2404", "OnePlus-Pad-Pro",       "0"}},
    {23803, {"OnePlus", "OP5CFBL1", "OnePlus", "PJF110",    "PJF110",  "OnePlus-ACE-3-V",       "0"}},
    {24211, {"OnePlus", "OP5E93L1", "OnePlus", "CPH2661",   "CPH2661", "OnePlus-NORD-4",        "0"}},
    {23814, {"OnePlus", "OP5D06L1", "OnePlus", "PJX110",    "PJX110",  "OnePlus-ACE-3-Pro",     "0"}},
    {23631, {"Realme",  "RE5C4FL1", "Realme",  "RMX3800",   "RMX3800", "Realme-GT-6",           "0"}},
    {22825, {"OnePlus", "OP5929L1", "OnePlus", "PJD110",    "PJD110",  "Oneplus-12",            "0"}},
    {22861, {"OnePlus", "OP595DL1", "OnePlus", "CPH2573",   "CPH2573", "Oneplus-12",            "0"}},
    {22111, {"OPPO",    "OP565FL1", "OPPO",    "PHY110",    "PHY110",  "OPPO-Find-X7-Ultra",    "0"}},
    {22112, {"OPPO",    "OP5660L1", "OPPO",    "PHY120",    "PHY120",  "OPPO-Find-X7-Ultra-SL", "0"}},
    {0,     {"OPLUS",   "SM86XX",   "OPLUS",   "SM86XX",    "SM86XX",  "OPLUS",                 "0"}}, // Default
};

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

void SetupModelProperties(const ModelInfo& info, const std::string& region) {
    std::string name = info.base_name + region;
    struct PropPair {
        const char* key;
        const char* value;
    } props[] = {
        {"ro.product.brand",                info.brand},
        {"ro.product.device",               info.device},
        {"ro.product.manufacturer",         info.manufacturer},
        {"ro.product.model",                info.model},
        {"ro.product.name",                 name.c_str()},
        {"vendor.display.enable_spr",       info.supportSpr},
        {"ro.product.system.device",        info.model},
        {"ro.product.product.model",        info.model},
        {"ro.product.system.model",         info.model},
        {"ro.product.system_ext.model",     info.model},
        {"ro.product.vendor.model",         info.model},
        {"ro.product.odm.model",            info.model},
        {"ro.product.odm.model",            info.model},
        {"ro.product.vendor.device",        info.device},
        {"ro.product.odm.device",           info.device},
        {"ro.product.product.device",       info.device},
        {"ro.product.system_ext.device",    info.device},
        {"ro.twrp.device_version",          info.twversion},
        {"ro.build.date.utc",               "0"},
    };
    for (const auto& p : props) {
        OverrideProperty(p.key, p.value);
    }
}

// 新增：直接读取/proc/cmdline并解析目标参数（替代GetKernelCmdline）
std::string ReadCmdlineParam(const std::string& param_name) {
    const char* cmdline_path = "/proc/cmdline";
    char buf[4096] = {0};
    int fd = open(cmdline_path, O_RDONLY);
    if (fd < 0) {
        LOG(WARNING) << "Failed to open " << cmdline_path << ", errno: " << errno;
        return "0";
    }

    ssize_t read_len = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (read_len < 0) {
        LOG(WARNING) << "Failed to read " << cmdline_path << ", errno: " << errno;
        return "0";
    }

    // 解析cmdline参数（格式：key=value 或 key）
    std::vector<std::string> params = Split(buf, " ");
    for (const auto& param : params) {
        std::string trimmed = Trim(param);
        if (trimmed.empty()) continue;

        size_t eq_pos = trimmed.find('=');
        if (eq_pos == std::string::npos) {
            // 无值参数，跳过
            continue;
        }

        std::string key = trimmed.substr(0, eq_pos);
        std::string value = trimmed.substr(eq_pos + 1);
        if (key == param_name) {
            return value;
        }
    }

    // 未找到参数，返回默认值
    LOG(WARNING) << "Param " << param_name << " not found in cmdline";
    return "0";
}

void vendor_load_properties() {
    // 替换：用自定义函数读取oplus_region，替代GetKernelCmdline
    std::string buf = ReadCmdlineParam("oplus_region");
    int region = 0;
    // 安全转换（避免std::stoi的异常风险）
    if (!android::base::ParseInt(buf, &region)) {
        LOG(ERROR) << "Invalid oplus_region value: " << buf << ", using default 0";
        region = 0;
    }

    // 查找区域后缀（保持原有逻辑）
    auto region_suffix_iter = kRegionSuffixMap.find(region);
    if (region_suffix_iter == kRegionSuffixMap.end()) {
        region_suffix_iter = kRegionSuffixMap.find(0); // 未匹配时用默认值
    }

    // 读取prjname（保持原有逻辑）
    std::string prjname_str = GetProperty("ro.boot.prjname", "0");
    int prjname = 0;
    if (!android::base::ParseInt(prjname_str, &prjname)) {
        LOG(ERROR) << "Invalid ro.boot.prjname value: " << prjname_str << ", using default 0";
        prjname = 0;
    }

    // 查找机型信息（保持原有逻辑）
    auto model_info_iter = kModelInfoMap.find(prjname);
    if (model_info_iter == kModelInfoMap.end()) {
        model_info_iter = kModelInfoMap.find(0); // 未匹配时用默认机型
    }

    SetupModelProperties(model_info_iter->second, region_suffix_iter->second);
}