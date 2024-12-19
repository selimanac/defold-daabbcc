local collision                  = require("example.scripts.lib.collision")
local camera                     = require("example.scripts.lib.camera")

local manager                    = {}
local info_label_url             = msg.url(".")
local collection_title_label_url = msg.url(".")
local next_btn_url               = msg.url(".")
local pointer_url                = msg.url(".")
local next_btn_id                = 0
local pointer_id                 = 0
local info_string                = "0 - Reset / 1 - Sort: %s / 2 - Manifold: %s / 3 - Mask: %s"
local KEYS                       = {
	AABB_DEFAULT = hash("aabb_default"),
	AABB_SORT = hash("aabb_sort"),
	AABB_MANIFOLD = hash("aabb_manifold"),
	AABB_MASK = hash("aabb_mask"),
}

manager.world_position           = vmath.vector3()
manager.is_sort                  = false
manager.is_manifold              = false
manager.is_mask                  = false

local function set_info_text()
	label.set_text(info_label_url, string.format(info_string, tostring(manager.is_sort), tostring(manager.is_manifold), tostring(manager.is_mask)))
end

local function toogle_sort()
	manager.is_sort = not manager.is_sort and true or false
	set_info_text()
end

local function toogle_manifold()
	manager.is_manifold = not manager.is_manifold and true or false
	set_info_text()
end

function manager.toogle_manifold()
	toogle_manifold()
end

local function toogle_mask()
	manager.is_mask = not manager.is_mask and true or false
	set_info_text()
end

local function toogle_default()
	manager.is_sort     = false
	manager.is_manifold = false
	manager.is_mask     = false
	set_info_text()
end

function manager.init(title_txt)
	msg.post(".", "acquire_input_focus")
	msg.post("@render:", "clear_color", { color = vmath.vector4(100 / 255, 100 / 255, 100 / 255, 0) })

	pointer_url                         = msg.url(".")
	pointer_url.path                    = "/container/pointer"

	info_label_url                      = msg.url(".")
	info_label_url.path                 = "/container/info"
	info_label_url.fragment             = "label"

	collection_title_label_url          = msg.url(".")
	collection_title_label_url.path     = "/container/info"
	collection_title_label_url.fragment = "collection_txt"
	label.set_text(collection_title_label_url, title_txt)

	next_btn_url                 = msg.url(".")
	next_btn_url.path            = "/container/next_btn"

	local next_btn_sprite_url    = msg.url(".")
	next_btn_sprite_url.path     = "/container/next_btn"
	next_btn_sprite_url.fragment = "sprite"

	local next_btn_position      = go.get_position(next_btn_url)
	local next_btn_size          = go.get(next_btn_sprite_url, "size")

	camera.init()
	collision.init()

	next_btn_id = collision.insert_aabb(next_btn_position.x, next_btn_position.y, next_btn_size.x, next_btn_size.y, collision.collision_bits.BUTTON)
	pointer_id = collision.insert_gameobject(pointer_url, 5, 5, collision.collision_bits.POINTER)

	set_info_text()
end

function manager.input(action_id, action)
	manager.world_position = camera.screen_to_world(action.x, action.y, 0)
	go.set_position(manager.world_position, pointer_url)

	if action_id == KEYS.AABB_DEFAULT and action.pressed then
		toogle_default()
	elseif action_id == KEYS.AABB_SORT and action.pressed then
		toogle_sort()
	elseif action_id == KEYS.AABB_MANIFOLD and action.pressed then
		toogle_manifold()
	elseif action_id == KEYS.AABB_MASK and action.pressed then
		toogle_mask()
	end

	local result, _ = collision.query_id_btn(pointer_id)
	if result and action.pressed then
		msg.post("load:/proxies#load", "next", {})
	end
end

function manager.final()
	manager.world_position = vmath.vector3()
	manager.is_sort        = false
	manager.is_manifold    = false
	manager.is_mask        = false

	collision.reset()
end

return manager
