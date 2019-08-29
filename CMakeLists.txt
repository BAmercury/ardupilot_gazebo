cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


project(ardupilot_sitl_gazebo)


#######################
## Find Dependencies ##
#######################

# Set up Custom Boost Path
execute_process(COMMAND lsb_release -sr OUTPUT_VARIABLE DISTRO_VER)
message("${DISTRO_VER}")
if ("${DISTRO_VER}" VERSION_EQUAL "16.04")
        message(STATUS "Ubuntu xenial, using custom update to date BOOST libraries")
        set(BOOSTROOT /home/james/boost/)
        set(BOOST_INCLUDEDIR /home/james/boost/boost_1_69_0_build/include/)
        set(BOOST_LIBRARYDIR /home/james/boost/boost_1_69_0_build/lib/)
        set(BOOST_NO_SYSTEM_PATHS ON)
elseif("$DISTRO_VER}" VERSION_EQUAL "18.04")
        message(STATUS "Ubuntu biotic, using system installation path for BOOST libraries")
endif()







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
        Profile
        #TetherPullPlugin
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


# Tether Pull Plugin
#add_library(TetherPullPlugin SHARED src/TetherPullPlugin.cc)
#target_link_libraries(TetherPullPlugin ${GAZEBO_LIBRARIES})
#install(TARGETS TetherPullPlugin DESTINATION ${GAZEBO_PLUGIN_PATH})

