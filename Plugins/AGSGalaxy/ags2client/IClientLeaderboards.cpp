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
#include "agsplugin.h"
#include "IClientLeaderboards.h"
using namespace AGS2Client;

IAGSEngine* GetAGSEngine();

IClientLeaderboards::~IClientLeaderboards() noexcept
{
}

namespace AGS2Client
{
	char const* ClientLeaderboards_GetCurrentLeaderboardName() noexcept
	{
		if (GetClientLeaderboards() == nullptr) return nullptr;
		char const *leaderboardName = GetClientLeaderboards()->GetCurrentLeaderboardName();
		return (leaderboardName == nullptr ? nullptr : GetAGSEngine()->CreateScriptString(leaderboardName));
	}

	void ClientLeaderboards_RequestLeaderboard(char const *leaderboardName, int type, int limit) noexcept
	{
		if (GetClientLeaderboards() != nullptr) GetClientLeaderboards()->RequestLeaderboard(leaderboardName, static_cast<LeaderboardScoreType>(type), limit);
	}

	int ClientLeaderboards_UploadScore(int score) noexcept
	{
		return (GetClientLeaderboards() == nullptr ? 0 : GetClientLeaderboards()->UploadScore(score));
	}

	char const* ClientLeaderboards_GetLeaderName(int index) noexcept
	{
		if (GetClientLeaderboards() == nullptr) return nullptr;
		char const *leaderName = GetClientLeaderboards()->GetLeaderName(index);
		return (leaderName == nullptr ? nullptr : GetAGSEngine()->CreateScriptString(leaderName));
	}

	int ClientLeaderboards_GetLeaderScore(int index) noexcept
	{
		return (GetClientLeaderboards() == nullptr ? 0 : GetClientLeaderboards()->GetLeaderScore(index));
	}

	int ClientLeaderboards_GetLeaderCount() noexcept
	{
		return (GetClientLeaderboards() == nullptr ? 0 : GetClientLeaderboards()->GetLeaderCount());
	}
}
