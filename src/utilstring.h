// Copyright (c) 2019 The Fleet Credits Core developers
// Copyright (c) 2022 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_UTIL_STRING_H
#define FLEETCREDITS_UTIL_STRING_H

#include "attributes.h"

#include <cstring>
#include <string>

/**
 * Check if a string does not contain any embedded NUL (\0) characters
 */
NODISCARD inline bool ValidAsCString(const std::string& str) noexcept
{
    return str.size() == strlen(str.c_str());
}

#endif // FLEETCREDITS_UTIL_STRING_H
