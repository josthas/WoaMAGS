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
#ifndef AGSGALAXY_H
#define AGSGALAXY_H

#include "ags2client/IAGS2Client.h"

namespace AGSGalaxy
{
	class AGSGalaxy : public AGS2Client::IAGS2Client
	{
	protected:
		AGSGalaxy();

	public:
		static AGSGalaxy& GetAGSGalaxy();
		~AGSGalaxy();
		bool IsInitialized() const override;
		void ResetStatsAndAchievements() const override;
		char const* GetExtraFunctionsForScriptHeader() const override;
		char const* GetUserName() const override;
		void Startup() const override;
		void Shutdown() const override;
		void Update() const override;
		char const* GetAGSPluginName() const override;
		char const* GetAGSPluginDesc() const override;
		float GetVersion() const override;
		bool ClaimKeyPress(int, int(*)(int)) const override;
		void RegisterScriptFunctions(IAGSEngine *engine) const override;
	};
} // namespace AGSGalaxy

#endif // AGSGALAXY_H
