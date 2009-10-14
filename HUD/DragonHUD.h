// TimerLayer
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TIMER_LAYER_H_
#define _TIMER_LAYER_H_

#include <Core/IListener.h>
#include <Display/IFrame.h>
#include <Display/HUD.h>
#include <Renderers/IRenderer.h>
#include <Renderers/TextureLoader.h>
#include <Resources/CairoResource.h>
#include <Utils/CairoTextTool.h>
#include <ScriptSystem.h>

using OpenEngine::Core::IListener;
using OpenEngine::Renderers::RenderingEventArg;
using OpenEngine::Renderers::TextureLoader;

using namespace OpenEngine::Utils;
using namespace OpenEngine::Display;

using namespace OpenEngine;
using namespace OpenEngine::Resources;

class DragonHUD : public IListener<ProcessEventArg> {
private:
    CairoTextTool* textTool;
    CairoResourcePtr timeTexture;
    CairoResourcePtr pointTexture;
    HUD::Surface* logoSurface, *timeSurface, *pointSurface;
    unsigned int frameWidth, frameHeight;    
    Display::HUD& hud;
    TextureLoader& texLoader;
    int timeleft, score;
public:
    DragonHUD(IFrame& frame, HUD& hud, TextureLoader& texLoader);
    virtual ~DragonHUD();

    void Handle(ProcessEventArg arg);
};


#endif // _TIMER_LAYER_H_
