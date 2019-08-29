

#ifndef GAZEBO_PLUGINS_RAILSIMPLUGIN_HH_
#define GAZEBO_PLUGINS_RAILSIMPLUGIN_HH_

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
        // Default values for Velocity Sine Motion profile
        // Can be updated via SDF
        private: double amplitude = 8.0;
        private: double max_velocity = 1.524.0; // m/s
        private: double frequency_w = max_velocity / amplitude;  // maxVel/Amplitude
        // Variable to specify the motion profile for the rail sim
        private: int motion_type;

        // World Pointer
        private: physics::WorldPtr world_ptr;


    };
}

#endif