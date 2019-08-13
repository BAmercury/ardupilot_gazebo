

#ifndef GAZEBO_PLUGINS_RAILSIMPLUGIN_HH_
#define GAZEBO_PLGUINS_RAILSIMPLUGIN_HH_

#include <sdf/sdf.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math4/ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>

namespace gazebo
{
    class GAZEBO_VISIBLE RailSim : public ModelPlugin
    {
        public: virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
        public: void OnUpdate(const common::UpdateInfo &_info);

        // Private Membersphysics::WorldPtr _parent, sd

        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer for the update even updateConnection
        private: event::ConnectionPtr updateConnection;

        // Sine Wave Parameters:
        private: const double amplitude = 15.0;
        private: const double max_velocity = 2.0; // ft/s
        private: const double frequency_w = 0.133;  // maxVel/Amplitude

        // Contact Sensor:
        private: sensors::ContactSensorPtr sensor;
        private: const std::string sensor_name = "contact_sensor";
        private: const std::string drone_coll_name = "iris::iris_demo::iris::base_link::base_link_collision";
        private: bool contacted = false;

        // World Pointer
        private: physics::WorldPtr world_ptr;

        // Variable to specify the motion profile for the rail sim
        private: std::string desired_motion;
    };
}

#endif