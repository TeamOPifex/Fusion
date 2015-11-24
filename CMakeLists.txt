cmake_minimum_required(VERSION 2.8)

project(Fusion)

SET(OPIFEX_ENGINE_REPOSITORY CACHE STRING "../OPengine/")

# Sets up all of your application needs
# it then sets up all of the OPifex Engine links
include(CMake/application.cmake)



#########################################
# Common
#########################################

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/Common/inc
	.
	..
)
# Include directories for the engine itself
# Plus all of the external libraries needed
add_external_opifex_includes()


add_source_group(inc "${PROJECT_SOURCE_DIR}/Common/inc" COMMON_SRC)
add_source_group(src "${PROJECT_SOURCE_DIR}/Common/src" COMMON_SRC)


# Application BUILD
add_library(Common ${COMMON_SRC})
add_opifex_definitions(Common "")
add_opifex_defines()
set_target_properties(Common PROPERTIES OUTPUT_NAME "Common")
add_opifex_libraries(Common)





#########################################
# Server
#########################################

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/Server/inc
	.
	..
)
# Include directories for the engine itself
# Plus all of the external libraries needed
add_external_opifex_includes()


add_source_group(inc "${PROJECT_SOURCE_DIR}/Server/inc" SERVER_SRC)
add_source_group(src "${PROJECT_SOURCE_DIR}/Server/src" SERVER_SRC)


# Application BUILD
add_library(Server ${SERVER_SRC})
target_link_libraries(Server Common)
add_opifex_definitions(Server "")
add_opifex_defines()
set_target_properties(Server PROPERTIES OUTPUT_NAME "Server")
add_opifex_libraries(Server)



#########################################
# LAYER 9 - Application
#########################################

# Include Directories
include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/Game/inc
	.
	..
)

# Include directories for the engine itself
# Plus all of the external libraries needed
add_external_opifex_includes()


# Application Source Files
add_source_group(inc "${PROJECT_SOURCE_DIR}/Game/inc" APPLICATION_SRC)
add_source_group(src "${PROJECT_SOURCE_DIR}/Game/src" APPLICATION_SRC)


# Application BUILD
add_executable(9_Application ${APPLICATION_SRC})
add_opifex_definitions(9_Application "")
add_opifex_defines()
target_link_libraries(9_Application Common)
set_target_properties(9_Application PROPERTIES OUTPUT_NAME "Fusion")
add_opifex_libraries(9_Application)
add_opifex_assets(9_Application)


opifex_engine_status_messages()
