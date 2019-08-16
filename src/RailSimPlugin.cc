
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
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
        {
            // Store pointers for the model
            this->model = _parent;

            // Create a sensor
            sensors::SensorManager *manager = sensors::SensorManager::Instance();
            this->sensor =  std::dynamic_pointer_cast<sensors::ContactSensor>(manager->GetSensor(this->sensor_name));
            this->sensor->SetActive(true);

            // Get World Pointer
            this->world_ptr = this->model->GetWorld();


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
		gzdbg << "Waiting for Ardupilot to calibrate" << std::endl;
	    }
	    else
	    {
	    	// Apply Linear Velocity to the model
	        // Velocity = -aw * sin(wt)physics::WorldPtr _parent, sd
	        double desired_vel = (-amplitude * frequency_w) * sin( (frequency_w) * _info.simTime.Double());

                // Should oscillate between 0.6096:-0.6096 m/s ? 
                desired_vel = desired_vel * 0.2;
                //gzdbg << "Cart Scaled Speed: " << desired_vel << std::endl;
                this->model->SetLinearVel(ignition::math::Vector3d(0, desired_vel, 0));	    
	    }

            // Check if drone has landed on the platform
            //msgs::Contacts contacts;
            //contacts = this->sensor->Contacts();
            //for (unsigned int i = 0; i < contacts.contact_size(); ++i)
            //{
                // If contact is made between platform and drone, pause the simulation
                //if (contacts.contact(i).collision1() == this->drone_coll_name)
                //{
                //    gzdbg << "Collision has been detected"<< std::endl;
                    //this->model->SetLinearVel(ignition::math::Vector3d(0,0,0));
                    //this->world_ptr->SetPaused(true);
                    //contacted = true;
                    //break;

                //}
                //else if (contacted == false)
                //{
                    
                //}


            }
        }


        // Private Membersphysics::WorldPtr _parent, sd

        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer for the update even updateConnection
        private: event::ConnectionPtr updateConnection;

        // Sine Wave Parameters:
        private: const double amplitude = 8.0;
        private: const double max_velocity = 5.0; // ft/s
        private: const double frequency_w = max_velocity/amplitude;  // maxVel/Amplitude

        // Contact Sensor:
        private: sensors::ContactSensorPtr sensor;
        private: const std::string sensor_name = "contact_sensor";
        private: const std::string drone_coll_name = "iris::iris_demo::iris::base_link::base_link_collision";
        private: bool contacted = false;

        // World Pointer
        private: physics::WorldPtr world_ptr;
    };
	// Register the plugin with the simulator
	GZ_REGISTER_MODEL_PLUGIN(RailSim)

}

