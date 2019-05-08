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
#ifndef __AGS_EE_MAIN__GRAPHICSMODE_H
#define __AGS_EE_MAIN__GRAPHICSMODE_H

int  graphics_mode_init();
void graphics_mode_shutdown();
int init_gfx_mode(int wid,int hit,int cdep, bool ignorefilter = false);

extern Size GameSize;
extern Size LetterboxedGameSize; // size of the game combined with letterbox borders
extern int final_scrn_wid, final_scrn_hit, final_col_dep, game_frame_x_offset, game_frame_y_offset;

#endif // __AGS_EE_MAIN__GRAPHICSMODE_H
