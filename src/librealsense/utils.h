// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _UTILS_H
#define _UTILS_H

#include <librealsense/rs.hpp>
#include <string>

rs::stream GetRSStreamFromString(const std::string& stream);

std::string GetStringFromRSStream(const rs::stream stream);

rs::option GetRSOptionFromString(const std::string& option);

std::string GetStringFromRSFormat(const rs::format format);

rs::format GetRSFormatFromString(const std::string& format);

#endif  // _UTILS_H
