#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
//#include <osg/ShapeDrawable>

//#include <osgViewer/ViewerEventHandlers>
//#include <osgGA/StateSetManipulator>

// JSBSim
#include <FGFDMExec.h>

using namespace std;

class modelData : public osg::Referenced {
    public:
        modelData(osg::PositionAttitudeTransform* n);
        void updateAttitude();
        void updatePosition();
    protected:
        osg::PositionAttitudeTransform* modelPATNode;
        osg::Vec3d position;
        //osg::Quat attitude;
};

modelData::modelData(osg::PositionAttitudeTransform* n) {
    modelPATNode = n;
    position = osg::Vec3d(0,0,0);
}

void modelData::updatePosition() {
    // --> evtl. get position from FDM here ?
    position = position + osg::Vec3d(0,0,0.01);
    modelPATNode->setPosition(position);
}

void modelData::updateAttitude() {
    //...
}

class modelCallback : public osg::NodeCallback {
    public:
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
            osg::ref_ptr<modelData> md =
                dynamic_cast<modelData*> (node->getUserData());
            if (md) {
                md->updateAttitude();
                md->updatePosition();
        }
        traverse(node, nv);
    }
};

/*class modelData {

    public:
        modelData(osg::Vec3d* pos, osg::Vec3d* att);
        void set_position(osg::Vec3d* pos);
        void set_attitude(osg::Vec3d* att);
    protected:
        osg::Vec3d position;
        osg::Vec3d attitude;

}*/

/*void update_position_attitude(modelData& md) {
    //
}*/

/*
int initialize_flight_dynamics(const std::string& name) {
    JSBSim::FGFDMExec* fdmex = new JSBSim::FGFDMExec();
    fdmex->LoadModel(name);
    //FDMExec->DoTrim(mode);
}*/

/*int run_flight_dynamics() {
    //copy_inputs_to_jsbsim();
    //FDMExec->Run();
    //copy_outputs_from_jsbsim();
}*/

// (declaration)
class FDMInterface {
    // interface to FDM (JSBSim)
    // initialize and input/output interface
    public:
        // instantiate and initialize FDM
        // (constructor)
        FDMInterface(string& aircraft_name, string& aircraft_path);
    private:
        // main FDM instance
        JSBSim::FGFDMExec* fdmex;
};

FDMInterface::FDMInterface(string& aircraft_name, string& aircraft_path) {
    // initialize FDM
    fdmex = new JSBSim::FGFDMExec();
    fdmex->SetAircraftPath(aircraft_path);
    //fdmex->SetEnginePath("engine");
    //fdmex->SetSystemsPath("systems");
    fdmex->LoadModel(aircraft_name);
}

int main() {

    // SETTINGS:
    //
    string aircraft_name = "ball";
    string aircraft_path = "aircraft";

    // instantiate FDM
    FDMInterface* fdm_interf = new FDMInterface(aircraft_name, aircraft_path);

    // initialize fdm
    //initialize_flight_dynamics("ball");

	//Creating the viewer
    osgViewer::Viewer viewer;

	//Creating the root node
	osg::ref_ptr<osg::Group> root (new osg::Group);

    // create model position-attitude transformer
    osg::ref_ptr<osg::PositionAttitudeTransform> modelPAT (new osg::PositionAttitudeTransform);

    // object position vector
    // --> where is it used ?
    //osg::Vec3f objectPosTrans = osg::Vec3f(-1,3,5);

    // object geode ("geometry node")
    //osg::ref_ptr<osg::Geode> objectGeode (new osg::Geode);

    // load model from file
    osg::ref_ptr<osg::Node> model (osgDB::readNodeFile("cessna.osgt"));
    if (!model) {
        return 1;
    }

    // set up callback
    modelData* model_data = new modelData(modelPAT);

    modelPAT->setUserData(model_data);
    modelPAT->setUpdateCallback(new modelCallback);

    // set up scene graph
    //
    // root --> modelPAT --> model
    //

    // add the object geode as a child of the object PAT
    modelPAT->addChild(model.get());

    root->addChild(modelPAT.get());

    // set scene to render and run
    viewer.setSceneData(root.get());

    // attach a trackball manipulator to all user control of the view
    // (needed when loop is used below)
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);

    // instead of returning, use loop now
    //return viewer.run();
    viewer.realize();

    while (!viewer.done()) {
        //modelData->updatePosition();
        viewer.frame();
    }


    //---------(previous example)

	//The geode containing our shpae
   	//osg::ref_ptr<osg::Geode> myshapegeode (new osg::Geode);

	//Our shape: a capsule, it could have been any other geometry (a box, plane, cylinder etc.)
	//osg::ref_ptr<osg::Capsule> myCapsule (new osg::Capsule(osg::Vec3f(),1,2));

	//Our shape drawable
	//osg::ref_ptr<osg::ShapeDrawable> capsuledrawable (new osg::ShapeDrawable(myCapsule.get()));

	//myshapegeode->addDrawable(capsuledrawable.get());

	//root->addChild(myshapegeode.get());

	//viewer.setSceneData( root.get() );
	//viewer.setThreadingModel(osgViewer::Viewer::ThreadingModel::SingleThreaded);

	//Stats Event Handler s key
	//viewer.addEventHandler(new osgViewer::StatsHandler);

	//Windows size handler
	//viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// add the state manipulator
    //viewer.addEventHandler( new 		osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	//The viewer.run() method starts the threads and the traversals.
	//return (viewer.run());
}
