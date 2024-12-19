local collision        = require("example.scripts.lib.collision")
local manager          = require("example.scripts.lib.manager")
local utils            = require("example.scripts.lib.utils")

local data             = {}
local active_boxes     = {}
local active_box_count = 0
local box_factory      = "/container/factories#box"
local BOX_SIZE         = {
	SQUARE = { width = 40, height = 40 },
	RECTANGLE = { width = 120, height = 60 },
}

data.BOX_TYPE          = {
	AABB = {
		size          = BOX_SIZE.SQUARE,
		collision_bit = collision.collision_bits.ITEM,
		anim          = "aabb_40x40",
		sprite_url    = msg.url(),
		label_url     = msg.url(),
		id            = hash(""),
		aabb_id       = 0,
		position      = vmath.vector3(),
		selected      = false

	},
	ITEM = {
		size          = BOX_SIZE.SQUARE,
		collision_bit = collision.collision_bits.ITEM,
		anim          = "aabb_40x40",
		sprite_url    = msg.url(),
		label_url     = msg.url(),
		id            = hash(""),
		aabb_id       = 0,
		position      = vmath.vector3(),
		selected      = false

	},
	ENEMY = {
		size          = BOX_SIZE.SQUARE,
		collision_bit = collision.collision_bits.ENEMY,
		anim          = "enemy_40x40",
		sprite_url    = msg.url(),
		label_url     = msg.url(),
		id            = hash(""),
		aabb_id       = 0,
		position      = vmath.vector3(),
		selected      = false
	},
	GROUND = {
		size          = BOX_SIZE.RECTANGLE,
		collision_bit = collision.collision_bits.GROUND,
		anim          = "ground_120x60",
		sprite_url    = msg.url(),
		label_url     = msg.url(),
		id            = hash(""),
		aabb_id       = 0,
		position      = vmath.vector3(),
		selected      = false
	},
	PLAYER = {
		size          = BOX_SIZE.SQUARE,
		collision_bit = collision.collision_bits.PLAYER,
		anim          = "player_40x40",
		sprite_url    = msg.url(),
		label_url     = msg.url(),
		id            = hash(""),
		aabb_id       = 0,
		position      = vmath.vector3(),
		selected      = false
	}
}

data.aabbs             = {}
data.boxes             = {}

local function setup_box(box, box_position)
	box.position   = box_position
	box.id         = factory.create(box_factory, box.position)
	box.sprite_url = msg.url(nil, box.id, "sprite")
	box.label_url  = msg.url(nil, box.id, "label")
	sprite.play_flipbook(box.sprite_url, box.anim)

	return box
end

function data.add_box(box_position, box_type, box_static, animate)
	animate = animate and true or false

	local box = setup_box(utils.table_copy(box_type), box_position)

	if box_static then
		box.aabb_id = collision.insert_aabb(box.position.x, box.position.y, box.size.width, box.size.height, box.collision_bit)
	else
		box.aabb_id = collision.insert_gameobject(box.id, box.size.width, box.size.height, box.collision_bit)
	end

	label.set_text(box.label_url, tostring(box.aabb_id))

	table.insert(data.boxes, box)
	table.insert(data.aabbs, box.aabb_id, #data.boxes)

	if animate then
		go.animate(box.id, "position.x", go.PLAYBACK_LOOP_PINGPONG, box.position.x + 100, go.EASING_INOUTELASTIC, 4)
	end
end

function data.add_aabb()
	return setup_box(utils.table_copy(data.BOX_TYPE.AABB), vmath.vector3(0, 0, 0))
end

local function clear_highlight(id)
	local box_id = data.aabbs[id]
	local box = data.boxes[box_id]
	box.selected = false
	go.cancel_animations(box.sprite_url, "tint.y")
	go.set(box.sprite_url, "tint.y", 1)
end

function data.clear_highlights()
	for _, v in pairs(active_boxes) do
		clear_highlight(v)
	end
	active_boxes = {}
end

function data.highlight(result, count)
	local result_aabb_id = 0
	local box_id = 0
	local box = {}

	for i = 1, count do
		if manager.is_sort or manager.is_manifold then
			result_aabb_id = result[i].id
		else
			result_aabb_id = result[i]
		end

		box_id = data.aabbs[result_aabb_id]
		box = data.boxes[box_id]

		if not box.selected then
			box.selected = true
			go.animate(box.sprite_url, "tint.y", go.PLAYBACK_LOOP_PINGPONG, 0.5, go.EASING_LINEAR, 0.4)
			active_boxes[box.aabb_id] = box.aabb_id
		end
	end

	for k, aabb_id in pairs(active_boxes) do
		for i = 1, count do
			if manager.is_sort or manager.is_manifold then
				result_aabb_id = result[i].id
			else
				result_aabb_id = result[i]
			end

			if aabb_id == result_aabb_id then
				active_box_count = active_box_count + 1
			end
		end

		if active_box_count == 0 then
			clear_highlight(aabb_id)
			active_boxes[k] = nil
		end

		active_box_count = 0
	end
end

function data.reset()
	data.aabbs = {}
	data.boxes = {}
end

return data
