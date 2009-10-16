
#include "MixedStuff.h"

string ScriptFloatVector::classname = "FloatVector";
CPPToLuaBinder<ScriptFloatVector>::RegType ScriptFloatVector::Register[] = {
  {"Add", &ScriptFloatVector::Add},
  {"AddTo", &ScriptFloatVector::AddTo},
  {"Subtract", &ScriptFloatVector::Subtract},
  {"SubtractFrom", &ScriptFloatVector::SubtractFrom},
  {"Mul", &ScriptFloatVector::Mul},
  {"MulTo", &ScriptFloatVector::MulTo},
  {"Devide", &ScriptFloatVector::Devide},
  {"DevideBy", &ScriptFloatVector::DevideBy},
  {"GetNormalize", &ScriptFloatVector::GetNormalize},
  {"GetVector", &ScriptFloatVector::GetVector},
  {"GetX", &ScriptFloatVector::GetX},
  {"GetY", &ScriptFloatVector::GetY},
  {"GetZ", &ScriptFloatVector::GetZ},
  {"SetX", &ScriptFloatVector::SetX},
  {"SetY", &ScriptFloatVector::SetY},
  {"SetZ", &ScriptFloatVector::SetZ},
  {"SetToZero", &ScriptFloatVector::SetToZero},
  {"Dot", &ScriptFloatVector::Dot},
  {"Cross", &ScriptFloatVector::Cross}
};

string ScriptHeightMap::classname = "HeightMap";
CPPToLuaBinder<ScriptHeightMap>::RegType ScriptHeightMap::Register[] = {
  {"HeightAt", &ScriptHeightMap::HeightAt},
  {"NormalAt", &ScriptHeightMap::NormalAt}
};

int ActivateAISupportFuncs(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "ActivateAISupportFuncs", 0))
    return 0;
  
  ScriptSystem::RegisterClass<ScriptFloatVector>();
  ScriptSystem::RegisterClass<ScriptHeightMap>();
  ScriptSystem::RegisterNonClassFunc("TruncateFloat", TruncateFloat);
  ScriptSystem::RegisterNonClassFunc("SetTransformationNode", SetTransformationNode);
  ScriptSystem::RegisterNonClassFunc("SetActiveBoidfire", SetActiveBoidfire);
  ScriptSystem::RegisterNonClassFunc("KillBoid", KillBoid);
  ScriptSystem::RegisterNonClassFunc("MakeRipple", MakeRipple);

  return 0;

}

ScriptHeightMap::ScriptHeightMap(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "ScriptHeightMap", 1) || 
      ScriptSystem::CheckArgType(L, "ScriptHeightMap", 'p', 1, -1))
    return;

  map = (HeightMap*) lua_touserdata(L, -1);

}

int ScriptHeightMap::HeightAt(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "HeightAt", 1))
    return 0;

  if (map == NULL) {
    logger.info << "scriptheightmap not properly initialised" << logger.end;
    return 0;
  }    

  Vector<3, float>* pos;

  if (lua_istable(L, -1))
    pos = GetVectorFromOther(L);
  else
    pos = (Vector<3, float>*) lua_touserdata(L, -1);

  Vector<3, float> ans = map->HeightAt(*pos);
  lua_pop(L, 1);
  MakeVector(L, ans);
  return 1;
}

int ScriptHeightMap::NormalAt(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "NormalAt", 1))
    return 0;

  if (map == NULL) {
    logger.info << "scriptheightmap not properly initialised" << logger.end;
    return 0;
  }    
  
   Vector<3, float>* pos;

  if (lua_istable(L, -1))
    pos = GetVectorFromOther(L);
  else
    pos = (Vector<3, float>*) lua_touserdata(L, -1);

  Vector<3, float> ans = map->HeightAt(*pos);
  lua_pop(L, 1);
  MakeVector(L, ans);
  return 1;

}

int SetTransformationNode(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SetTransformationNode", 3) ||
      ScriptSystem::CheckArgType(L, "SetTransformationNode", 'p', 1, -2) ||
      ScriptSystem::CheckArgType(L, "SetTransformationNode", 'p', 2, -1))
    return 0;

  BoidFire* bf = (BoidFire*) lua_touserdata(L, -2);
  Vector<3, float>* pos = (Vector<3, float>*) lua_touserdata(L, -1);

  TransformationNode* tn = bf->GetTransformationNode();
  if (tn) 
    tn->SetPosition(*pos);
 
  return 0;

}

int SetActiveBoidfire(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SetActiveBoidfire", 3) ||
      ScriptSystem::CheckArgType(L, "SetActiveBoidfire", 'p', 1, -2) ||
      ScriptSystem::CheckArgType(L, "SetActiveBoidfire", 'b', 2, -1))
    return 0;

  BoidFire* bf = (BoidFire*) lua_touserdata(L, -2);
  bool val = lua_toboolean(L, -1);
  bf->SetActive(val);
  return 0;

}

int KillBoid(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "KillBoid", 3) ||
      ScriptSystem::CheckArgType(L, "KillBoid", 'p', 1, -2) ||
      ScriptSystem::CheckArgType(L, "KillBoid", 'p', 2, -1))
    return 0;
  
  Boid* b = (Boid*) lua_touserdata(L, -2);
  BoidsSystem* bs = (BoidsSystem*) lua_touserdata(L, -1);

  bs->BoidDied(*b);

  return 0;

}

int MakeRipple(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "MakeRipple", 6) ||
      ScriptSystem::CheckArgType(L, "MakeRipple", 'p', 1, -5) ||
      ScriptSystem::CheckArgType(L, "MakeRipple", 'd', 2, -4) ||
      ScriptSystem::CheckArgType(L, "MakeRipple", 'd', 3, -3) ||
      ScriptSystem::CheckArgType(L, "MakeRipple", 'd', 4, -2) ||
      ScriptSystem::CheckArgType(L, "MakeRipple", 'd', 5, -1))
    return 0;
 
  OscSurface* os = (OscSurface*) lua_touserdata(L, -5);
  float x = lua_tonumber(L, -4);
  float z = lua_tonumber(L, -3);
  float width = lua_tonumber(L, -2);
  float heigth = lua_tonumber(L, -1);
 
  os->createRipple(x, z, width, heigth);

  return 0;

}
