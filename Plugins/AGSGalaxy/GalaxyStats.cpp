// AGSGalaxy
// GOG Galaxy Plugin for AGS
// Copyright � 2015-2016 MonkeyMoto Productions, Inc.
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
#include "GalaxyStats.h"
using namespace AGSGalaxy;

GalaxyStats& GalaxyStats::GetGalaxyStats() noexcept
{
	static GalaxyStats GalaxyStats{};
	return GalaxyStats;
}

int GalaxyStats::GetIntStat(char const *name) const noexcept
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return 0;
		}
		return galaxy::api::Stats()->GetStatInt(name);
	}
	catch (...)
	{
		return 0;
	}
	return 0;
}

float GalaxyStats::GetFloatStat(char const *name) const noexcept
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return 0.0;
		}
		return galaxy::api::Stats()->GetStatFloat(name);
	}
	catch (...)
	{
		return 0.0;
	}
	return 0.0;
}

float GalaxyStats::GetAverageRateStat(char const *name) const noexcept
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return 0.0;
		}
		return galaxy::api::Stats()->GetStatFloat(name);
	}
	catch (...)
	{
		return 0.0;
	}
	return 0.0;
}

bool GalaxyStats::SetIntStat(char const *name, int value) const noexcept
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return false;
		}
		galaxy::api::Stats()->SetStatInt(name, value);
		galaxy::api::Stats()->StoreStatsAndAchievements();
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

bool GalaxyStats::SetFloatStat(char const *name, float value) const noexcept
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return false;
		}
		galaxy::api::Stats()->SetStatFloat(name, value);
		galaxy::api::Stats()->StoreStatsAndAchievements();
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

bool GalaxyStats::UpdateAverageRateStat(char const *name, float countThisSession, float sessionLength) const noexcept
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return false;
		}
		galaxy::api::Stats()->UpdateAvgRateStat(name, countThisSession, sessionLength);
		galaxy::api::Stats()->StoreStatsAndAchievements();
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}
