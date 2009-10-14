
#include "ScriptFloatVector.h"

ScriptFloatVector::ScriptFloatVector(lua_State* L) {

  int nargs = lua_gettop(L);

  //choose appropiate constructor
  if (nargs == 3) {

    if (ScriptSystem::CheckArgType(L, "ScriptFloatVector", "float", 1, -3) ||
	ScriptSystem::CheckArgType(L, "ScriptFloatVector", "float", 2, -2) ||
	ScriptSystem::CheckArgType(L, "ScriptFloatVector", "float", 3, -1))
      return;

    float x = lua_tonumber(L, -3);
    float y = lua_tonumber(L, -2);
    float z = lua_tonumber(L, -1);

    vec = new Vector<3, float>(x, y, z);

  }
  else if (nargs == 1) {

    Vector<3, float>* initvec;

    if (lua_istable(L, -1))
      initvec = GetVectorFromOther(L);
    else if (lua_isuserdata(L, -1))
      initvec = (Vector<3, float>*) lua_touserdata(L, -1);
    else {
      logger.info << "argument given to constructor could not be processed. Only "
		<< "Vector<3, float> or FloatVector are supported"
		<< logger.end;
      return;
    }

    vec = new Vector<3, float>(*initvec);

  }
  else {

    if (nargs != 0) 
      logger.info << "Signature of ScriptFloatVector is: "
		  << "ScriptFloatVector(float x, float y, float z) or "
		  << "ScriptFloatVector(). However " << nargs 
		  << " arguments were received. "
		  << "ScriptFloatVector() constructor used." << logger.end;

    vec = new Vector<3, float>();

  }

}

int ScriptFloatVector::Add(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "Add", 2))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  Vector<3, float> res;

  //we are dealing with vector addition (our version or original
  if (lua_isuserdata(L, -1) || lua_istable(L, -1)) {
    
    Vector<3, float>* other; 

    if (lua_istable(L, -1)) 
      other = GetVectorFromOther(L);        
    else 
      other = (Vector<3, float>*) lua_touserdata(L, -1);          

    res = (*vec) + (*other);
  
  }
  //we are dealing with number addition
  else if (lua_isnumber(L, -1)) {

    float num = lua_tonumber(L, -1);

    res = (*vec) + num;
    
  }
  else {
    logger.info << "argument given to Add could not be processed. Only "
		<< "Vector<3, float>, FloatVector or constant are supported"
		<< logger.end;
    return 0;
  }

  lua_pop(L, 1);
  MakeVector(L, res);
  return 1;

}

int ScriptFloatVector::AddTo(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "AddTo", 2))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  Vector<3, float> res;

  //we are dealing with vector addition (our version or original
  if (lua_isuserdata(L, -1) || lua_istable(L, -1)) {
    
    Vector<3, float>* other; 

    if (lua_istable(L, -1)) 
      other = GetVectorFromOther(L);        
    else 
      other = (Vector<3, float>*) lua_touserdata(L, -1);          

    res = (*vec) + (*other);
  
  }
  //we are dealing with number addition
  else if (lua_isnumber(L, -1)) {

    float num = lua_tonumber(L, -1);

    res = (*vec) + num;
    
  }
  else {
    logger.info << "argument given to AddTo could not be processed. Only "
		<< "Vector<3, float>, FloatVector or constant are supported"
		<< logger.end;
    return 0;
  }

  *vec = res;
  return 0;

}

int ScriptFloatVector::Subtract(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "Subtract", 2))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  Vector<3, float> res;

  //we are dealing with vector addition (our version or original
  if (lua_isuserdata(L, -1) || lua_istable(L, -1)) {
    
    Vector<3, float>* other; 

    if (lua_istable(L, -1)) 
      other = GetVectorFromOther(L);        
    else 
      other = (Vector<3, float>*) lua_touserdata(L, -1);          

    res = (*vec) - (*other);
  
  }
  //we are dealing with number addition
  else if (lua_isnumber(L, -1)) {

    float num = lua_tonumber(L, -1);

    res = (*vec) - num;
    
  }
  else {
    logger.info << "argument given to Subtract could not be processed. Only "
		<< "Vector<3, float>, FloatVector or constant are supported"
		<< logger.end;
    return 0;
  }

  lua_pop(L, 1);
  MakeVector(L, res);
  return 1;

}

int ScriptFloatVector::SubtractFrom(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SubtractFrom", 2))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  Vector<3, float> res;

  //we are dealing with vector addition (our version or original
  if (lua_isuserdata(L, -1) || lua_istable(L, -1)) {
    
    Vector<3, float>* other; 

    if (lua_istable(L, -1)) 
      other = GetVectorFromOther(L);        
    else 
      other = (Vector<3, float>*) lua_touserdata(L, -1);          

    res = (*vec) - (*other);
  
  }
  //we are dealing with number addition
  else if (lua_isnumber(L, -1)) {

    float num = lua_tonumber(L, -1);

    res = (*vec) - num;
    
  }
  else {
    logger.info << "argument given to SubtractFrom could not be processed. Only "
		<< "Vector<3, float>, FloatVector or constant are supported"
		<< logger.end;
    return 0;
  }

  *vec = res;
  return 0;

}

int ScriptFloatVector::Mul(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "Mul", 2) ||
      ScriptSystem::CheckArgType(L, "Mul", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  float num = lua_tonumber(L, -1);
  Vector<3, float> ans = (*vec) * num;
  lua_pop(L, 1);
  MakeVector(L, ans);
  return 1;

}


