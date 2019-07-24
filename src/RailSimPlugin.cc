
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math4/ignition/math/Vector3.hh>

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
                std::bind(&RailSim::OnUpdate, this));
        }
        // Called by the world update start event
        public: void OnUpdate()
        {
            // Apply Linear Velocity to the model
            static double desired_cart_vel = 0.3000;
            gzdbg << "Cart Speed " << desired_cart_vel << std::endl;
            this->model->SetLinearVel(ignition::math::Vector3d(desired_cart_vel, 0, 0));
        }


        // Private Members

        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer for the update even updateConnection
        private: event::ConnectionPtr updateConnection;
    };
	// Register the plugin with the simulator
	GZ_REGISTER_MODEL_PLUGIN(RailSim)

}

