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
#include <algorithm>
#include <stddef.h>
#include "stdint.h"
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "ags2client/agsplugin.h"
#include "GalaxyLeaderboards.h"
using namespace AGSGalaxy;

GalaxyLeaderboards& GalaxyLeaderboards::GetGalaxyLeaderboards() noexcept
{
	static GalaxyLeaderboards GalaxyLeaderboards{};
	return GalaxyLeaderboards;
}

class FriendListListener : public galaxy::api::GlobalFriendListListener
{
private:
	FriendListListener() = default;
	char const *leaderboardName;
	int leaderboardLimit;

public:
	static FriendListListener& GetFriendListListener(char const *leaderboardName, int limit) noexcept
	{
		static FriendListListener listener{};
		listener.leaderboardName = leaderboardName;
		listener.leaderboardLimit = limit;
		return listener;
	}

	void OnFriendListRetrieveSuccess() noexcept override
	{
		try
		{
			if ((galaxy::api::Friends() == nullptr) || (galaxy::api::Stats() == nullptr) || (galaxy::api::Friends()->GetFriendCount() == 0U))
			{
				return;
			}
			std::vector<galaxy::api::GalaxyID> friendIDs{};
			for (auto count = galaxy::api::Friends()->GetFriendCount(), i = 0U; i < count; ++i)
			{
				friendIDs.push_back(galaxy::api::Friends()->GetFriendByIndex(i));
			}
			galaxy::api::Stats()->RequestLeaderboardEntriesForUsers(this->leaderboardName, friendIDs.data(), std::min(static_cast<std::size_t>(this->leaderboardLimit), friendIDs.size()));
		}
		catch (...)
		{
		}
	}

	void OnFriendListRetrieveFailure(FailureReason failureReason) noexcept override
	{
		// TODO: log error
	}
};

static struct {
	std::string Name;
	std::map<uint32_t, std::pair<galaxy::api::GalaxyID, int32_t>> Entries;
} leaderboard;

class LeaderboardEntriesRetrieveListener : public galaxy::api::GlobalLeaderboardEntriesRetrieveListener
{
private:
	LeaderboardEntriesRetrieveListener() = default;

public:
	static LeaderboardEntriesRetrieveListener& GetLeaderboardEntriesRetrieveListener() noexcept
	{
		static LeaderboardEntriesRetrieveListener listener{};
		return listener;
	}

	void OnLeaderboardEntriesRetrieveSuccess(char const *name, uint32_t entryCount) noexcept override
	{
		leaderboard.Entries.clear();
		if (galaxy::api::Stats() == nullptr)
		{
			leaderboard.Name = "";
			return;
		}
		struct
		{
			uint32_t Rank;
			int32_t Score;
			galaxy::api::GalaxyID UserID;
		} entry;
		try
		{
			for (auto i = 0U; i < entryCount; ++i)
			{
				galaxy::api::Stats()->GetRequestedLeaderboardEntry(i, entry.Rank, entry.Score, entry.UserID);
				leaderboard.Entries[entry.Rank] = std::make_pair(entry.UserID, entry.Score);
			}
		}
		catch (...)
		{
		}
	}

	void OnLeaderboardEntriesRetrieveFailure(char const *name, FailureReason failureReason) noexcept override
	{
		leaderboard.Entries.clear();
		leaderboard.Name = "";
	}
};

void GalaxyLeaderboards::RequestLeaderboard(char const *leaderboardName, AGS2Client::LeaderboardScoreType type, int limit) const noexcept
{
	leaderboard.Name = (leaderboardName == nullptr ? "" : leaderboardName);
	leaderboard.Entries.clear();
	if (leaderboardName == nullptr)
	{
		return;
	}
	if (type == AGS2Client::LeaderboardScoreType::Friends)
	{
		if (galaxy::api::Friends() == nullptr)
		{
			return;
		}
		try
		{
			auto &listener = FriendListListener::GetFriendListListener(leaderboardName, limit);
			galaxy::api::Friends()->RequestFriendList();
		}
		catch (...)
		{
			return;
		}
	}
	else if (galaxy::api::Stats() == nullptr)
	{
		return;
	}
	else if (type == AGS2Client::LeaderboardScoreType::AroundUser)
	{
		try
		{
			galaxy::api::Stats()->RequestLeaderboardEntriesAroundUser(leaderboardName, limit / 2, limit / 2);
		}
		catch (...)
		{
		}
	}
	else if (type == AGS2Client::LeaderboardScoreType::Global)
	{
		try
		{
			galaxy::api::Stats()->RequestLeaderboardEntriesGlobal(leaderboardName, 0, limit);
		}
		catch (...)
		{
		}
	}
}

bool GalaxyLeaderboards::UploadScore(int score) const noexcept
{
	if ((galaxy::api::Stats() == nullptr) || (leaderboard.Name == ""))
	{
		return false;
	}
	try
	{
		galaxy::api::Stats()->SetLeaderboardScore(leaderboard.Name.c_str(), score);
	}
	catch (...)
	{
	}
	return true;
}

char const* GalaxyLeaderboards::GetCurrentLeaderboardName() const noexcept
{
	return (leaderboard.Name == "" ? nullptr : leaderboard.Name.c_str());
}

char const* GalaxyLeaderboards::GetLeaderName(int index) const noexcept
{
	if ((index < 0) || (index >= leaderboard.Entries.size()) || (galaxy::api::Friends() == nullptr))
	{
		return nullptr;
	}
	char const *name = galaxy::api::Friends()->GetFriendPersonaName(leaderboard.Entries[index].first);
	return (name == nullptr ? nullptr : name);
}

int GalaxyLeaderboards::GetLeaderScore(int index) const noexcept
{
	if ((index < 0) || (index >= leaderboard.Entries.size()))
	{
		return 0;
	}
	return leaderboard.Entries[index].second;
}

int GalaxyLeaderboards::GetLeaderCount() const noexcept
{
	return leaderboard.Entries.size();
}
