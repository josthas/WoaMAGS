// AGSGalaxy
// GOG Galaxy Plugin for AGS
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
#ifndef AGSGALAXY_STATS_H
#define AGSGALAXY_STATS_H

#include "ags2client/IClientStats.h"
#include "galaxy/GalaxyApi.h"

namespace AGSGalaxy
{
	class GalaxyStats : public AGS2Client::IClientStats
	{
	protected:
		GalaxyStats() noexcept = default;

	public:
		static GalaxyStats& GetGalaxyStats() noexcept;
		~GalaxyStats() noexcept = default;
		int GetIntStat(char const *name) const noexcept override;
		float GetFloatStat(char const *name) const noexcept override;
		float GetAverageRateStat(char const *name) const noexcept override;
		bool SetIntStat(char const *name, int value) const noexcept override;
		bool SetFloatStat(char const *name, float value) const noexcept override;
		bool UpdateAverageRateStat(char const *name, float countThisSession, float sessionLength) const noexcept override;
	}; // class GalaxyStats
} // namespace AGSGalaxy

#endif // AGSGALAXY_STATS_H
