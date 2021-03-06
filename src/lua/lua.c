/*
   This file is part of darktable,
   copyright (c) 2012 Jeremy Rosen

   darktable is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   darktable is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with darktable.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "lua/lua.h"
#include "common/darktable.h"
#include "control/control.h"
#include "lua/call.h"

void dt_lua_debug_stack_internal(lua_State *L, const char *function, int line)
{
  printf("lua stack at %s:%d", function, line);
  if(!L)
  {
    printf(" Stack is NULL\n");
    return;
  }
  else
  {
    printf("(size %d),\n",lua_gettop(L)); //usefull to detect underflows
  }
  for(int i = 1; i <= lua_gettop(L); i++)
  {
#if 1
    printf("\t%d:%s %s\n", i, lua_typename(L, lua_type(L, i)), luaL_tolstring(L, i, NULL));
    lua_pop(L, 1); // remove the result of luaL_tolstring() from the stack
#else
    // no tolstring when stack is really screwed up 
    printf("\t%d:%s %p\n", i, lua_typename(L, lua_type(L, i)),lua_topointer(L,i));
#endif
  }
}

void dt_lua_debug_table_internal(lua_State *L, int t, const char *function, int line)
{
  t = lua_absindex(L,t);
  /* table is in the stack at index 't' */
  lua_len(L,t);
  printf("lua table at index %d at %s:%d (length %f)\n", t, function, line,lua_tonumber(L,-1));
  lua_pop(L,1);
  if(lua_type(L, t) != LUA_TTABLE)
  {
    printf("\tnot a table: %s\n", lua_typename(L, lua_type(L, t)));
    return;
  }
  lua_pushnil(L); /* first key */
  while(lua_next(L, t ) != 0)
  {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    if(lua_type(L,-2) != LUA_TNUMBER) {
      printf("%s - %s\n", lua_tostring(L, -2), lua_typename(L, lua_type(L, -1)));
    } else {
      printf("%f - %s\n", luaL_checknumber(L, -2), lua_typename(L, lua_type(L, -1)));
    }

    /* removes 'value'; keeps 'key' for next iteration */
    lua_pop(L, 1);
  }
}


int dt_lua_push_darktable_lib(lua_State *L)
{
  lua_getfield(L, LUA_REGISTRYINDEX, "dt_lua_dtlib");
  if(lua_isnil(L, -1))
  {
    lua_pop(L, 1);
    lua_newtable(L);
    lua_newtable(L);
    lua_setmetatable(L, -2);
    lua_pushvalue(L, -1);
    lua_setfield(L, LUA_REGISTRYINDEX, "dt_lua_dtlib");
  }
  return 1;
}


void dt_lua_goto_subtable(lua_State *L, const char *sub_name)
{
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, sub_name);
  if(lua_isnil(L, -1))
  {
    lua_pop(L, 1);
    lua_newtable(L);
    lua_setfield(L, -2, sub_name);
    lua_getfield(L, -1, sub_name);
  }
  lua_remove(L, -2);
}

void dt_lua_init_lock()
{
  pthread_mutexattr_t a;
  pthread_mutexattr_init(&a);
  pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
  dt_pthread_mutex_init(&darktable.lua_state.mutex, &a);
  pthread_mutexattr_destroy(&a);
  // we want our lock initialized locked
  dt_pthread_mutex_lock(&darktable.lua_state.mutex);
}

void dt_lua_lock_internal(const char *function, const char *file, int line, gboolean silent)
{
  if(!silent && !darktable.lua_state.ending && pthread_equal(darktable.control->gui_thread, pthread_self()) != 0)
  {
    dt_print(DT_DEBUG_LUA, "LUA WARNING locking from the gui thread should be avoided\n");
    //g_assert(false);
  }

  dt_pthread_mutex_lock(&darktable.lua_state.mutex);
#ifdef _DEBUG
  dt_print(DT_DEBUG_LUA,"LUA DEBUG : %s called from %s:%d (%s)\n", __FUNCTION__, file, line, function);
#endif
}
void dt_lua_unlock_internal(const char *function, int line)
{
#ifdef _DEBUG
  dt_print(DT_DEBUG_LUA,"LUA DEBUG : %s called from %s %d\n",__FUNCTION__,function,line);
#endif
  dt_pthread_mutex_unlock(&darktable.lua_state.mutex);
}

static gboolean async_redraw(gpointer data)
{
  dt_control_queue_redraw();
  return false;
}

void dt_lua_redraw_screen()
{
  if(darktable.gui != NULL)
  {
    g_idle_add(async_redraw,NULL);
  }
}



// modelines: These editor modelines have been set for all relevant files by tools/update_modelines.sh
// vim: shiftwidth=2 expandtab tabstop=2 cindent
// kate: tab-indents: off; indent-width 2; replace-tabs on; indent-mode cstyle; remove-trailing-spaces modified;
