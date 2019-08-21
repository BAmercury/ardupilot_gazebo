

#include <sstream>
#include <gazebo/msgs/msgs.hh>
#include "GUI_ResetWorld.hh"
#include <gazebo/transport/transport.hh>

using namespace gazebo;



GUI_ResetWorld::GUI_ResetWorld() : GUIPlugin()
{
    // Set the frame background and foreground colours
    this->setStyleSheet(
        "QFrame { background-color : rgba(100, 100, 100, 255); color : white; }"
    );

    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // Create frame to hold all widgets
    QFrame *mainFrame = new QFrame();

    // Create the layout that sits inside the frame
    QVBoxLayout *frameLayout = new QVBoxLayout();

    // Create a push button, connect it with the OnButton function
    QPushbutton *button = new QPushButton(tr("Reset World"));
    connect(button, SIGNAL(clicked()), this, SLOT(OnButton()));

    // Add the button to the frame layout
    frameLayout->addWidget(button);

    // Add frameLayout to the main layout
    mainLayout->addWidget(frameLayout);

    // remove margins to reduce space
    frameLayout->setContentsMargins(0,0,0,0);
    mainLayout->setContentsMargins(0,0,0,0);

    this->setLayout(mainLayout);

    // Position and rsize this widgets
    this->move(10,10);
    this->resize(120, 30);

    // Create node for transportation
    this->node = gazebo::transport::NodePtr(new gazebo::transport::Node());
    this->node->Init();
    this->factoryPub = this->Node->Advertise<msgs::Factory("~factory");

}