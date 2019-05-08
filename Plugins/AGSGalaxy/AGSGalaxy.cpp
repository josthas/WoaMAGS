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
#include <sstream>
#include <string>
#include "AGSGalaxy.h"
#ifdef USE_GOG
#include <GalaxyApi.h>
#endif
#include "ags2client/agsplugin.h"

/*
static bool GalaxyInitialized = false;

// GOG API helper for user sign-in callbacks
class AuthListener : public galaxy::api::GlobalAuthListener
{
public:
	void OnAuthSuccess() override
	{
		try
		{
			if (galaxy::api::Stats() == NULL) // this is less than ideal, but will occur if the client is not running or API is not yet initialized
			{
				return;
			}
			// on sign-in, immediately request stats+achievements
			galaxy::api::Stats()->RequestUserStatsAndAchievements();
		}
		catch (...)
		{
		}
	}

	void OnAuthFailure(FailureReason reason) override
	{
		// TODO: log failure reason?
	}

	void OnAuthLost() override
	{
	}
};

AuthListener* pListener;

// GOG API helper for stats+achievements retrieved callbacks
class UserStatsAndAchievementsRetrieveListener : public galaxy::api::GlobalUserStatsAndAchievementsRetrieveListener
{
	void OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) override
	{
		// once we have received the user stats+achievements, we are fully initialized
		GalaxyInitialized = true;
	}

	void OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason reason) override
	{
		// TODO: log failure
	}
};

// Initialize GOG API, sign in the user, and request user stats+achievements (called from user scripts with clientID and clientSecret)
int AGSGalaxy_Initialize(char const *clientID, char const *clientSecret)
{
#ifdef USE_GOG
	if (!GalaxyInitialized)
	{
		try
		{
			galaxy::api::Init(clientID, clientSecret);
		}
		catch (...)
		{
			return false;
		}
		// these CANNOT appear before the API is initialized! d'oh!!!!
		AuthListener authListener;
		UserStatsAndAchievementsRetrieveListener statsListener;
		try
		{
			if (galaxy::api::User() == NULL)
			{
				return false;
			}
			// API is initialized, sign user in (async, see callback classes above)
			galaxy::api::User()->SignIn();
		}
		catch (...)
		{
			return false;
		}
	}
	return GalaxyInitialized;
#else
	return false;
#endif
}*/
/*
AGSGalaxy::AGSGalaxy& AGSGalaxy::AGSGalaxy::GetAGSGalaxy()
{
	static AGSGalaxy galaxy{};
	return galaxy;
}*/
/*
void AGSGalaxy::AGSGalaxy::Startup() const
{
}

bool AGSGalaxy::AGSGalaxy::IsInitialized() const
{
#ifdef USE_GOG
	return GalaxyInitialized;
#else
	return false;
#endif
}

void AGSGalaxy::AGSGalaxy::ResetStatsAndAchievements() const
{
#ifdef USE_GOG
	try
	{
		if (galaxy::api::Stats() == NULL)
		{
			return;
		}
		galaxy::api::Stats()->ResetStatsAndAchievements();
		// per API docs, this stores changes automatically
	}
	catch (...)
	{
	}
#endif
}

char const* AGSGalaxy::AGSGalaxy::GetExtraFunctionsForScriptHeader() const
{
	return
		"  ///AGSGalaxy: Attempts to initialize the Galaxy client with the specified ID and secret. Must be called before Galaxy features can be used. Initialization is asynchronous and may not occur immediately.\r\n"
		"  import static void Initialize(const string galaxyClientID, const string galaxyClientSecret);\r\n";
}

char const* AGSGalaxy::AGSGalaxy::GetUserName() const
{/*
#ifdef USE_GOG
	try
	{
		if (galaxy::api::Friends() == NULL)
		{
			return NULL;
		}
		char const *name = galaxy::api::Friends()->GetPersonaName();
		return (name == NULL ? NULL : name);
	}
	catch (...)
	{
		return NULL;
	}
	return NULL;
#else*/
	/*return "UNKNOWN";
//#endif
}

void AGSGalaxy::AGSGalaxy::Shutdown() const
{
#ifdef USE_GOG
	try
	{
		galaxy::api::Shutdown();
	}
	catch (...)
	{
	}
#endif
}

void AGSGalaxy::AGSGalaxy::Update() const
{
#ifdef USE_GOG
	try
	{
		galaxy::api::ProcessData();
	}
	catch (...)
	{
	}
#endif
}

char const* AGSGalaxy::AGSGalaxy::GetAGSPluginName() const
{
	return "AGSGalaxy";
}

char const* AGSGalaxy::AGSGalaxy::GetAGSPluginDesc() const
{
	return "AGSGalaxy: GOG Galaxy Plugin for AGS (C) 2015-2016 MonkeyMoto Productions, Inc.";
}

float AGSGalaxy::AGSGalaxy::GetVersion() const
{
	return 1.0;
}

bool AGSGalaxy::AGSGalaxy::ClaimKeyPress(int, int(*)(int)) const
{
	// TODO: GOG supported feature? (Shift+Tab code below)
	//bool isShift = ((data == 403) || (data == 404)); // is pressed key shift
	//bool isTab = (data == 9); // is pressed key tab
	//bool isShiftTab = ((data == 0x00001111) || // shift+tab as a single key
	//	((isShift) && (IsKeyPressed(9) != 0)) || // key is shift and tab is held
	//	((isTab) && ((IsKeyPressed(403) != 0) || (IsKeyPressed(404) != 0)))); // key is tab and shift is held
	//return isShiftTab; // Claim (Shift+Tab), ignore other keys
	return false;
}

void AGSGalaxy::AGSGalaxy::RegisterScriptFunctions(IAGSEngine *engine) const
{
	IAGS2Client::RegisterScriptFunctions(engine);
	engine->RegisterScriptFunction("AGS2Client::Initialize^2", reinterpret_cast<void*>(AGSGalaxy_Initialize));
}
*/