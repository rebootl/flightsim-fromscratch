#include <iostream>
// OpenSceneGraph
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

#include "FDMInterface.h"

using namespace std;

struct pos_data {
    double alt_asl;
    double lon;
    double lat;
};

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
    // get position from interface

    // v.1) get alt_asl using getter, returns Prop.->GetAlt...
    //double alt_asl = fdmi->get_alt_asl();

    // v.2) get alt_asl from interface using position struct,
    //      Prop.->GetAlt has to be done in separate call copy_from_JSBSim
    double alt_asl = fdmi->position.alt_asl;
    double lat = fdmi->position.lat;
    double lon = fdmi->position.lon;
    double r = fdmi->position.r;
    double loc_x = fdmi->position.loc_X;
    double loc_y = fdmi->position.loc_Y;
    double loc_z = fdmi->position.loc_Z;
    double x = fdmi->position.X;
    double y = fdmi->position.Y;
    double z = fdmi->position.Z;

    // v.1/2 ==> hard to see a difference atm... using v.2)

    //cout << "Altitude: " << alt_asl << endl;
    cout << "lon: " << lon << endl;
    cout << "lat: " << lat << endl;
    //cout << "Radius: " << r << endl;
    cout << "local x: " << loc_x << endl;
    cout << "local y: " << loc_y << endl;
    cout << "local z: " << loc_z << endl;
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
    cout << "z: " << z << endl;

    //position = position + osg::Vec3d(0,0,0.01);
    position = osg::Vec3d(loc_x, loc_y, loc_z);
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
        // update FDM
        fdm_interf->update();
        fdm_interf->update_from_JSBSim();

        // update viewer
        viewer.frame();
    }

}
