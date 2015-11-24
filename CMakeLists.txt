cmake_minimum_required(VERSION 2.8)

project(Fusion)

SET(OPIFEX_ENGINE_REPOSITORY CACHE STRING "../OPengine/")

# Sets up all of your application needs
# it then sets up all of the OPifex Engine links
include(CMake/application.cmake)




#########################################
# LAYER 9 - Application
#########################################

# Include Directories
include_directories(
	${PROJECT_SOURCE_DIR}
	.
	..
)

# Include directories for the engine itself
# Plus all of the external libraries needed
add_external_opifex_includes()


# Application Source Files
add_source_group(src "${PROJECT_SOURCE_DIR}/src" APPLICATION_SRC)


# Application BUILD
add_executable(9_Application ${APPLICATION_SRC})
add_opifex_definitions(9_Application "")
add_opifex_defines()
set_target_properties(9_Application PROPERTIES OUTPUT_NAME "Fusion")
add_opifex_libraries(9_Application)
add_opifex_assets(9_Application)


opifex_engine_status_messages()
