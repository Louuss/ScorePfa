
project(score_addon_dawimport LANGUAGES CXX)

# General initialization
score_common_setup()

# Source files
set(HDRS
    DawImport/ApplicationPlugin.hpp
    DawImport/ParseXml.hpp
    DawImport/Structs.hpp
    DawImport/CreateScore.hpp
    DawImport/gzipSupport.hpp
    score_addon_dawimport.hpp
)
set(SRCS
    DawImport/ApplicationPlugin.cpp
    DawImport/ParseXml.cpp
    DawImport/CreateScore.cpp
    DawImport/gzipSupport.cpp
    score_addon_dawimport.cpp
)


# Creation of the library
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

# Code generation
score_generate_command_list_file(${PROJECT_NAME} "${HDRS}")

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC score_plugin_engine score_plugin_midi score_plugin_media)
target_compile_definitions(${PROJECT_NAME} PRIVATE PLUGIN_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

# Target-specific options
setup_score_plugin(${PROJECT_NAME})

project(score_addons LANGUAGES CXX)

macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()


subdirlist(SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}")
set(SCORE_ADDON True)
foreach(subdir ${SUBDIRS})
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
    add_subdirectory("${subdir}")
  endif()
endforeach()
