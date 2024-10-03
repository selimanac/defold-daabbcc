set(DAABBCC_SOURCE_FILES
	aabb.cpp
	allocate.cpp
	core.cpp
	dynamic_tree.cpp
	math_functions.cpp
	daabbcc.cpp
)
add_library(daabbcc ${DAABBCC_SOURCE_FILES}  )

target_include_directories(daabbcc
	PRIVATE  "/Users/selimanac/Development/c-addons/defoldsdk/sdk/include" 
	)