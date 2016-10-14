#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
//#include <osg/ShapeDrawable>

//#include <osgViewer/ViewerEventHandlers>
//#include <osgGA/StateSetManipulator>

// JSBSim
#include <FGFDMExec.h>
#include <initialization/FGInitialCondition.h>

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
        FDMInterface(string& aircraft_name, string& aircraft_path, string& ic_name);
    private:
        // main FDM instance
        JSBSim::FGFDMExec* fdmex;
        JSBSim::FGInitialCondition *ic;
};

// (constructor)
FDMInterface::FDMInterface(string& aircraft_name, string& aircraft_path, string& ic_name) {
    // create JSBSim instance
    cout << "initializing FDM" << endl;
    fdmex = new JSBSim::FGFDMExec();
    fdmex->SetRootDir("");
    fdmex->SetAircraftPath(aircraft_path);
    //fdmex->SetEnginePath("engine");
    //fdmex->SetSystemsPath("systems");

    // load aircraft FDM
    fdmex->LoadModel(aircraft_name);
    // (error is catched by JSBSim 255)
    /*if ( ! fdmex->LoadModel(aircraft_name)) {
        delete fdmex;
        cerr << "could not load FDM model..." << endl;
        exit(1);
    }*/

    // load initial conditions
    ic = fdmex->GetIC();
    ic->Load(ic_name);
    // (error is catched by JSBSim 255)
    /*if ( ! ic->Load(ic_name)) {
        delete fdmex;
        cerr << "could not load initial FDM conditions" << endl;
        exit(1);
    }*/

    // run once w/o integrating
    // --> copy to JSBSim
    fdmex->RunIC();
    // --> do trim here
    // --> copy from JSBSim
}

int main() {

    // FDM

    // Settings:
    //
    string aircraft_name = "ball";
    string aircraft_path = "aircraft";
    // load initial conditions from file (for now)
    string ic_name = "cannonball_init";

    // instantiate FDM
    FDMInterface* fdm_interf = new FDMInterface(aircraft_name, aircraft_path, ic_name);

    // initialize fdm
    // --> done in the constructor
    //initialize_flight_dynamics("ball");


    // GRAPHICS

	//Creating the viewer
    osgViewer::Viewer viewer;

	//Creating the root node
	osg::ref_ptr<osg::Group> root (new osg::Group);

    // create model position-attitude transformer
    osg::ref_ptr<osg::PositionAttitudeTransform> modelPAT (new osg::PositionAttitudeTransform);

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


    // MAIN LOOP

    while (!viewer.done()) {
        //modelData->updatePosition();
        viewer.frame();
    }

}
