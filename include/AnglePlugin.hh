
#ifndef GAZEBO_PLUGINS_ANGLEPLUGIN_HH_
#define GAZEBO_PLUGINS_ANGLEPLUGIN_HH_

#include "gazebo/common/Event.hh"
#include "gazebo/common/Plugin.hh"
#include <ignition/math4/ignition/math/Vector3.hh>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

        public: std::string irlock_addr = "127.0.0.1";
        public: uint16_t irlock_port = 9005;



        private: physics::WorldPtr world;
        private: event::ConnectionPtr updateConnection;
        private: physics::ModelPtr model_drone;
        private: physics::ModelPtr model_target;

    };

}

#endif