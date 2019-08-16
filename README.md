![DAABBCC](https://github.com/selimanac/DAABBCC/blob/v2.0/assets/header.png?raw=true)

This is a [Dynamic AABB Tree](http://box2d.org/files/GDC2019/ErinCatto_DynamicBVH_Full.pdf) native extension with Branch and Bound Algorithm for [Defold Engine](https://www.defold.com/). DAABBCC build by using Box2D’s [Dynamic Tree](https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2DynamicTree.cpp).

Special thank to [Randy Gaul](https://twitter.com/RandyPGaul) for [Branch and Bound](https://github.com/erincatto/Box2D/pull/548) implementation.  
Box2D Copyright (c) 2009 Erin Catto http://www.box2d.org

**What is DAABBCC?**

A Dynamic AABB Tree is a binary search algorithm for fast overlap testing. Dynamic AABB trees are good for general purpose use, and can handle moving objects very well. The data structure provides an efficient way of detecting potential overlap between objects. DAABBCC does not contain narrow-phase collision detection.

You can use it whenever precise collision manifold is not required. 


## Installation

You can use the DAABBCC in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/).  
Open your game.project file and in the dependencies field under project add:

>https://github.com/selimanac/DAABBCC/archive/v2.0.zip


## Release Notes

2.0

This is a complete rewritten version by using Box2D’s [Dynamic Tree](https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2DynamicTree.cpp).  
- API revised and simplify     
- Nearly 6x performance increment according to v1.x (4800 sprites vs 500 sprites)  
- Low cpu usage
- Experimental narrow phase collision detections removed

[1.1](https://github.com/selimanac/DAABBCC/releases/tag/v1.1)

Final 1.x release.   
- Updated cute_c2 version to 1.05

1.0

Initial release.


## API

### aabb.new_group()

New group for AABBs.  
**Returns group id.**

```lua
local enemy_group = aabb.new_group()
```

### aabb.insert(`group_id, x, y, w, h`)

Insert AABB into the group.  
**Returns aabb id.**

```lua
local x = 0
local y = 0
local w = 50
local h = 50

local enemy_id = aabb.insert(enemy_group, x, y, w, h)
```

You can get position and size from sprite.

```lua
self.pos = go.get_position(".")
self.size = go.get("#sprite", "size")

self.enemy_id = aabb.insert(enemy_group, self.pos.x , self.pos.y, self.size.x, self.size.y)
```

### aabb.update(`group_id, aabb_id, x, y, w, h`)

Updates the AABB position and size when you change it is position or size.

```lua
local new_x = 10
local new_y = 10
local new_w = 60
local new_h = 60

aabb.update(enemy_group, self.enemy_id, new_x, new_y, new_w, new_h)
```

### aabb.query_id(`group_id, aabb_id`)

Query the possible overlaps using ID.  
**Returns result table with ids.**

```lua
local result = aabb.query_id(enemy_group, self.enemy_id)
```

### aabb.query(`group_id, x, y, w, h`)

Query the possible overlaps using AABB.  
**Returns result table with ids.**

```lua
local result = aabb.query(enemy_group, x, y, w, h)
```

### aabb.raycast(`group_id, start_x, start_y, end_x, end_y`)

Query the possible overlaps using RAYCAST.  
**Returns result table with ids.**

```lua
local ray_start = vmath.vector3(0, 0, 0)
local ray_end = vmath.vector3(365, 370, 0)

local result = aabb.raycast(enemy_group, ray_start.x, ray_start.y, ray_end.x, ray_end.y)
```

### aabb.remove_group(`group_id`)

Removes the group and cleans all AABBs

```lua
aabb.remove_group(enemy_group)
```




