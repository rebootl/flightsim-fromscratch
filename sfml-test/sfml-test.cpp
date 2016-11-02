// compile e.g.:
// g++ sfml-test.cpp -o sfml-test -lsfml-window -lsfml-graphics -lsfml-system -losg -losgViewer -losgDB -losgGA

#include <iostream>

// SFML
#include <SFML/Window.hpp>
//#include <SFML/Graphics.hpp>

// OpenSceneGraph
#include <osg/PositionAttitudeTransform>
//#include <osg/ShapeDrawable>

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
//#include <osgGA/OrbitManipulator>
#include <osgDB/ReadFile>

using namespace std;

int main() {

    // SFML Window

    sf::Window window(sf::VideoMode(800, 600),
                      "FOOBAR",
                      sf::Style::Default,
                      sf::ContextSettings(24));
    /*sf::Window window;
    window.create(sf::VideoMode(800, 600), "My window", sf::Style::Default, sf::ContextSettings(24));
    window.create(sf::VideoMode(800, 600), "My window"));*/
    window.setVerticalSyncEnabled(true);

    sf::Vector2u size = window.getSize();

    // OSG

    // load model from file
    //osg::ref_ptr<osg::Node> model (osgDB::readNodeFile("cessna.osgt"));
    osg::ref_ptr<osg::Node> model (osgDB::readNodeFile("cessna.osg"));
    if (!model) {
        cout << "3D model not found, leaving..." << endl;
        return 1;
    }

    // create model position-attitude transformer
    osg::ref_ptr<osg::PositionAttitudeTransform> model_pat (new osg::PositionAttitudeTransform);

    // add the object geode as a child of the object PAT
    model_pat->addChild(model.get());

    // set up scene graph
    //
    // root --> model_pat --> model
    //

	// creating the root node
	osg::ref_ptr<osg::Group> root (new osg::Group);

    root->addChild(model_pat.get());

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
    viewer.getCameraManipulator()->setHomePosition( osg::Vec3d(30,-50,30),  // eye
                                                    osg::Vec3d(0,0,0),      // center
                                                    osg::Vec3d(0,0,1)       // up
                                                    );
    viewer.home();

    // instead of returning, use loop now
    viewer.realize();

    /*auto state = gw->getState();
    state->setUseModelViewAndProjectionUniforms(true);
    state->setUseVertexAttributeAliasing(true);*/

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


        window.setActive();
        //window.pushGLStates();
        // OSG Viewer step
        viewer.frame();
        //window.popGLStates();
        window.display();

    }
    return 0;
}
