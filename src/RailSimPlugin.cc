
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
        /*
            Loads model and optionally sdf of the model
            Also adds subscriber for World update Events
        */
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
        {
            // Store pointers for the model
            this->model = _parent;

            /*
                worldUpdateBegin event is fired at each physics iteration
                
                ConnectWorldBegin takes in a callback for the triggered event
            */
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                std::bind(&RailSim::OnUpdate, this, std::placeholders::_1));
        }
        // Called by the world update start event
        public: void OnUpdate(const common::UpdateInfo &_info)
        {
            // Apply Linear Velocity to the model
            // Velocity = -aw * sin(wt)
            double desired_vel = (-amplitude * frequency_w) * sin( (frequency_w) * _info.simTime.Double());
            gzdbg << "Cart Speed: " << desired_vel << std::endl; // Time in seconds
            //gzdbg << "Cart Speed " << desired_cart_vel << std::endl;

            // Should oscillate between 0.6096:-0.6096 m/s ? 
            desired_vel = desired_vel * 0.2;
            gzdbg << "Cart Scaled Speed: " << desired_vel << std::endl;
            this->model->SetLinearVel(ignition::math::Vector3d(desired_vel, 0, 0));
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

