---@class daabbcc
daabbcc = {
	UPDATE_INCREMENTAL = 0,
	UPDATE_FULLREBUILD = 1,
	UPDATE_PARTIALREBUILD = 2
}



---@alias rebuildType integer
---| 'UPDATE_INCREMENTAL' [# description]
---| 'UPDATE_FULLREBUILD' [# description]
---| 'UPDATE_PARTIALREBUILD' [# description]



---init
---@param max_group_count integer
---@param max_gameobject_count integer
---@param max_query_count integer
---@param max_raycast_count integer
---@param update_frequency? integer
function daabbcc.init(max_group_count, max_gameobject_count, max_query_count, max_raycast_count, update_frequency) end

---new_group
---@param rebuild_type rebuildType
---@return integer group_id
function daabbcc.new_group(rebuild_type) end

---remove_group
---@param group_id integer
function daabbcc.remove_group(group_id) end

---insert_aabb
---@param group_id integer
---@param x number
---@param y number
---@param width integer
---@param height integer
---@param mask_bits? integer
---@return integer aabb_id
function daabbcc.insert_aabb(group_id, x, y, width, height, mask_bits) end

---insert_gameobject
---@param group_id integer
---@param go_url url
---@param width integer
---@param height integer
---@param mask_bits? integer
---@return integer aabb_id
function daabbcc.insert_gameobject(group_id, go_url, width, height, mask_bits) end

---update_aabb
---@param group_id integer
---@param aabb_id integer
---@param x number
---@param y number
---@param width integer
---@param height integer
function daabbcc.update_aabb(group_id, aabb_id, x, y, width, height) end

---update_aabb
---@param group_id integer
---@param aabb_id integer
---@param width integer
---@param height integer
function daabbcc.update_gameobject_size(group_id, aabb_id, width, height) end

---remove
---@param group_id integer
---@param aabb_id integer
function daabbcc.remove(group_id, aabb_id) end

---query_aabb
---@param group_id integer
---@param x number
---@param y number
---@param width integer
---@param height integer
---@param mask_bits? integer
---@return table result
---@return integer count
function daabbcc.query_aabb(group_id, x, y, width, height, mask_bits) end

---query_id
---@param group_id integer
---@param mask_bits? integer
---@return table result
---@return integer count
function daabbcc.query_id(group_id, aabb_id, mask_bits) end

---query_aabb_sort
---@param group_id integer
---@param x number
---@param y number
---@param width integer
---@param height integer
---@param mask_bits? integer
---@return table result
---@return integer count
function daabbcc.query_aabb_sort(group_id, x, y, width, height, mask_bits) end

---query_id_sort
---@param group_id integer
---@param mask_bits? integer
---@return table result
---@return integer count
function daabbcc.query_id_sort(group_id, aabb_id, mask_bits) end

---raycast
---@param group_id integer
---@param start_x number
---@param start_y number
---@param end_x number
---@param end_y number
---@param mask_bits? integer
---@return table result
---@return integer count
function daabbcc.raycast(group_id, start_x, start_y, end_x, end_y, mask_bits) end

---raycast_sort
---@param group_id integer
---@param start_x number
---@param start_y number
---@param end_x number
---@param end_y number
---@param mask_bits? integer
---@return table result
---@return integer count
function daabbcc.raycast_sort(group_id, start_x, start_y, end_x, end_y, mask_bits) end

---run
---@param state boolean
function daabbcc.run(state) end

---update_frequency
---@param frequency integer
function daabbcc.update_frequency(frequency) end

---rebuild
---@param group_id integer
---@param full_build boolean
function daabbcc.rebuild(group_id, full_build) end

---rebuild_all
---@param full_build boolean
function daabbcc.rebuild_all(full_build) end

---reset
function daabbcc.reset() end
