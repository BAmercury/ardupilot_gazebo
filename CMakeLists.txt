cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


project(ardupilot_sitl_gazebo)


#######################
## Find Dependencies ##
#######################

find_package(gazebo REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

if("${GAZEBO_VERSION}" VERSION_LESS "9.0")
    message(FATAL_ERROR "You need at least Gazebo 9.0. Your version: ${GAZEBO_VERSION}")
else()
    message("Gazebo version: ${GAZEBO_VERSION}")
endif()

###########
## Build ##
###########

include_directories(
        ${PROJECT_SOURCE_DIR}
        include
        ${GAZEBO_INCLUDE_DIRS}
        )

link_libraries(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${GAZEBO_LIBRARIES}
        )

link_directories(
        ${GAZEBO_LIBRARY_DIRS}
        )

set (plugins_single_header
        ArduPilotPlugin
        ArduCopterIRLockPlugin
        RailSimPlugin
	WindPlugin
        )
add_library(WindPlugin SHARED src/WindPlugin.cc)
target_link_libraries(WindPlugin ${GAZEBO_LIBRARIES})
install(TARGETS WindPlugin DESTINATION ${GAZEBO_PLUGIN_PATH})

add_library(ArduPilotPlugin SHARED src/ArduPilotPlugin.cc)
target_link_libraries(ArduPilotPlugin ${GAZEBO_LIBRARIES})


add_library(ArduCopterIRLockPlugin SHARED src/ArduCopterIRLockPlugin.cc)
target_link_libraries(ArduCopterIRLockPlugin ${GAZEBO_LIBRARIES})



install(TARGETS ArduPilotPlugin DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ArduCopterIRLockPlugin DESTINATION ${GAZEBO_PLUGIN_PATH})


# Rail Sim plugin
add_library(RailSimPlugin SHARED src/RailSimPlugin.cc)
target_link_libraries(RailSimPlugin ${GAZEBO_LIBRARIES})
install(TARGETS RailSimPlugin DESTINATION ${GAZEBO_PLUGIN_PATH})

