#ifndef GAZEBO_PLUGINS_RAILSIMPLUGIN_HH_
#define GAZEBO_PLUGINS_RAILSIMPLUGIN_HH_

#include <sdf/sdf.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math/Vector3.hh>
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
        private: double max_velocity = 1.524; // m/s
        private: double frequency_w = max_velocity / amplitude;  // maxVel/Amplitude
        private: int direction = 2; // In Gazebo Coordinates, 1:x, 2:y, 3:z
        private: int motion_type;  // Variable to specify the motion profile for the rail sim
        private: double delay_time = 50; // In Seconds, Variable to specify the amount of time for Gazebo to wait on Ardupilot SITL
        private: int index = 0; // Used to iterate through the static motion profile header
        private: int size = 0; // Used to iterate through the static motion profile header
        private: bool back_bool = false; // To tell system to travel backwards

        // Motion Profile 2/3 variables
        private: double desired_position = 0;
        private: ignition::math::Vector3d vel;
        private: ignition::math::Pose3d pos;
        private: double vely;
        private: double posy;
        private: double force;

        // Motion Profile 2 and 4 Update Timer
        private: double start_time;
        private: double current_time;
        private: const double period = 0.05; // In Seconds
        private: bool setup_bool = false; // Used to configure reading from static motion profile header files

        // Motion Profile 3 State Feedback Control
        // Calculated using model.m found in the ardupilot_extra folder
        private: double feed_forward_gain = 250.0;
        private: double gain_k1 = 250.0; // poles at -5, -1
        private: double gain_k2 = 299.0;


        // Motion Profile 4 Step input Parameters
        private: double target1_pos = 0.0;
        private: double target1_hold = 0.0; // Seconds
        private: bool hold_control = false;
        private: ignition::math::Pose3d origin_pose;


        // World Pointer
        private: physics::WorldPtr world_ptr;

        private: physics::LinkPtr link;


    };
}

#endif
