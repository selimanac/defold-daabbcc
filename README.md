# DAABBCC
AABBs tree native extension for Defold Engine

## About
DAABBCC is a C++ wrapper of [AABB.cc](https://github.com/lohedges/aabbcc) lib for Defold Engine. [AABB.cc](https://github.com/lohedges/aabbcc) is developed by [Lester Hedges](http://lesterhedges.net) and released under the [Zlib](http://zlib.net/zlib_license.html) license. The code in [AABB.cc](https://github.com/lohedges/aabbcc) library was adapted from parts of the [Box2D](http://www.box2d.org) physics engine. DAABBCC is a native wrapper developed by [Selim Anaç](https://twitter.com/selimanac)

DAABBCC is a C++ implementation of a dynamic bounding volume hierarchy ([BVH](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)) using axis-aligned bounding boxes ([AABBs](https://en.wikipedia.org/wiki/Minimum_bounding_box)).
The data structure provides an efficient way of detecting potential overlap between objects of arbitrary shape and size and is commonly used in computer games engines for collision detection and ray tracing.

## Installation
You can use DAABBCC in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/). Open your game.project file and in the dependencies field under project add:

	https://github.com/selimanac/DAABBCC/archive/master.zip
  
## Usage in Defold Engine

You can use this native extension directly.

### Creating a new AABB Tree

You should create a new tree(s)

#### - createTree
```lua
local tree_name = "particles" -- Name of your tree
local dimension = 2 -- 2D. Original library works with 2D and 3D. Only 2D is implemented   
local skin_thickness = 0.1 -- Thickness of bounding boxes.
local n_particles = 4 -- Number of bounding boxes

daabbcc.createTree(tree_name, dimension, skin_thickness, n_particles)
```

You can create as much as trees according to your needs.
Pseudo example for a platformer:

```lua
daabbcc.createTree("walls", 2, 0.1, 4)
daabbcc.createTree("enemies", 2, 0.1, 50)
daabbcc.createTree("collectables", 2, 0.1, 150)
```

### Inserting AABBs

Two ways of inserting AABBs into the tree. 

#### - insertCircle

Insert a AABB into the name given tree. Works with radius. This is **not** going to create a circular shape. It creates a square from given radius.

insertCircle, returns the ID of the added AABB. You should keep track of those IDs in a table.
**Caution**: IDs starts with '0'

```lua
local table_name = "particles" -- Name of your tree
local radius = 10 -- radius of the circle. It is basically:  width/2
local position = vmath.vector3(x,y,z) -- Position of your game object

local _id = daabbcc.insertCircle(table_name, radius, position.x , position.y)
```

#### - insertRect

Insert a AABB into the name given tree.

insertRect, returns the ID of the added AABB. You should keep track of those IDs in a table.
**Caution**: IDs starts with '0'

```lua
local table_name = "particles" -- Name of your tree
local position = vmath.vector3(x,y,z) -- Position of your game object
local size = vmath.vector3(x,y,z) -- Size of your game object/Sprite

local _id = daabbcc.insertRect(table_name, position.x , position.y, size.x , size.y)
```

## Performance and Notes

## Examples

## Building [AABB.cc](https://github.com/lohedges/aabbcc) lib

https://github.com/selimanac/aabbcc/tree/defoldWrapper

```bash
make release 
make build
make PREFIX=MY_INSTALL_DIR install
```

## [AABB.cc](https://github.com/lohedges/aabbcc) Docs

Doxygen Docs: https://codedocs.xyz/selimanac/aabbcc/
