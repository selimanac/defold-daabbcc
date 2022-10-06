DEV 2.1

TODO:  

- [x] Short results
- [x] Get positions from Gameobjects
- [x] Remove external libs
- [x] Remove group limit
- [ ] Remove Gameobject when AABB or Group removed 
- [x] Add Clear function to clear all remanings.


![DAABBCC](https://github.com/selimanac/DAABBCC/blob/v2.0/assets/header.png?raw=true)

This is a [Dynamic AABB Tree](https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf) native extension with Branch and Bound Algorithm for [Defold Engine](https://www.defold.com/).  
DAABBCC build by using Box2D’s [Dynamic Tree](https://github.com/erincatto/box2d/blob/master/src/collision/b2_dynamic_tree.cpp).

Special thanks to [Randy Gaul](https://twitter.com/RandyPGaul) for [Branch and Bound](https://github.com/erincatto/Box2D/pull/548) implementation.  
[Radix sort](https://github.com/JCash/containers) by [Mathias Westerdahl](https://github.com/JCash)  
Box2D Copyright (c) 2009 Erin Catto http://www.box2d.org

**What is DAABBCC?**

A Dynamic AABB Tree is a binary search algorithm for fast overlap testing. Dynamic AABB trees are good for general purpose use, and can handle moving objects very well. The data structure provides an efficient way of detecting potential overlap between objects. DAABBCC does not contain narrow-phase collision detection.

You can use it whenever precise collision manifold(narrow-phase) is not required. 


## Installation

You can use the DAABBCC in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/).  
Open your game.project file and in the dependencies field under project add:

>https://github.com/selimanac/DAABBCC/archive/refs/tags/v2.1.zip


## Release Notes

2.1
- It is now possible to short results by distance. Use `raycast_short`, `query_id_short`, `query_short` according to your needs [#5](https://github.com/selimanac/DAABBCC/issues/5).
- Automated position updates for Defold Gameobjects [#6](https://github.com/selimanac/DAABBCC/issues/6).
- External Array and HashTable libs are removed. 
- Group limit removed(Previously, it was limited to 20)
- Clear function added for removing and reseting everything. 

2.0

This is a complete rewritten version by using Box2D’s [Dynamic Tree](https://github.com/erincatto/Box2D/blob/master/Box2D/Collision/b2DynamicTree.cpp).  
- API revised and simplified    
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

New group for AABBs. Currently groups has limited to 20.  
>Returns group id.

```lua
local enemy_group = aabb.new_group()
```

### aabb.insert(`group_id, x, y, w, h`)

Insert AABB into the group.  
>Returns aabb id.

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

### aabb.insert_gameobject(`group_id, url, w, h`)
Insert Gameobject and its AABB into group.   
Gameobject ID is necessary when removing gameobject(and its aabb) and setting new width, height.

>Returns aabb id and Gameobject ID.

```lua
local go_url = msg.url("/go")
local w = 50
local h = 50

local aabb_id, go_id = aabb.insert_gameobject(enemy_group, go_url, w, h)
```


### aabb.update(`group_id, aabb_id, x, y, w, h`)

Updates the AABB position and size when you change it is position or size.  
Gameobject AABB positions will be overwritten.

```lua
local new_x = 10
local new_y = 10
local new_w = 60
local new_h = 60

aabb.update(enemy_group, self.enemy_id, new_x, new_y, new_w, new_h)
```

### aabb.query_id(`group_id, aabb_id`)

Query the possible overlaps using ID.  
>Returns result table with ids.

```lua
local result = aabb.query_id(enemy_group, self.enemy_id)
```

### aabb.query(`group_id, x, y, w, h`)

Query the possible overlaps using AABB.  
>Returns result table with ids.

```lua
local result = aabb.query(enemy_group, x, y, w, h)
```

### aabb.raycast(`group_id, start_x, start_y, end_x, end_y`)

Query the possible overlaps using RAYCAST.  
>Returns result table with ids.

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

### aabb.remove(`group_id, aabb_id`)

Removes the AABB from group

```lua
aabb.remove(group_id, aabb_id)
```

### aabb.remove_gameobject(`go_id`)
Removes gameobject and it is AABB. You don't need to call `aabb.remove` again for removing AABB.

```lua
aabb.remove_gameobject(go_id)
```


### aabb.clear()

Clear everything(AABBs,groups, gameobjects) and reset it is initial state.

```lua
aabb.clear()
```

## Notes

- Performance is really depend on CPU load. I manage to get 60fps with 4800 sprite on my desktop. But this doesn't mean that you can use 4800 sprites on mobile platforms. You have to do your own test on your target platforms.  
