
![DAABBCC](/.github/header830.png?raw=true)

This is a Dynamic Tree(aka AABB Tree) native extension for [Defold Engine](https://www.defold.com/).    
DAABBCC build by using Box2D’s [Dynamic Tree](https://box2d.org/documentation/md_collision.html#autotoc_md46).  

**Credits**  
[Box2D](https://github.com/erincatto/box2d) by  [Erin Catto](https://x.com/erin_catto)  


**What is DAABBCC?**

A Dynamic AABB Tree is a binary search algorithm for fast overlap testing. Dynamic AABB trees are well-suited for general-purpose use and can handle moving objects efficiently. This data structure provides an effective method for detecting potential overlap between objects.  

DAABBCC is not a physics engine. It does not include narrow-phase collision detection or ~manifold generation~<sup>1</sup>.  

It is particularly well-suited for casual games, platformers, bullet-hell, top-down games, server-side headless builds that do not require narrow-phase collision detection.   

_<sup>1</sup> Added  v3.0.1_   

## Notes

### `daabbcc.remove()` is required

When using `daabbcc.insert_gameobject()`, you **must** call `daabbcc.remove()` before or when the game object is deleted — either before `go.delete()` or inside the game object's `final()` callback. Failing to do so leaves a stale reference in the update loop, which can cause a crash on the next frame.

### `game.project` settings

| Key | Default | Description |
|-----|---------|-------------|
| `daabbcc.max_group_count` | `3` | Maximum number of tree groups |
| `daabbcc.max_gameobject_count` | `128` | Maximum tracked game objects |
| `daabbcc.max_query_result_count` | `32` | Maximum query results |
| `daabbcc.validate_gameobjects` | `0` | Set to `1` to enable per-frame validity checks on tracked game objects. When enabled, the extension automatically removes and logs an error for any game object that was deleted without calling `daabbcc.remove()`. Has a small per-frame cost proportional to the number of tracked objects. |

---

## Discussions & Release Notes

https://github.com/selimanac/defold-daabbcc/discussions

## Documentation

https://github.com/selimanac/defold-daabbcc/wiki 

---

## Toss a Coin to Your Witcher
If you find my [Defold Extensions](https://github.com/selimanac) useful for your projects, please consider [supporting](https://github.com/sponsors/selimanac) it.  
I'd love to hear about your projects! Please share your released projects that use my native extensions. It would be very motivating for me.


---

# Games using DAABBCC

### Skull Horde

![Skull Horde](/.github/skull_horde.png?raw=true)

**Developer:** [8BitSkull](https://www.8bitskull.com/)   
**Publisher:** [8BitSkull](https://www.8bitskull.com/)   
**Release Date:** TBA  
**Steam:** https://store.steampowered.com/app/3199360/Skull_Horde/  



## Bore Blasters

![Bore Blasters](/.github/boreblast.jpg?raw=true)

**Developer:** [8BitSkull](https://www.8bitskull.com/)  
**Publisher:** [8BitSkull](https://www.8bitskull.com/)   
**Release Date:** 8 Mar, 2024  
**Steam:** https://store.steampowered.com/app/2398170/BORE_BLASTERS/  



## Void Scrappers

![Void Scrappers](/.github/void_scrappers.jpg?raw=true)

**Developer:** [8BitSkull](https://www.8bitskull.com/)  
**Publisher:** [8BitSkull](https://www.8bitskull.com/)   
**Release Date:** 21 Oct, 2022  
**Steam:** https://store.steampowered.com/app/2005210/Void_Scrappers/  
**Switch:** https://www.nintendo.co.uk/Games/Nintendo-Switch-download-software/Void-Scrappers-2344179.html. 

