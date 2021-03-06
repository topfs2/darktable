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
#ifndef LUA_CONFIGURATION_H
#define LUA_CONFIGURATION_H
#include <lua/lua.h>

/*
 * LUA API VERSIONNING
 * This API versionning follows semantic versionning as defined in
 * http://semver.org
 * only stable releases are considered "released"
 *   => no need to increase API version with every commit,
 *   however, beware of stable releases and API changes
 */
// LAST RELEASED VERSION : 1.4 was 1.0.0
// 1.6 was 2.0.1
// 1.6.1 was 2.0.2
// 2.0.0 was 3.0.0
/* incompatible API change */
#define LUA_API_VERSION_MAJOR 3
/* backward compatible API change */
#define LUA_API_VERSION_MINOR 1
/* bugfixes that should not change anything to the API */
#define LUA_API_VERSION_PATCH 0
/* suffix for unstable version */
#define LUA_API_VERSION_SUFFIX "dev"

/** initialize lua stuff at DT start time */
int dt_lua_init_configuration(lua_State *L);
#endif
// modelines: These editor modelines have been set for all relevant files by tools/update_modelines.sh
// vim: shiftwidth=2 expandtab tabstop=2 cindent
// kate: tab-indents: off; indent-width 2; replace-tabs on; indent-mode cstyle; remove-trailing-spaces modified;
