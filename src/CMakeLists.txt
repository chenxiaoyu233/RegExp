# NDEBUG
if (NDEBUG)
  set (CMAKE_CXX_FLAGS "-O2 -DNDEBUG ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
  set (CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)


aux_source_directory (. RegExpSrc)
add_library (RegExpLib ${RegExpSrc})
