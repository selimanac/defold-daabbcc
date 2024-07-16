![DAABBCC](https://github.com/selimanac/DAABBCC/blob/v2.0/assets/header.png?raw=true)

This is a [Dynamic AABB Tree](https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf) native extension with Branch and Bound Algorithm for [Defold Engine](https://www.defold.com/).  
DAABBCC build by using Box2D’s [Dynamic Tree](https://github.com/erincatto/box2d/blob/master/src/collision/b2_dynamic_tree.cpp).

[Branch and Bound](https://github.com/erincatto/Box2D/pull/548) implementation by [Randy Gaul](https://twitter.com/RandyPGaul)   
[Radix sort](https://github.com/JCash/containers) by [Mathias Westerdahl](https://github.com/JCash)  
Box2D Copyright (c) 2009 Erin Catto http://www.box2d.org

**What is DAABBCC?**

A Dynamic AABB Tree is a binary search algorithm for fast overlap testing. Dynamic AABB trees are good for general purpose use, and can handle moving objects very well. The data structure provides an efficient way of detecting potential overlap between objects. DAABBCC does not contain narrow-phase collision detection.

You can use it whenever precise collision manifold(narrow-phase) is not required. 


## Installation

You can use the DAABBCC in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/).  
Open your game.project file and in the dependencies field under project add:

>https://github.com/selimanac/DAABBCC/archive/refs/tags/v2.1.2.zip


## Example

>https://github.com/selimanac/space-shooter-daabbcc


## Release Notes

2.1.3

- Small fix to prevent collision with Defold's build-in box2D lib.    

2.1.2

- Internal Update Frequency added. Default value is set from game.project file (display.frequency).  
It is now possible to set a new internal(independent from Defold game loop) update frequency by using `aabb.update_frequency()`.
- Fixed [#9](https://github.com/selimanac/DAABBCC/issues/9#issue-1533854235)

2.1.1

- Table count added to query results.
- Documentation update

2.1

- It is now possible to sort results by distance. Use `raycast_sort`, `query_id_sort`, `query_sort` according to your needs [#5](https://github.com/selimanac/DAABBCC/issues/5).
- Automated position updates for Defold Gameobjects [#6](https://github.com/selimanac/DAABBCC/issues/6).
- External Array and HashTable libs are removed. 
- Group limit removed (previously the number of groups was limited to 20)
- Remove Gameobject when AABB or Group removed 
- Clear function added for removing and resetting everything. 
- Stop/Resume for automated Gameobject position updates.
- All query results return nil when they are empty. No need to check `#count` anymore.



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


# API




## aabb.new_group()

New empty group for AABBs.

**RETURN**
* ```group_id``` (int) - New group ID.


**EXAMPLE**
```lua

   local group_id = aabb.new_group()

``` 

## aabb.insert(`group_id`, `x`, `y`, `width`, `height`)
Insert AABB into the group.  

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```x``` (number) - X position of AABB.
* ```y``` (number) - Y position of AABB.
* ```width``` (int) - Width of AABB.
* ```height``` (int) - Height of AABB.

**RETURN**
* ```aabb_id``` (int) - New AABB ID.

**EXAMPLE**
```lua
	
    local x = 0
    local y = 0
    local width = 50
    local height = 50
    local group_id = aabb.new_group()

    local aabb_id = aabb.insert(group_id, x, y, width, height)

    -- OR --

    self.pos = go.get_position(".")
    self.size = go.get("#sprite", "size")
    self.group_id = aabb.new_group()

    self.aabb_id = aabb.insert(self.group_id, self.pos.x , self.pos.y, self.size.x, self.size.y)

``` 

## aabb.insert_gameobject(`group_id`, `url`, `width`, `height`)
Insert Gameobject and the associated AABB into a group.  
Most suitable for moving gameobjects. If your gameobject is static then use `aabb.insert` instead.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```url``` (msg.url) - URL of Gameobject.
* ```width``` (int) - Width of AABB.
* ```height``` (int) - Height of AABB.

**RETURN**
* ```aabb_id``` (int) - New AABB ID.

**EXAMPLE**
```lua
	
    local go_url = msg.url("/go")
    local width = 50
    local height = 50
    local group_id = aabb.new_group()

    local aabb_id = aabb.insert_gameobject(group_id, go_url, width, height)
    
``` 


## aabb.update(`group_id`, `aabb_id`, `x`, `y`, `width`, `height`)
Updates the AABB position and size when you change its position or size.  
Gameobject AABB positions will be overwritten.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```aabb_id``` (int) - AABB ID.
* ```x``` (number) - X position of AABB.
* ```y``` (number) - Y position of AABB.
* ```width``` (int) - Width of AABB.
* ```height``` (int) - Height of AABB.

**EXAMPLE**
```lua

    local x = 0
    local y = 0
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
    local aabb_id = aabb.insert(group_id, x, y, width, height)
 
    local new_x = 10
    local new_y = 10
    local new_w = 60
    local new_h = 60

    aabb.update(group_id, aabb_id, new_x, new_y, new_w, new_h)
    
``` 

## aabb.update_gameobject(`group_id`, `aabb_id`, `w`, `h`)
Updates the AABB size related to the Gameobject.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```aabb_id``` (int) - AABB ID.
* ```width``` (int) - Width of AABB.
* ```height``` (int) - Height of AABB.

**EXAMPLE**
```lua

    local go_url = msg.url("/go")
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
    local aabb_id = aabb.insert_gameobject(group_id, go_url, width, height)
	
    local new_w = 60
    local new_h = 60

    aabb.update_gameobject(group_id, aabb_id,  new_w, new_h)
    
``` 

## aabb.query_id(`group_id`, `aabb_id`)
Query the possible overlaps using AABB ID.  

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```aabb_id``` (int) - AABB ID.

**RETURN**
* ```result``` (table) - Table of possible overlapping AABBs.
* ```count``` (int) - Count of `result` table.

**EXAMPLE**
```lua

    local go_url = msg.url("/go")
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
    local aabb_id = aabb.insert_gameobject(group_id, go_url, width, height)

    local result, count = aabb.query_id(group_id, aabb_id)

    if result then
        for i = 1, count do
            print(result[i])
        end
    end
    
``` 


## aabb.query(`group_id`, `x`, `y`, `width`, `height`)

Query the possible overlaps without AABB ID.   

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```x``` (number) - X position. 
* ```y``` (number) - Y position. 
* ```width``` (int) - Width. 
* ```height``` (int) - Height. 

**RETURN**
* ```result``` (table) - Table of possible overlapping AABBs.
* ```count``` (int) - Count of `result` table.

**EXAMPLE**
```lua

    local x = 0
    local y = 0
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
   

   local result, count = aabb.query(group_id, x, y, width, height)
    
    if result then
        for i = 1, count do
            print(result[i])
        end
    end
    
``` 

## aabb.query_id_sort(`group_id`, `aabb_id`)
Query the possible overlaps using AABB ID.  
Returns result table with ids and distance, ordered by closest first.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```aabb_id``` (int) - AABB ID.

**RETURN**
* ```result``` (table) - Table of possible overlapping AABBs. Result table contains `aabb_id` and  `distance`.
* ```count``` (int) - Count of `result` table.

**EXAMPLE**
```lua

    local go_url = msg.url("/go")
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
    local aabb_id = aabb.insert_gameobject(group_id, go_url, width, height)

    local result, count = aabb.query_id_sort(group_id, aabb_id)

    if result then
        for i = 1, count do
            print(result[i])
        end
    end
    
``` 

## aabb.query_sort(`group_id`, `x`, `y`, `width`, `height`)
Query the possible overlaps without AABB ID.  
Returns result table with ids and distance, ordered by closest first.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```x``` (number) - X position. 
* ```y``` (number) - Y position. 
* ```width``` (int) - Width. 
* ```height``` (int) - Height. 

**RETURN**
* ```result``` (table) - Table of possible overlapping AABBs. Result table contains `aabb_id` and  `distance`.
* ```count``` (int) - Count of `result` table.

**EXAMPLE**
```lua

    local x = 0
    local y = 0
    local width = 50
    local height = 50
    local group_id = aabb.new_group()

    local result, count = aabb.query_sort(group_id, x, y, width, height)
    
    if result then
        for i = 1, count do
            print(result[i])
        end
    end
    
    
``` 

## aabb.raycast(`group_id`, `start_x`, `start_y`, `end_x`, `end_y`)
Query the possible overlaps using RAYCAST.  

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```start_x``` (number) - Ray start position X.
* ```start_y``` (number) - Ray start position Y.
* ```end_x``` (number) - Ray end position X. 
* ```end_y``` (number) - Ray end position Y.   

**RETURN**
* ```result``` (table) - Table of possible overlapping AABBs. 
* ```count``` (int) - Count of `result` table.

**EXAMPLE**
```lua

    local group_id = aabb.new_group()

    local ray_start = vmath.vector3(0, 0, 0)
    local ray_end = vmath.vector3(365, 370, 0)

    local result, count = aabb.raycast(group_id, ray_start.x, ray_start.y, ray_end.x, ray_end.y)
    
    if result then
        for i = 1, count do
            print(result[i])
        end
    end
    
    
``` 

## aabb.raycast_sort(`group_id`, `start_x`, `start_y`, `end_x`, `end_y`)
Query the possible overlaps using RAYCAST.  
Returns result table with ids and distance, ordered by closest first.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```start_x``` (number) - Ray start position X.
* ```start_y``` (number) - Ray start position Y.
* ```end_x``` (number) - Ray end position X. 
* ```end_y``` (number) - Ray end position Y.   

**RETURN**
* ```result``` (table) - Table of possible overlapping AABBs. Result table contains `aabb_id` and  `distance`.
* ```count``` (int) - Count of `result` table.

**EXAMPLE**
```lua

    local group_id = aabb.new_group()

    local ray_start = vmath.vector3(0, 0, 0)
    local ray_end = vmath.vector3(365, 370, 0)

    local result, count = aabb.raycast_sort(group_id, ray_start.x, ray_start.y, ray_end.x, ray_end.y)
    
    if result then
        for i = 1, count do
            print(result[i])
        end
    end
    
    
``` 


## aabb.remove_group(`group_id`)

Removes the group and all associated AABBs and Gameobjects.

**PARAMETERS**
* ```group_id``` (int) - Group ID.


**EXAMPLE**
```lua

    local group_id = aabb.new_group()

    aabb.remove_group(group_id)

``` 


## aabb.remove(`group_id`, `aabb_id`)

Removes the AABB and Gameobject from group.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```aabb_id``` (int) - AABB ID.


**EXAMPLE**
```lua

    local go_url = msg.url("/go")
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
    local aabb_id = aabb.insert_gameobject(group_id, go_url, width, height)

    aabb.remove(group_id, aabb_id)

``` 


## aabb.remove_gameobject(`group_id`, `aabb_id`)

Removes gameobject and the associated AABB.  
You don't need to call `aabb.remove` again for removing AABB.

**PARAMETERS**
* ```group_id``` (int) - Group ID.
* ```aabb_id``` (int) - AABB ID.


**EXAMPLE**
```lua

    local go_url = msg.url("/go")
    local width = 50
    local height = 50
    local group_id = aabb.new_group()
    local aabb_id = aabb.insert_gameobject(group_id, go_url, width, height)

    aabb.remove_gameobject(group_id, aabb_id)

``` 

## aabb.run(`state`)

Stop/resume Gameobject position update iteration.
It is `true` by default, but does not iterate when when there are no Gameobjects registered.

**PARAMETERS**
* ```state``` (boolean) - True/False

**EXAMPLE**
```lua

    aabb.run(true)

``` 

## aabb.clear()

Clear everything (AABBs,groups, gameobjects) and reset to initial state.



**EXAMPLE**
```lua

    aabb.clear()

``` 


## aabb.update_frequency(`update_frequency`)

It is possible to set a independent update frequency for gameobject loop. Default value is set from game.project file(display.frequency).

**PARAMETERS**
* ```update_frequency``` (int) - Update Frequency.


**EXAMPLE**
```lua

    aabb.update_frequency(5)

``` 



---

## Notes

- Performance is heavily dependent on CPU load.
- For objects that do not move, it is advised to use aabb.insert() instead of aabb.insert_gameobject() to avoid unnecessary calculations.

---

## Games using DAABBCC



### Void Scrappers

![DAABBCC](/assets/void_scrappers.jpg?raw=true)

**Developer:** [8BitSkull](https://www.8bitskull.com/)  
**Publisher:** [8BitSkull](https://www.8bitskull.com/)   
**Release Date:** 21 Oct, 2022  
**Steam:** https://store.steampowered.com/app/2005210/Void_Scrappers/  
**Switch:** https://www.nintendo.co.uk/Games/Nintendo-Switch-download-software/Void-Scrappers-2344179.html. 

### Bore Blast

![DAABBCC](/assets/boreblast.jpg?raw=true)

**Developer:** [8BitSkull](https://www.8bitskull.com/)  
**Publisher:** [8BitSkull](https://www.8bitskull.com/)   
**Release Date:** 8 Mar, 2024  
**Steam:** https://store.steampowered.com/app/2398170/BORE_BLASTERS/  
**Switch:** TBA



