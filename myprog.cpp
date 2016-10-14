// OpenSceneGraph
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

#include "FDMInterface.h"

using namespace std;

class modelData : public osg::Referenced {
    public:
        modelData(osg::PositionAttitudeTransform* n, FDMInterface* f);
        void updateAttitude();
        void updatePosition();
    protected:
        osg::PositionAttitudeTransform* modelPATNode;
        FDMInterface* fdmi;
        osg::Vec3d position;
        //osg::Quat attitude;
};

modelData::modelData(osg::PositionAttitudeTransform* n, FDMInterface* f) {
    modelPATNode = n;
    fdmi = f;
    position = osg::Vec3d(0,0,0);
}

void modelData::updatePosition() {
    // --> evtl. get position from FDM here ?
    double alt_asl = fdmi->get_alt_asl();
    cout << "Altitude: " << alt_asl << endl;
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
        cout << "3D model not found, leaving..." << endl;
        return 1;
    }

    // set up callback
    modelData* model_data = new modelData(modelPAT, fdm_interf);

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
