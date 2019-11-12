
#ifndef GAZEBO_PLUGINS_ANGLEPLUGIN_HH_
#define GAZEBO_PLUGINS_ANGLEPLUGIN_HH_

#include "gazebo/common/Event.hh"
#include "gazebo/common/Plugin.hh"
#include <ignition/math4/ignition/math/Vector3.hh>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "gazebo/common/Assert.hh"
#include "gazebo/common/Events.hh"
#include "gazebo/physics/physics.hh"
#include <math.h>
#include <iostream>
#include <fstream>
using raw_type = void;

namespace gazebo
{

    class GAZEBO_VISIBLE AnglePlugin : public WorldPlugin
    {
        public: virtual void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);

        public: void OnUpdate(const common::UpdateInfo &_info);

        public: struct DataPacket
        {
            uint64_t timestamp;
            uint16_t num_targets;
            float pos_x;
            float pos_y;
            float size_x;
            float size_y;
        };
        // Log data to csv file
        public: std::ofstream myFile;
        // UDP Client
        private: int sock_fd = 0;
        private: struct sockaddr_in servaddr;
        private: uint16_t server_port = 9005;

        private: physics::WorldPtr world;
        private: event::ConnectionPtr updateConnection;
        private: physics::ModelPtr model_drone;
        private: physics::ModelPtr model_target;
        private: std::string model_target_name;
        private: int update_rate = 10; // Hz
        private: double start_time;
        private: double current_time;
        private: double period = 1.0/update_rate;
        private: bool setup_bool = false; // Bool to declare timer varibales at first loop interation

    };

}

#endif