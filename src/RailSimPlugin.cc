
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math4/ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>

namespace gazebo
{
    class RailSim : public ModelPlugin
    {
        // Plugin to control movement of a simple cart based on motion profiles
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
        {
            // Store pointers for the model
            this->model = _parent;

            // Listen for update event
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                std::bind(&RailSim::OnUpdate, this, std::placeholders::_1));
        }
        // Called by the world update start event
        public: void OnUpdate(const common::UpdateInfo &_info)
        {
            // Apply Linear Velocity to the model
            // Velocity = -aw * sin(wt)
            static double desired_cart_vel = 0.3000;
            gzdbg << "Simulation Time" << _info.simTime.Double() << std::endl; // Time in seconds
            //gzdbg << "Cart Speed " << desired_cart_vel << std::endl;
            this->model->SetLinearVel(ignition::math::Vector3d(desired_cart_vel, 0, 0));
        }


        // Private Members

        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer for the update even updateConnection
        private: event::ConnectionPtr updateConnection;

        // Sine Wave Parameters:
        private: const double amplitude = 15.0;
        private: const double max_velocity = 2.0; // ft/s
        private: const double frequency_w = 0.133;  // maxVel/Amplitude
    };
	// Register the plugin with the simulator
	GZ_REGISTER_MODEL_PLUGIN(RailSim)

}

