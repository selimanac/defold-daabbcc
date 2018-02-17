#define LIB_NAME "DAABBCC"
#define MODULE_NAME "daabbcc"

#include <dmsdk/sdk.h>
#include <iostream>
#include <aabb/AABB.h>

using namespace std;
using namespace aabb;

/*TODO: Camera update. */

Tree  * treeObjectPointer; // Tree Pointer
vector<Tree  *> treeArr; // Tree Array
vector<string> treeTxtArr; // Tree name array for name conversation

unsigned int particleCount = 0; // Particle IDs

//Debug vector values
void pprint (const vector<unsigned int>& v){
  cout << "HIT: \n";
  for (int i=0; i<v.size();i++){
    cout << v[i] << endl;
  }
}

void warning(string _name){
  cout << "WARNING!! : " << _name << " tree not found. Check your tree name. \n";
}

//Tree name conversation
pair<bool, int> checkTreeName(string _name)
{
  long pos = find(treeTxtArr.begin(), treeTxtArr.end(), _name) - treeTxtArr.begin();
  if( pos < treeTxtArr.size() ) return std::make_pair(true, pos);
  warning(_name);
  return std::make_pair(false, 0);
}

//Bounds
pair< vector<double> , vector<double> > calculateBounds(double x, double y, double w, double h)
{
  double xl = x - (w/2);
  double yb = y - (h/2);
  double xr = x + (w/2);
  double yt = y + (h/2);
  vector<double> lowerBound;
  lowerBound.push_back(xl);
  lowerBound.push_back(yb);
  vector<double> upperBound;
  upperBound.push_back(xr);
  upperBound.push_back(yt);
  return std::make_pair(lowerBound, upperBound);
}

//Constructor (non-periodic)
static int createTree(lua_State* L)
{
  int top = lua_gettop(L);
  string _name =  luaL_checkstring(L, 1);
  unsigned int _dimension =  luaL_checknumber(L, 2);
  double _skinThickness = luaL_checknumber(L, 3);
  unsigned int _nParticles = luaL_checknumber(L, 4);
  treeObjectPointer = new Tree(_dimension, _skinThickness, _nParticles);
  treeArr.push_back (treeObjectPointer);
  treeTxtArr.push_back(_name);
  return 0;
}

//Query the tree to find candidate interactions for a particle ID.
static int queryID(lua_State* L){
  int top = lua_gettop(L);
  string _name =  luaL_checkstring(L, 1);
  unsigned int _id =  luaL_checknumber(L, 2);
  vector<unsigned int> particles;
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first) {
    particles = treeArr[_result.second]->query(_id);
  }
  lua_createtable(L, particles.size(), 0);
  int newTable = lua_gettop(L);
  for(int i=0; i < particles.size(); i++) {
    lua_pushnumber(L, particles[i]);
    lua_rawseti(L, newTable, i + 1);
  }
  assert(top + 1 == lua_gettop(L));
  return 1;
}

//Query the tree to find candidate interactions for an AABB.
static int queryAABB(lua_State* L){
  int top = lua_gettop(L);
  string _name =  luaL_checkstring(L, 1);
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double w = luaL_checknumber(L, 4);
  double h = luaL_checknumber(L, 5);
  vector<unsigned int> particles;
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first) {
    pair< vector<double> , vector<double>  > _bounds = calculateBounds(x,y,w,h);
    AABB aabb(_bounds.first, _bounds.second);
    particles = treeArr[_result.second]->query(aabb);
  }
  lua_createtable(L, particles.size(), 0);
  int newTable = lua_gettop(L);
  for(int i=0; i < particles.size(); i++) {
    lua_pushnumber(L, particles[i]);
    lua_rawseti(L, newTable, i + 1);
  }
  assert(top + 1 == lua_gettop(L));
  return 1;
}

//Insert a particle into the tree (point particle)
static int insertCircle(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  double _radius = luaL_checknumber(L, 2);
  double x = luaL_checknumber(L, 3);
  double y = luaL_checknumber(L, 4);
  unsigned int _index = particleCount;
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first) {
    vector<double> _position;
    _position.push_back(x);
    _position.push_back(y);
    treeArr[_result.second]->insertParticle(particleCount, _position, _radius);
    particleCount++;
  }
  lua_pushinteger(L, _index);
  assert(top + 1 == lua_gettop(L));
  return 1;
}

