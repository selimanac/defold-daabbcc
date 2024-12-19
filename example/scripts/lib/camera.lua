local camera         = {}
local DISPLAY_WIDTH  = sys.get_config_int("display.width")
local DISPLAY_HEIGHT = sys.get_config_int("display.height")
local camera_url     = msg.url(".")

function camera.init()
	msg.post("@render:", "use_camera_projection")

	camera_url = msg.url(".")
	camera_url.path = "/container/camera"
	camera_url.fragment = "camera"
end

function camera.screen_to_world(x, y, z)
	local projection = go.get(camera_url, "projection")
	local view = go.get(camera_url, "view")
	local w, h = window.get_size()

	w = w / (w / DISPLAY_WIDTH)
	h = h / (h / DISPLAY_HEIGHT)

	local inv = vmath.inv(projection * view)
	x = (2 * x / w) - 1
	y = (2 * y / h) - 1
	z = (2 * z) - 1
	local x1 = x * inv.m00 + y * inv.m01 + z * inv.m02 + inv.m03
	local y1 = x * inv.m10 + y * inv.m11 + z * inv.m12 + inv.m13
	local z1 = x * inv.m20 + y * inv.m21 + z * inv.m22 + inv.m23
	return vmath.vector3(x1, y1, z1)
end

return camera
