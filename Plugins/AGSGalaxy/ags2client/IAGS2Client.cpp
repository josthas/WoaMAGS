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
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // WIN32_LEAN_AND_MEAN
#elif defined(__linux__) // !_WIN32
#define __stdcall
#endif // _WIN32, __linux__

#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>
#include "agsplugin.h"
#include "IAGS2Client.h"
#include "IClientAchievements.h"
#include "IClientLeaderboards.h"
#include "IClientStats.h"
using namespace AGS2Client;

IAGSEngine *GetAGSEngine();

char const* IAGS2Client_GetPluginNameStripped() noexcept
{
	static std::string pluginName;
	if (pluginName.empty())
	{
		std::string buffer{ GetClient()->GetAGSPluginName() };
		buffer.erase(std::remove_if(buffer.begin(), buffer.end(), [](char c)
		{
			return (((c < 48) || (c > 57)) && ((c < 65) || (c > 90)) && (c != 95) && ((c < 97) || (c > 122)));
		}), buffer.end());
		buffer.erase(0, buffer.find_first_not_of("0123456789"));
		pluginName = (buffer.empty() ? "AGS2Client" : buffer);
	}
	return pluginName.c_str();
}

char const* IAGS2Client::GetClientNameForScript() const noexcept
{
	static std::string clientName;
	if (clientName.empty())
	{
#ifdef AGS2CLIENT_UNIFIED_CLIENT_NAME
		clientName = "AGS2Client";
#else // !AGS2CLIENT_UNIFIED_CLIENT_NAME
		clientName = IAGS2Client_GetPluginNameStripped();
#endif // AGS2CLIENT_UNIFIED_CLIENT_NAME
	}
	return clientName.c_str();
}

char const* IAGS2Client::GetAGSScriptHeader() const noexcept
{
	static std::string generatedHeader;
	if (generatedHeader.empty())
	{
		float version = this->GetVersion();
		std::string clientName{ this->GetClientNameForScript() };
		std::ostringstream header;
#ifdef AGS2CLIENT_UNIFIED_CLIENT_NAME
		std::string pluginName{ IAGS2Client_GetPluginNameStripped() };
		if (pluginName != clientName) // account for the unlikely event that pluginName has no usable characters and defaults to AGS2Client
		{
			// include plugin's ACTUAL name version macro (if it is not already included by DISJOINT plugin!)
			header << "#ifndef " << pluginName << "_VERSION\r\n"
				"#define " << pluginName << "_VERSION " << version << "\r\n"
				"#endif // " << pluginName << "_VERSION\r\n";
		}
#endif // AGS2CLIENT_UNIFIED_CLIENT_NAME
		header <<
			"#define " << clientName << "_VERSION " << version << "\r\n"
			"\r\n"
			"enum " << clientName << "StatType\r\n"
			"{\r\n"
			"  e" << clientName << "StatInt = 0,\r\n"
			"  e" << clientName << "StatFloat = 1,\r\n"
			"  e" << clientName << "StatAverageRate = 2\r\n"
			"};\r\n"
			"\r\n"
			"enum " << clientName << "ScoresRequestType\r\n"
			"{\r\n"
			"  e" << clientName << "ScoresRequestGlobal = 0,\r\n"
			"  e" << clientName << "ScoresRequestAroundUser = 1,\r\n"
			"  e" << clientName << "ScoresRequestFriends = 2\r\n"
			"};\r\n"
			"\r\n"
			"managed struct " << clientName << "\r\n"
			"{\r\n"
			"  ///" << clientName << ": Returns whether the specified achievement has been achieved\r\n"
			"  import static int IsAchievementAchieved(const string name);\r\n"
			"  ///" << clientName << ": Sets an achievement as achieved\r\n"
			"  import static int SetAchievementAchieved(const string name);\r\n"
			"  ///" << clientName << ": Resets the specified achievement, so it can be achieved again\r\n"
			"  import static int ResetAchievement(const string name);\r\n"
			"  ///" << clientName << ": Sets the value of an INT stat\r\n"
			"  import static int SetIntStat(const string name, int value);\r\n"
			"  ///" << clientName << ": Sets the value of a FLOAT stat\r\n"
			"  import static int SetFloatStat(const string name, float value);\r\n"
			"  ///" << clientName << ": Updates an average rate stat\r\n"
			"  import static int UpdateAverageRateStat(const string name, float countThisSession, float sessionLength);\r\n"
			"  ///" << clientName << ": Returns the value of an INT stat\r\n"
			"  import static int GetIntStat(const string name);\r\n"
			"  ///" << clientName << ": Returns the value of a FLOAT stat\r\n"
			"  import static float GetFloatStat(const string name);\r\n"
			"  ///" << clientName << ": Returns the value of an average rate stat\r\n"
			"  import static float GetAverageRateStat(const string name);\r\n"
			"  ///" << clientName << ": Resets all stats and achievements\r\n"
			"  import static void ResetStatsAndAchievements();\r\n"
			"  ///" << clientName << ": Returns whether the client is loaded and the user logged in\r\n"
			"  readonly import static attribute int Initialized;\r\n"
			"  ///" << clientName << ": Returns the name of the current leaderboard (call FindLeaderboard first)\r\n"
			"  readonly import static attribute String CurrentLeaderboardName;\r\n"
			"  ///" << clientName << ": Requests to load the specified leaderboard. This call is asynchronous and does not return the data immediately, check for results in repeatedly_execute\r\n"
			"  import static void RequestLeaderboard(const string leaderboardName, " << clientName << "ScoresRequestType, int limit=10);\r\n"
			"  ///" << clientName << ": Uploads the score to the current leaderboard (call RequestLeaderboard first). Returns false if an error occurs\r\n"
			"  import static int UploadScore(int score);\r\n"
			"  ///" << clientName << ": Returns the name associated with a leaderboard rank (call RequestLeaderboard first)\r\n"
			"  readonly import static attribute String LeaderboardNames[];\r\n"
			"  ///" << clientName << ": Returns a leader's score from the current leaderboard (call RequestLeaderboard first)\r\n"
			"  readonly import static attribute int LeaderboardScores[];\r\n"
			"  ///" << clientName << ": Returns the number of leaders in the current leaderboard (if any) (call RequestLeaderboard first)\r\n"
			"  readonly import static attribute int LeaderboardCount;\r\n"
			"  ///" << clientName << ": Returns the username from the client\r\n"
			"  import static String GetUserName();\r\n"
			<< this->GetExtraFunctionsForScriptHeader() <<
			"};\r\n"
			"\r\n";
		generatedHeader = header.str();
	}
	return generatedHeader.c_str();
}

