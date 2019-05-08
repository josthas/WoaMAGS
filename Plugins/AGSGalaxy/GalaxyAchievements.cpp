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
/*#include "GalaxyAchievements.h"
using namespace AGSGalaxy;

GalaxyAchievements& GalaxyAchievements::GetGalaxyAchievements()
{
	static GalaxyAchievements GalaxyAchievements{};
	return GalaxyAchievements;
}

bool GalaxyAchievements::ResetAchievement(char const *name) const
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return false;
		}
		galaxy::api::Stats()->ClearAchievement(name);
		galaxy::api::Stats()->StoreStatsAndAchievements();
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

bool GalaxyAchievements::IsAchievementAchieved(char const *name) const
{
	bool unlocked = false;
	uint32_t unlockTime = 0U;
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return false;
		}
		galaxy::api::Stats()->GetAchievement(name, unlocked, unlockTime);
		return unlocked;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

bool GalaxyAchievements::SetAchievementAchieved(char const *name) const
{
	try
	{
		if (galaxy::api::Stats() == nullptr)
		{
			return false;
		}
		galaxy::api::Stats()->SetAchievement(name);
		galaxy::api::Stats()->StoreStatsAndAchievements();
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}
*/