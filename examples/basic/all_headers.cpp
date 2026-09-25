/*
 *
 * Copyright (c) 2026 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2026-09-25
 */

// Includes every Lichens-CPP public header so that a consumer build with strict flags checks them all.
// On Windows, <windows.h> comes first to check that the headers do not collide with its macros.

#ifdef _WIN32
    #include <windows.h>
#endif

#include <LichensCPP/bimap.h>
#include <LichensCPP/config/AppConfig.h>
#include <LichensCPP/config/AppConfigHelper.h>
#include <LichensCPP/config/Config.h>
#include <LichensCPP/config/ConfigValue.h>
#include <LichensCPP/debouncing.h>
#include <LichensCPP/expected.h>
#include <LichensCPP/expected.hpp>
#include <LichensCPP/fileutil.h>
#ifndef _WIN32
    #include <LichensCPP/launcher/LaunchProcess.h>
#endif
#include <LichensCPP/launcher/MainLoopHelper.h>
#include <LichensCPP/launcher/PeriodicTask.h>
#include <LichensCPP/log/Logger.h>
#include <LichensCPP/log/LoggerHelper.h>
#include <LichensCPP/mqtt/MqttHelper.h>
#include <LichensCPP/ring_buffer.h>
#include <LichensCPP/scope_action.h>
#include <LichensCPP/stringutil.h>
#include <LichensCPP/timeutil.h>
#include <LichensCPP/unused.h>
#include <LichensCPP/uuid.h>

#include <string>

// Instantiates the header templates and macros so that their warnings show up. Never called.
void check_all_headers()
{
    LichensCPP::BiMap<std::string, int> bimap{{"one", 1}};
    UNUSED(bimap.contains_key("one"));
    UNUSED(bimap.contains_value(1));

    LichensCPP::Debouncing debouncing([](int) {});
    debouncing.call(1);

    LichensCPP::expected<int, std::string> value = LichensCPP::unexpected<std::string>("error");
    UNUSED(value.has_value());

    UNUSED_LIST();
    UNUSED_LIST(1, "two");

    LOG_INFO_F("format without arguments");
#if LICHENS_CPP_CPLUSPLUS >= 202002L
    LOG_INFO_F("format with arguments {} {}", 1, "two");
#else
    LOG_INFO_F("format with arguments %d %s", 1, "two");
#endif
    LOG_INFO_S("stream ", 1, " two");
}
