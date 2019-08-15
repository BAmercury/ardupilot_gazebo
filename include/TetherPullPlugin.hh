


#ifndef _GAZEBO_TETHERPULL_PLUGIN_HH_
#define _GAZEBO_TETHERPULL_PLUGIN_HH_

#include "gazebo/common/Event.hh"
#include "gazebo/common/Plugin.hh"
#include <gazebo/common/common.hh>
#include "gazebo/physics/physics.hh"
#include <ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>

namespace gazebo
{


    class GAZEBO_VISIBLE TetherPullPlugin : public ModelPlugin
    {
        public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);


        private: void OnUpdate(const common::UpdateInfo &_info);

        private: void ApplyPullForce(const double *setpoint);

        private: bool InitSignalSocket(sdf::ElementPtr _sdf) const;
        private: void RecvData();


        private: physics::WorldPtr world_ptr;
        private: physics::ModelPtr model;
        private: event::ConnectionPtr updateConnection;
        private: sensors::ContactSensorPtr sensor;
        private: const std::string sensor_name = "contact_sensor";
        private: const std::string drone_coll_name = "iris::iris_demo::iris::base_link::base_link_collision";
        private: bool contacted = false;
        private: bool enable_control = false;
        private: volatile double setpoint = 0; // M/s
        private: volatile double* setpoint_ptr = &setpoint; // pointer to the setpoitn variable



        // TCP Client
        private: int sock = 0;
        private: int valread;
        struct sockaddr_in server_addr;
        private: char buffer[1024] = {0};
        private: const char *server_ip = "127.0.0.1";
        private: uint16_t server_port = 8080; 
 

    };

}


#endif