
#include "include/AnglePlugin.hh"
#include "gazebo/common/Assert.hh"
#include "gazebo/common/Events.hh"

using namespace gazebo;

GZ_REGISTER_WORLD_PLUGIN(AnglePlugin)

void AnglePlugin::Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
{
    this->world = _world;








    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        std::bind(&AnglePlugin::OnUpdate, this, std::placeholders::_1));
}

void AnglePlugin::OnUpdate(const common::UpdateInfo &_info)
{

}
