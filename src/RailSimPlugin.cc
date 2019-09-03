
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math/Vector3.hh>
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
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
        {
            // Store pointers for the model
            this->model = _parent;

            // Get World Pointer
            this->world_ptr = this->model->GetWorld();

            // Go through the SDF parameters and setup the motion profile
            if (_sdf->HasElement("motion_profile"))
            {
                sdf::ElementPtr motion_sdf = _sdf->GetElement("motion_profile");
                if (motion_sdf->HasElement("type"))
                {
                    this->amplitude = motion_sdf->Get<double>("amplitude");
                    this->max_velocity = motion_sdf->Get<double>("max_velocity");
                    this->frequency_w = this->max_velocity / this->amplitude;
                }
            }


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
	
            if (_info.simTime.Double() < 50) // if less than 50 seconds into the simulation
            {
                // Apply no velocity
                this->model->SetLinearVel(ignition::math::Vector3d(0,0,0));
            //gzdbg << "Waiting for Ardupilot to calibrate" << std::endl;
            }
            else
            {
                // Apply Linear Velocity to the model
                // Velocity = -aw * sin(wt)physics::WorldPtr _parent, sd
                double desired_vel = (-amplitude * frequency_w) * sin( (frequency_w) * _info.simTime.Double());

                    //gzdbg << "Cart Scaled Speed: " << desired_vel << std::endl;
                    this->model->SetLinearVel(ignition::math::Vector3d(0, desired_vel, 0));	    
            }


        }


        // Private Membersphysics::WorldPtr _parent, sd

        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer for the update even updateConnection
        private: event::ConnectionPtr updateConnection;


        // motion type Parameters

        // Sine Wave Parameters:
        private: double amplitude = 8.0;
        private: double max_velocity = 5.0; // ft/s
        private: double frequency_w = max_velocity/amplitude;  // maxVel/Amplitude


        // World Pointer
        private: physics::WorldPtr world_ptr;
    };
	// Register the plugin with the simulator
	GZ_REGISTER_MODEL_PLUGIN(RailSim)

}

