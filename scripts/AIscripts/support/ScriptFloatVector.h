
#ifndef ScriptFloatVector_h
#define ScriptFloatVector_h

#include <Math/Vector.h>
#include <ScriptSystem.h>
#include <LuaHeader.h>
#include "../../../Common/VectorExt.h"
#include <CPPToLuaBinder.h>

int TruncateFloat(lua_State* L);
void MakeVector(lua_State* L, Vector<3, float> vec);
Vector<3, float>* GetVectorFromOther(lua_State* L);

class ScriptFloatVector {

 private:
  Vector<3, float>* vec;

 public:

  ScriptFloatVector(lua_State* L);
  int Add(lua_State* L);
  int AddTo(lua_State* L);
  int Subtract(lua_State* L);
  int SubtractFrom(lua_State* L);
  int Mul(lua_State* L);
  int MulTo(lua_State* L);
  int Devide(lua_State* L);
  int DevideBy(lua_State* L);
  int GetNormalize(lua_State* L);
  int GetVector(lua_State* L);
  Vector<3, float>* GetVector();
  int GetX(lua_State* L);
  int GetY(lua_State* L);
  int GetZ(lua_State* L);
  int SetX(lua_State* L);
  int SetY(lua_State* L);
  int SetZ(lua_State* L);
  int SetToZero(lua_State* L);
  int Dot(lua_State* L);
  int GetLength(lua_State* L);
  int Cross(lua_State* L);

  static string classname;
  static CPPToLuaBinder<ScriptFloatVector>::RegType Register[];

};

#endif
