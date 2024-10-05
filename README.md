
![DAABBCC](/.github/header830.png?raw=true)

This is a [Dynamic AABB Tree](https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf) native extension for [Defold Engine](https://www.defold.com/).    
DAABBCC build by using Box2D’s [Dynamic Tree](https://github.com/erincatto/box2d/blob/main/src/dynamic_tree.c).  

[Radix sort](https://github.com/JCash/containers) by [Mathias Westerdahl](https://x.com/mwesterdahl76)  
[Box2D](https://github.com/erincatto/box2d) Copyright (c)  [Erin Catto](https://x.com/erin_catto) http://www.box2d.org  


**What is DAABBCC?**

A Dynamic AABB Tree is a binary search algorithm for fast overlap testing. Dynamic AABB trees are good for general purpose use, and can handle moving objects very well. The data structure provides an efficient way of detecting potential overlap between objects.   
DAABBCC is not a physics engine. DAABBCC does not contain narrow-phase collision detection and manifold generation.

It is very well suited for platformers, bullet-hell, top down games which doesnt require narrow-phase collision detection.


## Installation

You can use the A* extension in your own project by adding this project as a [Defold library dependency](https://defold.com/manuals/libraries/#setting-up-library-dependencies).  
Open your `game.project` file, select  `Project ` and add a  `Dependencies ` field:

>https://github.com/selimanac/defold-astar/archive/master.zip <- TODO 3.0

or you can add stable versions from [releases](https://github.com/selimanac/defold-daabbcc/releases).  

## Examples

Flappy Bird Clone: https://github.com/selimanac/DAABBCC-Flappybird-Example  
Breakout: https://github.com/selimanac/DAABBCC-Breakout-Example  

## Forum

https://forum.defold.com/t/daabbcc-dynamic-aabb-tree-native-extension-with-branch-and-bound-algorithm

## Toss a Coin to Your Witcher
If you find my [Defold Extensions](https://github.com/selimanac) useful for your projects, please consider [supporting](https://github.com/sponsors/selimanac) it.  
I'd love to hear about your projects! Please share your released projects that use my native extensions. It would be very motivating for me.



## Release Notes

https://github.com/selimanac/defold-daabbcc/discussions/categories/announcements