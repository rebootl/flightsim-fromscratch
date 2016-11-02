#include <iostream>

// SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <GL/gl.h>
#include <GL/glu.h>

// OpenSceneGraph
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
//#include <osgGA/OrbitManipulator>
#include <osgDB/ReadFile>

// project
#include "FDMInterface.h"
#include "Model.h"
//#include "Input.h"

using namespace std;

int main() {

    // SFML Window

    // (leads to rendering prob., repl. by below)
    /*sf::Window window;
    window.create(sf::VideoMode(800, 600), "My window");*/

    sf::Window window(sf::VideoMode(800, 600),
                      "My Window",
                      sf::Style::Default,
                      sf::ContextSettings(24));

    window.setVerticalSyncEnabled(true);

    sf::Vector2u size = window.getSize();


    // FDM

    // Settings
    //
    string aircraft_name = "ball";
    string aircraft_path = "aircraft";
    // load initial conditions from file (for now)
    string ic_name = "cannonball_init";

    // instantiate FDM
    FDMInterface* fdm_interf = new FDMInterface(aircraft_name, aircraft_path, ic_name);

    // get earth radius for position
    //double er = fdm_interf->position.er;

    // initialize fdm
    // --> done in the constructor
    //initialize_flight_dynamics("ball");


    // TERRAIN

    // create landmark at north-pole

    // north pole coordinates
    //osg::Vec3d npc = osg::Vec3d(0,0,6378137);
    //osg::Vec3d np_vec = osg::Vec3d(0, 0, 6378137);
    osg::Vec3d np_vec = osg::Vec3d(0,0,    0);

    osg::Geode* terrain1 = new osg::Geode();

    // ==> creating the capsule at the north-pole directly creates a corrupted model
    //     for some reason, evtl. some sort of out-of-range problem
    terrain1->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(0,0,0), 0.5f, 1.f)));
    osg::ref_ptr<osg::PositionAttitudeTransform> terrain1_pat (new osg::PositionAttitudeTransform);
    terrain1_pat->addChild(terrain1);
    terrain1_pat->setPosition(np_vec);
    // --> give it a nice color !!


    // MODEL (aircraft)

    // object geode ("geometry node")
    //osg::ref_ptr<osg::Geode> objectGeode (new osg::Geode);

    // load model from file
    osg::ref_ptr<osg::Node> model (osgDB::readNodeFile("cessna.osgt"));
    if (!model) {
        cout << "3D model not found, leaving..." << endl;
        return 1;
    }

    // create model position-attitude transformer
    osg::ref_ptr<osg::PositionAttitudeTransform> model_pat (new osg::PositionAttitudeTransform);

    // add the object geode as a child of the object PAT
    model_pat->addChild(model.get());

    // set up callback
    modelData* model_data = new modelData(model_pat, fdm_interf);
    model_pat->setUserData(model_data);
    model_pat->setUpdateCallback(new modelCallback);


    // ROOT SCENE GRAPH

    // set up scene graph
    //
    // root --> model_pat --> model
    //

	// creating the root node
	osg::ref_ptr<osg::Group> root (new osg::Group);

    root->addChild(model_pat.get());
    root->addChild(terrain1_pat.get());


    // VIEWER

	//Creating the viewer
    osgViewer::Viewer viewer;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> gw = viewer.setUpViewerAsEmbeddedInWindow(0, 0, size.x, size.y);

    // set scene to render and run
    viewer.setSceneData(root.get());

    // attach a trackball manipulator to all user control of the view
    // (needed when loop is used below)
    //osgGA::OrbitManipulator* om = new osgGA::OrbitManipulator;
    osgGA::TrackballManipulator* om = new osgGA::TrackballManipulator;
    viewer.setCameraManipulator(om);
    viewer.getCameraManipulator()->setHomePosition( np_vec + osg::Vec3d(0,-10,3),  // eye
                                                    np_vec,                        // center
                                                    osg::Vec3d(0,0,1)          // up
                                                    );
    viewer.home();
    //om->setMinimumDistance(5, false);
    //viewer.getCameraManipulator()->setMinimumDistance(5, false);
    //viewer.getCameraManipulator()->setCenter( np_vec );

    // input
    //myKeyboardEventHandler* myFirstEventHandler = new myKeyboardEventHandler();
    //viewer.addEventHandler(myFirstEventHandler);

    // instead of returning, use loop now
    //return viewer.run();
    viewer.realize();


    // GAME LOOP

    while (window.isOpen()) {

        // --> read input
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;
            case sf::Event::Resized:
                gw->resized(0, 0, event.size.width, event.size.height);
                break;
            }
        }

        // update FDM
        // --> copy input to JSBSim
        fdm_interf->update();
        fdm_interf->update_from_JSBSim();

        // update camera
        //om->setCenter( np_vec + osg::Vec3d(0,0,-2) );
        //om->setDistance(2);

        window.setActive();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // OSG Viewer step
        //
        // this is updating the model via
        // - modelCallback
        //     md (modelData)
        //       model_pat
        //       fdmi
        //       -> updateAttitude
        //       -> updatePosition
        viewer.frame();

        window.display();

    }
    return 0;
}
