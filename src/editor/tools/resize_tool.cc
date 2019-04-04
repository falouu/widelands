/*
 * Copyright (C) 2002-2019 by the Widelands Development Team
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "editor/tools/resize_tool.h"

#include "editor/editorinteractive.h"
#include "logic/field.h"
#include "logic/widelands_geometry.h"

int32_t EditorResizeTool::handle_click_impl(const Widelands::World& world,
                                                  const Widelands::NodeAndTriangle<>& center,
                                                  EditorInteractive& parent,
                                                  EditorActionArgs* args,
                                                  Widelands::Map* map) {
	Widelands::EditorGameBase& egbase = parent.egbase();

	args->resized.old_map_size = map->extent();
	args->resized.port_spaces.clear();
	args->resized.starting_positions.clear();
	for (const Widelands::Coords& ps : map->get_port_spaces()) {
		args->resized.port_spaces.push_back(Widelands::Coords(ps));
	}
	for (uint8_t i = 1; i <= map->get_nrplayers(); ++i) {
		args->resized.starting_positions.push_back(map->get_starting_pos(i));
	}

	args->resized.deleted_fields = map->resize(egbase, center.node, args->new_map_size.w, args->new_map_size.h);

	map->recalc_whole_map(world);
	return 0;
}

int32_t EditorResizeTool::handle_undo_impl(
   const Widelands::World& world,
   const Widelands::NodeAndTriangle<Widelands::Coords>& center,
   EditorInteractive& parent,
   EditorActionArgs* args,
   Widelands::Map* map) {
	Widelands::EditorGameBase& egbase = parent.egbase();

	map->resize(egbase, center.node, args->resized.old_map_size.w, args->resized.old_map_size.h);

	for (const auto& it : args->resized.deleted_fields) {
		const Widelands::FCoords f = map->get_fcoords(it.first);
		const Widelands::FieldData data = it.second;

		if (Widelands::BaseImmovable* imm = f.field->get_immovable()) {
			imm->remove(egbase);
		}
		for (Widelands::Bob* bob = f.field->get_first_bob(); bob; bob = bob->get_next_bob()) {
			bob->remove(egbase);
		}

		f.field->set_height(data.height);
		f.field->set_terrains(data.terrains);
		map->initialize_resources(f, data.resources, data.resource_amount);

		if (data.immovable != "") {
			egbase.create_immovable_with_name(f, data.immovable,
					Widelands::MapObjectDescr::OwnerType::kWorld, nullptr, nullptr);
		}
		for (const std::string& bob : data.bobs) {
			egbase.create_critter(f, bob);
		}
	}

	for (const Widelands::Coords& c : args->resized.port_spaces) {
		map->set_port_space(world, c, true);
	}
	for (uint8_t i = 1; i <= map->get_nrplayers(); ++i) {
		map->set_starting_pos(i, args->resized.starting_positions[i - 1]);
	}

	map->recalc_whole_map(world);
	return 0;
}

EditorActionArgs EditorResizeTool::format_args_impl(EditorInteractive& parent) {
	EditorActionArgs a(parent);
	a.new_map_size = Widelands::Extent(width_, height_);
	return a;
}

