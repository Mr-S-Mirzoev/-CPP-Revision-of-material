#pragma once
#ifndef INCLUDED_NOTIFICATIONS_02072020
#define INCLUDED_NOTIFICATIONS_02072020

#include <string>
#include <iostream>
#if defined(__APPLE__) || defined(__MACH__)
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <unistd.h>
#include <algorithm>

struct Notification {
    Notification (const std::string &name, const std::string &message, int timeout = 10000);
};
#elif defined(__linux__) || defined(__linux) || defined(linux)
#include <libnotify/notify.h>
struct Notification {
    Notification (const std::string &name, const std::string &message, int timeout = 10000);
};
#endif

#endif