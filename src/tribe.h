/*
 * Copyright (C) 2002 by the Widelands Development Team
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __TRIBE_H
#define __TRIBE_H

#include "bob.h"
#include "worker.h"
#include "building.h"
#include "descr_maintainer.h"

/*
Tribes
------

Every player chooses a tribe. A tribe has distinct properties such as the
buildings it can build and the associated graphics.
Two players can choose the same tribe.
*/
class Tribe_Descr {
   public:
      enum {
         OK = 0,
         ERR_WRONGVERSION
      };
      
      Tribe_Descr(const char* name);
      ~Tribe_Descr(void);

		inline const char *get_name() const { return m_name; }
		
		inline int get_nrworkers() { return m_workers.get_nitems(); }
      inline Worker_Descr* get_worker_descr(uint idx) { return m_workers.get(idx); }
		inline int get_nrwares() { return m_wares.get_nitems(); }
		inline int get_ware_index(const char *name) { return m_wares.get_index(name); }
      inline Item_Ware_Descr* get_ware_descr(uint idx) { return m_wares.get(idx); }
      inline int get_nrbuildings() { return m_buildings.get_nitems(); }
		inline int get_building_index(const char *name) { return m_buildings.get_index(name); }
		inline Building_Descr *get_building_descr(uint idx) { return m_buildings.get(idx); }
		
      inline uint get_frontier_anim(void) { return m_anim_frontier; }
		inline uint get_flag_anim(void) { return m_anim_flag; }

		void postload(Editor_Game_Base*);
		void load_graphics();
		
      void load_warehouse_with_start_wares(Editor_Game_Base*, Warehouse*);
      
   private:
      char m_name[30];
      
      uint m_anim_frontier;
		uint m_anim_flag;
		
      Descr_Maintainer<Worker_Descr> m_workers;
      Descr_Maintainer<Building_Descr> m_buildings;
      Descr_Maintainer<Item_Ware_Descr> m_wares;
     
      std::map<std::string, int> m_startwares;
      std::map<std::string, int> m_startworkers;
      
      // Parsing the tribe
		EncodeData	m_default_encdata;
      	
		void parse_root_conf(const char *directory);
		void parse_buildings(const char *directory);
		void parse_workers(const char *directory);
		void parse_wares(const char *wares);

};

#endif //__TRIBE_H