int ScriptFloatVector::MulTo(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "MulTo", 2) ||
      ScriptSystem::CheckArgType(L, "MulTo", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  float num = lua_tonumber(L, -1);
  Vector<3, float>  res = (*vec) * num;
  *vec = res;
  return 0;

}

int ScriptFloatVector::Devide(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "Devide", 2) ||
      ScriptSystem::CheckArgType(L, "Devide", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  float num = lua_tonumber(L, -1);
  Vector<3, float>  res = (*vec) / num;
  lua_pop(L, 1);
  MakeVector(L, res);
  return 1;

}

int ScriptFloatVector::DevideBy(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "Devide", 2) ||
      ScriptSystem::CheckArgType(L, "Devide", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  float num = lua_tonumber(L, -1);
  Vector<3, float>  res = (*vec) / num;
  *vec = res;
  return 0;

}

int ScriptFloatVector::GetNormalize(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "GetNormalize", 1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  Vector<3, float> ans = vec->GetNormalize();
  lua_pop(L, 1);
  MakeVector(L, ans);
  return 1;

}

int ScriptFloatVector::GetVector(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "GetVector", 1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  lua_pushlightuserdata(L, vec);
  return 1;

}

Vector<3, float>* ScriptFloatVector::GetVector() {

  return vec;

}

int ScriptFloatVector::GetX(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "GetX", 1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  float ans = (*vec)[0];
  lua_pushnumber(L, ans);
  return 1;
  
}

int ScriptFloatVector::GetY(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "GetY", 1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  float ans = (*vec)[1];
  lua_pushnumber(L, ans);
  return 1;
  
}

int ScriptFloatVector::GetZ(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "GetZ", 1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  float ans = (*vec)[2];
  lua_pushnumber(L, ans);
  return 1;
  
}

int ScriptFloatVector::SetX(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SetX", 2) ||
      ScriptSystem::CheckArgType(L, "SetX", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  float val = lua_tonumber(L, -1);
  (*vec)[0] = val;
  return 0;

}

int ScriptFloatVector::SetY(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SetX", 2) ||
      ScriptSystem::CheckArgType(L, "SetX", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  float val = lua_tonumber(L, -1);
  (*vec)[1] = val;
  return 0;

}

int ScriptFloatVector::SetZ(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SetX", 2) ||
      ScriptSystem::CheckArgType(L, "SetX", "float", 1, -1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  float val = lua_tonumber(L, -1);
  (*vec)[2] = val;
  return 0;

}

int TruncateFloat(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "TruncateFloat", 3))
    return 0;

  float max = lua_tonumber(L, -1);
  lua_pop(L, 1);

  Vector<3, float>* other;

  if (lua_istable(L, -1))
    other = GetVectorFromOther(L);
  else if (lua_isuserdata(L, -1))
    other = (Vector<3, float>*) lua_touserdata(L, -2);
  else {
    logger.info << "argument given to TruncateFloat could not be processed. Only "
		<< "Vector<3, float> or FloatVector"
		<< logger.end;
    return 0;
  }

  Vector<3, float> ans = VectorExt::Truncate(*other, max);
  lua_pop(L, 1);
  MakeVector(L, ans);
  return 1;

}

int ScriptFloatVector::SetToZero(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "SetToZero", 1))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  } 

  (*vec)[0] = 0.0;
  (*vec)[1] = 0.0;
  (*vec)[2] = 0.0;

  return 0;

}

int ScriptFloatVector::Dot(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "DotProduct", 2))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  Vector<3, float>* other;

  if (lua_istable(L, -1))
    other = GetVectorFromOther(L);
  else if (lua_isuserdata(L, -1))
    other = (Vector<3, float>*) lua_touserdata(L, -2);
  else {
    logger.info << "argument given to Dot could not be processed. Only "
		<< "Vector<3, float> or FloatVector are supported"
		<< logger.end;
    return 0;
  }

  float ans = (*vec)*(*other);
  lua_pushnumber(L, ans);
  return 1;

}

int ScriptFloatVector::GetLength(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "GetLength", 1))
    return 0;
  
  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  float length = vec->GetLength();
  lua_pushnumber(L, length);
  return 1;

}

int ScriptFloatVector::Cross(lua_State* L) {

  if (ScriptSystem::CheckStackSize(L, "Cross", 2))
    return 0;

  if (vec == NULL) {
    logger.info << "ScriptFloatVector not properly initialised" << logger.end;
    return 0;
  }

  Vector<3, float>* other;

  if (lua_istable(L, -1))
    other = GetVectorFromOther(L);
  else if (lua_isuserdata(L, -1))
    other = (Vector<3, float>*) lua_touserdata(L, -2);
  else {
    logger.info << "argument given to Cross could not be processed. Only "
		<< "Vector<3, float> or FloatVector are supported"
		<< logger.end;
    return 0;
  }

  Vector<3, float> res = (*vec)%(*other);
  lua_pop(L, 1);
  MakeVector(L, res);
  return 1;

}

Vector<3, float>* GetVectorFromOther(lua_State* L) {

  lua_pushnumber(L, 0); 
  lua_gettable(L, -2);
  ScriptFloatVector* vec = (ScriptFloatVector*) lua_touserdata(L, -1);
  lua_pop(L, 1); 
  return vec->GetVector();

}

void MakeVector(lua_State* L, Vector<3, float> vec) {

  lua_getglobal(L, "FloatVector");
  lua_pushnumber(L, vec[0]);
  lua_pushnumber(L, vec[1]);
  lua_pushnumber(L, vec[2]);
  lua_pcall(L, 3, LUA_MULTRET, 0);

}


