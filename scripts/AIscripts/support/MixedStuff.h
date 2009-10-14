
#ifndef MixedStuff_h
#define MixedStuff_h

#include <ScriptSystem.h>
#include <LuaHeader.h>
#include "../../../Modules/Island/HeightMap.h"
#include <Math/Vector.h>
#include <Logging/Logger.h>
#include <CPPToLuaBinder.h>
#include "ScriptFloatVector.h"
#include "../../../Modules/Particle/BoidFire.h"
#include <Scene/TransformationNode.h>
#include "../../../Modules/Boid/BoidsSystem.h"
#include "../../../Modules/Boid/Boid.h"
#include "../../../Modules/OscSurface/OscSurface.h"

int ActivateAISupportFuncs(lua_State* L);
int SetTransformationNode(lua_State* L);
int SetActiveBoidfire(lua_State* L);
int KillBoid(lua_State* L);
int MakeRipple(lua_State* L);

class ScriptHeightMap {

 private:
  HeightMap* map;

 public:

  ScriptHeightMap(lua_State* L);
  int HeightAt(lua_State* L);
  int NormalAt(lua_State* L);

  static string classname;
  static CPPToLuaBinder<ScriptHeightMap>::RegType Register[];

};



#endif