//Insert a particle into the tree (arbitrary shape with bounding box)
static int insertRect(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double w = luaL_checknumber(L, 4);
  double h = luaL_checknumber(L, 5);
  unsigned int _index = particleCount;
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    pair< vector<double> , vector<double>  > _bounds = calculateBounds(x,y,w,h);
    treeArr[_result.second]->insertParticle(particleCount, _bounds.first, _bounds.second);
    particleCount++;
  }
  lua_pushinteger(L, _index);
  assert(top + 1 == lua_gettop(L));
  return 1;
}

//Remove a particle from the tree.
static int removeAABB(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  unsigned int _id = luaL_checknumber(L, 2);
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    treeArr[_result.second]->removeParticle(_id);
  }
  return 0;
}

//Update the tree if a particle moves outside its fattened AABB.
static int updateCircle(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  unsigned int _id = luaL_checknumber(L, 2);
  double _radius = luaL_checknumber(L, 3);
  double x = luaL_checknumber(L, 4);
  double y = luaL_checknumber(L, 5);
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    vector<double> _position;
    _position.push_back(x);
    _position.push_back(y);
    treeArr[_result.second]->updateParticle(_id, _position, _radius);
  }
  return 0;
}

//Update the tree if a particle moves outside its fattened AABB.
static int updateRect(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  unsigned int _id = luaL_checknumber(L, 2);
  double x = luaL_checknumber(L, 3);
  double y = luaL_checknumber(L, 4);
  double w = luaL_checknumber(L, 5);
  double h = luaL_checknumber(L, 6);
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    pair< vector<double> , vector<double>  > _bounds = calculateBounds(x,y,w,h);
    treeArr[_result.second]->updateParticle(_id, _bounds.first, _bounds.second);
  }
  return 0;
}

/*---------------
**** Helpers ****
-----------------*/

//Validate the tree.
static int validateTree(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    treeArr[_result.second]->validate();
  }
  return 0;
}

//Rebuild an optimal tree.
static int rebuildTree(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    treeArr[_result.second]->rebuild();
  }
  return 0;
}

//Get the height of the binary tree..
static int getHeight(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  unsigned int _height=0;
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    _height = treeArr[_result.second]->getHeight();
  }
  lua_pushinteger(L, _height);
  assert(top + 1 == lua_gettop(L));
  return 1;
}

//Get the number of nodes in the tree.
static int getNodeCount(lua_State* L){
  int top = lua_gettop(L);
  string _name = luaL_checkstring(L, 1);
  unsigned int _nodeCount=0;
  pair<bool, int> _result = checkTreeName(_name);
  if( _result.first ) {
    _nodeCount = treeArr[_result.second]->getNodeCount();
  }
  lua_pushinteger(L, _nodeCount);
  assert(top + 1 == lua_gettop(L));
  return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
  {"rebuildTree", rebuildTree},
  {"validateTree", validateTree},
  {"queryAABB", queryAABB},
  {"queryID", queryID},
  {"updateRect", updateRect},
  {"updateCircle", updateCircle},
  {"getNodeCount", getNodeCount},
  {"getHeight", getHeight},
  {"removeAABB", removeAABB},
  {"insertCircle", insertCircle},
  {"insertRect", insertRect},
  {"createTree", createTree},
  {0, 0}
};

static void LuaInit(lua_State* L)
{
  int top = lua_gettop(L);
  luaL_register(L, MODULE_NAME, Module_methods);
  lua_pop(L, 1);
  assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeDAABBCC(dmExtension::AppParams* params)
{
  return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDAABBCC(dmExtension::Params* params)
{
  LuaInit(params->m_L);
  printf("Registered %s Extension\n", MODULE_NAME);
  return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeDAABBCC(dmExtension::AppParams* params)
{
  return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDAABBCC(dmExtension::Params* params)
{
  return dmExtension::RESULT_OK;
}


// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

DM_DECLARE_EXTENSION(DAABBCC, LIB_NAME, AppInitializeDAABBCC, AppFinalizeDAABBCC, InitializeDAABBCC, 0, 0, FinalizeDAABBCC)
