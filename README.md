# DAABBCC
DAABBCC is a terrible name of the [AABB Tree](https://github.com/selimanac/DAABBCC#creating-a-new-aabb-tree) + [Experimental Collision Detection](https://github.com/selimanac/DAABBCC#experimental-narrow-phase-collision-detection) native extension for [Defold Engine](https://www.defold.com/)

## About
DAABBCC is a C++ wrapper of [AABB.cc](https://github.com/lohedges/aabbcc) and [tinyc2](https://github.com/RandyGaul/tinyheaders/blob/master/tinyc2.h) libs for  [Defold Engine](https://www.defold.com/). 

[AABB.cc](https://github.com/lohedges/aabbcc) is developed by [Lester Hedges](http://lesterhedges.net) and released under the [Zlib](http://zlib.net/zlib_license.html) license. The code in [AABB.cc](https://github.com/lohedges/aabbcc) library was adapted from parts of the [Box2D](http://www.box2d.org) physics engine. 

[tinyc2](https://github.com/RandyGaul/tinyheaders/blob/master/tinyc2.h)  is developed by [Randy Gaul](https://twitter.com/RandyPGaul) and released under the [Zlib](http://zlib.net/zlib_license.html) license.

DAABBCC is a native wrapper developed by [Selim Anaç](https://twitter.com/selimanac) and released under the [Zlib](http://zlib.net/zlib_license.html) license.

DAABBCC is a C++ implementation of a dynamic bounding volume hierarchy ([BVH](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)) using axis-aligned bounding boxes ([AABBs](https://en.wikipedia.org/wiki/Minimum_bounding_box)).
The data structure provides an efficient way of detecting potential overlap between objects of arbitrary shape and size and is commonly used in computer games engines for collision detection and ray tracing.

## Installation
You can use DAABBCC in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/). Open your game.project file and in the dependencies field under project add:

	https://github.com/selimanac/DAABBCC/archive/master.zip
	
## Platforms

Supported platforms: Web, Win 64, MacOS 64, Linux 64. 
  
## What is this for?

First of all, **this is not a complete collision library/solution**. Basically it is a Broad Phase collision for fast overlap checking. You have to handle precise collision yoursef. This library contains experimental narrow phase collision detection functionalty.
You can use this lib for culling offscreen objects, raycasting through the tree or for puzzle game logics... 

## Usage in Defold

You can use this native extension by simply calling 'daabbcc'. 

Default "**World**" tree will be created on init with 0.1 tickness and 100 count.

**Caution**: Scaling may break your collision. You should recalculate the size manually.

### Creating a new AABB Tree

#### - createTree
```lua
local tree_name = "World" -- Name of your tree
local dimension = 2 -- 2D. Original library works with 2D and 3D. Only 2D is implemented   
local thickness = 0.1 -- Thickness of bounding boxes.
local number = 4 -- Number of aabbs/bounding boxes. Optional but recommended.  

daabbcc.createTree(tree_name, dimension, thickness, [number])
```
You can create as many as trees according to your needs.
Pseudo example for a platformer:

```lua
daabbcc.createTree("walls", 2, 0.1, 4)
daabbcc.createTree("enemies", 2, 0.1, 50)
daabbcc.createTree("collectables", 2, 0.1, 150)
```

### Inserting AABBs

#### - insertCircle

Insert a AABB into the created tree. Works with radius. This method is **not** going to create a circular shape. It creates a square AABB from given radius. (radius = 10 mean 20x20 square)

insertCircle, returns the ID of the inserted AABB. You should probably keep track of those IDs in a lua table.

**Caution**: IDs starts with '0'

```lua
local tree_name = "World" -- Name of your tree
local radius = 10 -- radius of the circle. It is basically:  width/2 
local position = vmath.vector3(x,y,z) -- Position of your game object / go.get_position()

local _id = daabbcc.insertCircle(tree_name, radius, position.x, position.y)
```

#### - insertRect

Same as insertCircle but using size.

insertRect, returns the ID of the inserted AABB. You should probably keep track of those IDs in a lua table.

**Caution**: IDs starts with '0'

```lua
local tree_name = "World" -- Name of your tree
local position = vmath.vector3(x,y,z) -- Position of your game object / go.get_position()
local size = vmath.vector3(x,y,z) -- Size of your game object or sprite / go.get("#sprite", "size")

local _id = daabbcc.insertRect(tree_name, position.x, position.y, size.x, size.y)
```
### Updating AABBs

If your game object(s) are not static, you should update their position and size when they move or resize.

#### - updateCircle

```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object
local radius = 10 -- radius of the circle. It is basically:  width/2
local position = vmath.vector3(x,y,z) -- Position of your game object / go.get_position()

daabbcc.updateCircle(tree_name, id, radius, position.x, position.y)
```

#### - updateRect
```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object
local position = vmath.vector3(x,y,z) -- Position of your game object / go.get_position()
local size = vmath.vector3(x,y,z) -- Size of your game object or sprite / go.get("#sprite", "size")

daabbcc.updateRect(tree_name, id, position.x, position.y, size.x, size.y)
```
### Removing AABBs from Tree

#### - removeAABB
```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object

daabbcc.removeAABB(tree_name,id)
```
### Queries
You can query a tree by id or with AABB for possible overlaps. 
Query returns a lua table of object IDs.

**Caution**: You can't cross query between different trees. 

#### - queryID
```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object

local _result = daabbcc.queryID(tree_name, id)
```

#### - queryAABB
```lua
local tree_name = "World" -- Name of your tree
local position = go.get_position() -- Position of your object
local size = go.get("#sprite", "size")

local _result = daabbcc.queryAABB(tree_name,position.x,position.y,size.x,size.y)
```

### Helpers

#### - validateTree
Validate the tree. (Not documented)

```lua
local tree_name = "World" -- Name of your tree

daabbcc.validateTree(tree_name)
```
#### - rebuildTree
Rebuild an optimal tree. (Not documented)

```lua
local tree_name = "World" -- Name of your tree

daabbcc.rebuildTree(tree_name)
```
#### - getHeight
Get the height of the tree.

```lua
local tree_name = "World" -- Name of your tree

daabbcc.getHeight(tree_name)
```
#### - getNodeCount
Get the number of nodes in the tree.

```lua
local tree_name = "World" -- Name of your tree

daabbcc.getNodeCount(tree_name)
```
#### - getAABB
Returns postion and size of bounding box.

```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object

local x,y,w,h = daabbcc.getAABB(tree_name, id)
```

## Experimental Narrow Phase Collision Detection

#### - [Swept AABB Collision Detection](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/)

Collision detection for a moving objects against static objects. Returns impact time and collision normal.

```lua
local tree_name = "World" -- Name of your tree
local moving_bb_id = 0 -- ID of your moving object
local static_bb_id = 1 -- ID of your static object
local target_velocity = vmath.vector3(25, 25, 0) -- target velocity of your moving object
local normal = vmath.vector3(1, 0, 0) -- Direction of your moving object

local collisionTime, c_normal_x,  c_normal_y = daabbcc.checkSweptCollision(tree_name, moving_bb_id, static_bb_id, target_velocity.x,target_velocity.y, normal.x, normal.y)
```

### - [tinyc2](https://github.com/RandyGaul/tinyheaders/blob/master/tinyc2.h) lib

#### - checkHit
Simple aabb to aabb collision check. Returns integer.

```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object
local other_id = 1 -- ID of your other object

local hit =   daabbcc.checkHit(tree_name,id,  other_id)

```

#### - checkManifold
Manifold generation for colliding objects. Returns count, depth, normal and contact points.

```lua
local tree_name = "World" -- Name of your tree
local id = 0 -- ID of your object
local other_id = 1 -- ID of your other object

local count, depth, normal, contact_points = daabbcc.checkManifold(tree_name,id, other_id)

```

#### - createRay
Creates a ray and returns it is ID. You should probably keep track of those IDs in a lua table.

```lua
local position = vmath.vector3(0, 0, 0) -- ray start position
local direction = vmath.vector3(1, 0, 0) -- ray direction
local distance = 20 -- ray distance

local ray_id = daabbcc.createRay(position.x,position.y, direction.x, direction.y, distance)

```

#### - updateRay
Updates current Ray.

```lua
local id = 0 -- ID of your ray
local position = vmath.vector3(0, 0, 0) -- ray start position
local direction = vmath.vector3(1, 0, 0) -- ray direction
local distance = 20 -- ray distance

daabbcc.updateRay(id, position.x,position.y, direction.x, direction.y, distance)
```

#### - removeRay
Removes a Ray.

```lua
local id = 0 -- ID of your ray

daabbcc.removeRay(ray_id)
```

#### - rayCastToAABB
Cast a ray to AABB. Returns hit, impact and end point(-1).

```lua
local ray_id = 0 -- ID of your ray
local other_id = 1 -- ID of you object
local tree_name = "World" -- Name of your tree

local hit, impact, end_point = daabbcc.rayCastToAABB(tree_name, id, other_id)
```

# Performance and Notes

If you know what you are doing, lib is very performant. But that doesn't mean it is a competitor to build in physic. It is not. First of all; you should handle collisions yourself. Which means you need loops and logics. Maybe lots of them. This approach is very CPU intensive and results may differ according to CPU performance. Also, resolution may affect the performance.

I made some stress tests just for fun. Those number are not acceptable in real game. Maybe(just maybe) you can use less than half of the numbers. Dt is fixed to 60, not variable. Also I drop down the item numbers and their speed. Because of the screen video recording cause a memory leap. I believe they can be more optimised.

- [Need more performance testing ](https://github.com/selimanac/DAABBCC/issues/2)

My test platform is:
- macOS 10.13.2
- 2GHz Intel Core i7
- 8GB 1600 MHz DDR3
- Intel HD Graphics 4000


### Youtube videos: 

[600 sprites in the box](https://www.youtube.com/watch?v=vUKKxa1Sso8)

[Crazy 700 sprites goes into the black hole](https://www.youtube.com/watch?v=1APPkiQ3Hag)

[![600 sprites in the box with Defold Engine](http://img.youtube.com/vi/vUKKxa1Sso8/0.jpg)](http://www.youtube.com/watch?v=vUKKxa1Sso8)


[![Crazy 700 sprites goes into the black hole with Defold Engine](http://img.youtube.com/vi/1APPkiQ3Hag/0.jpg)](https://www.youtube.com/watch?v=1APPkiQ3Hag)

## Examples

Since I am a lazy developer, you can find my terrible test files in the source. Also I am working on a simple platformer for testing this library: https://github.com/selimanac/DAABBCC-Platformer


## [AABB.cc](https://github.com/lohedges/aabbcc) Docs

Doxygen Docs: https://codedocs.xyz/selimanac/aabbcc/
