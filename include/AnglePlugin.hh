
#ifndef GAZEBO_PLUGINS_ANGLEPLUGIN_HH_
#define GAZEBO_PLUGINS_ANGLEPLUGIN_HH_

#include "gazebo/common/Event.hh"
#include "gazebo/common/Plugin.hh"
#include <ignition/math4/ignition/math/Vector3.hh>


namespace gazebo
{

    class GAZEBO_VISIBLE AnglePlugin : public WorldPlugin
    {
        public: virtual void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);

        public: void OnUpdate();


        private: physics::WorldPtr world;
        private: event::ConnectionPtr updateConnection;
        private: physics::ModelPtr model_drone;
        private: physics::ModelPtr model_target;
    };

}

#endif