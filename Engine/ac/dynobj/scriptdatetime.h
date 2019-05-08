//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
//
//
//=============================================================================
#ifndef __AGS_EE_DYNOBJ__SCRIPTDATETIME_H
#define __AGS_EE_DYNOBJ__SCRIPTDATETIME_H

#include "ac/dynobj/cc_agsdynamicobject.h"

struct ScriptDateTime : AGSCCDynamicObject {
    int year, month, day;
    int hour, minute, second;
    int rawUnixTime;

    virtual int Dispose(const char *address, bool force);
    virtual const char *GetType();
    virtual int Serialize(const char *address, char *buffer, int bufsize);
    virtual void Unserialize(int index, const char *serializedData, int dataSize);

    ScriptDateTime();
};

#endif // __AGS_EE_DYNOBJ__SCRIPTDATETIME_H
