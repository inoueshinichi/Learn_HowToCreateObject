cmake_minimum_required(VERSION 3.14.6)

# Include cmake script
include(../plugin_configure.cmake)

# Dll plugin config
set(MAJOR_VER 0)
set(MINOR_VER 1)
set(PATCH_VER 0)
is_macro_plugin_configure(mac_plugin ${MAJOR_VER} ${MINOR_VER} ${PATCH_VER})