char const* IAGS2Client::GetExtraFunctionsForScriptHeader() const noexcept
{
	return "";
}

void IAGS2Client::RegisterScriptFunctions(IAGSEngine *engine) const noexcept
{
	static std::unordered_map<std::string, void*> functions; // RegisterScriptFunction does not copy the buffer, we must persist it ourselves
	std::string clientName{ this->GetClientNameForScript() };
	functions[clientName + "::IsAchievementAchieved^1"] = reinterpret_cast<void*>(ClientAchievements_IsAchievementAchieved);
	functions[clientName + "::SetAchievementAchieved^1"] = reinterpret_cast<void*>(ClientAchievements_SetAchievementAchieved);
	functions[clientName + "::ResetAchievement^1"] = reinterpret_cast<void*>(ClientAchievements_ResetAchievement);
	functions[clientName + "::GetIntStat^1"] = reinterpret_cast<void*>(ClientStats_GetIntStat);
	functions[clientName + "::GetFloatStat^1"] = reinterpret_cast<void*>(ClientStats_GetFloatStat);
	functions[clientName + "::GetAverageRateStat^1"] = reinterpret_cast<void*>(ClientStats_GetAverageRateStat);
	functions[clientName + "::SetIntStat^2"] = reinterpret_cast<void*>(ClientStats_SetIntStat);
	functions[clientName + "::SetFloatStat^2"] = reinterpret_cast<void*>(ClientStats_SetFloatStat);
	functions[clientName + "::UpdateAverageRateStat^3"] = reinterpret_cast<void*>(ClientStats_UpdateAverageRateStat);
	functions[clientName + "::ResetStatsAndAchievements^0"] = reinterpret_cast<void*>(AGS2Client_ResetStatsAndAchievements);
	functions[clientName + "::get_Initialized"] = reinterpret_cast<void*>(AGS2Client_IsInitialized);
	functions[clientName + "::get_CurrentLeaderboardName"] = reinterpret_cast<void*>(ClientLeaderboards_GetCurrentLeaderboardName);
	functions[clientName + "::RequestLeaderboard^3"] = reinterpret_cast<void*>(ClientLeaderboards_RequestLeaderboard);
	functions[clientName + "::UploadScore^1"] = reinterpret_cast<void*>(ClientLeaderboards_UploadScore);
	functions[clientName + "::geti_LeaderboardNames"] = reinterpret_cast<void*>(ClientLeaderboards_GetLeaderName);
	functions[clientName + "::geti_LeaderboardScores"] = reinterpret_cast<void*>(ClientLeaderboards_GetLeaderScore);
	functions[clientName + "::get_LeaderboardCount"] = reinterpret_cast<void*>(ClientLeaderboards_GetLeaderCount);
	functions[clientName + "::GetUserName^0"] = reinterpret_cast<void*>(AGS2Client_GetUserName);
	for (auto &func : functions)
	{
		engine->RegisterScriptFunction(func.first.c_str(), func.second);
	}
}

IAGS2Client::~IAGS2Client() noexcept
{
}

namespace AGS2Client
{
	int AGS2Client_IsInitialized() noexcept
	{
		return (GetClient()->IsInitialized() != 0);
	}

	void AGS2Client_ResetStatsAndAchievements() noexcept
	{
		GetClient()->ResetStatsAndAchievements();
	}

	char const* AGS2Client_GetUserName() noexcept
	{
		char const *userName = GetClient()->GetUserName();
		return (userName == nullptr ? nullptr : GetAGSEngine()->CreateScriptString(userName));
	}
}
