// AGS2Client
// Client Plugin Interface for AGS
// Copyright © 2015-2016 MonkeyMoto Productions, Inc.
//
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the LICENSE file for more details.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
//
#pragma once
#ifndef AGS2CLIENT_AGSFLOATFIX_H
#define AGS2CLIENT_AGSFLOATFIX_H

#include "stdint.h"

namespace AGS
{
	using float_t = std::int32_t;
} // namespace AGS

#define FLOAT2AGS(x) *reinterpret_cast<AGS::float_t*>(&(x))
#define AGS2FLOAT(x) *reinterpret_cast<float*>(&(x))

#endif // AGS2CLIENT_AGSFLOATFIX_H
