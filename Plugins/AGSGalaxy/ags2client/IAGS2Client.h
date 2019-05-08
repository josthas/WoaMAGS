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
#ifndef AGS2CLIENT_IAGS2CLIENT_H
#define AGS2CLIENT_IAGS2CLIENT_H

class IAGSEngine;

namespace AGS2Client
{
    class IAGS2Client
    {
    public:
        virtual bool IsInitialized() const = 0;
        virtual void ResetStatsAndAchievements() const = 0;
        virtual char const* GetUserName() const = 0;
		virtual void Startup() const = 0;
        virtual void Shutdown() const = 0;
        virtual void Update() const = 0;
        virtual char const* GetAGSScriptHeader() const;
        virtual char const* GetAGSPluginName() const = 0;
        virtual char const* GetAGSPluginDesc() const = 0;
        virtual char const* GetClientNameForScript() const;
		virtual char const* GetExtraFunctionsForScriptHeader() const;
		virtual float GetVersion() const = 0;
        virtual bool ClaimKeyPress(int data, int(*IsKeyPressed)(int)) const = 0;
        virtual void RegisterScriptFunctions(IAGSEngine *engine) const;
        virtual ~IAGS2Client() = 0;
    };

    IAGS2Client* GetClient();
    int AGS2Client_IsInitialized();
    void AGS2Client_ResetStatsAndAchievements();
    char const* AGS2Client_GetUserName();
} // namespace AGS2Client

#endif // AGS2CLIENT_IAGS2CLIENT_H
