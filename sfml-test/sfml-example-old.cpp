// g++ main.cpp -o main -lsfml-window -lsfml-graphics -lsfml-system -losg -losgDB -losgGA -losgUtil

#include <SFML/Graphics.hpp>

#include <osgUtil/SceneView>
#include <osg/Node>
#include <osg/CameraNode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>


int  main()
{
    // Create Game Window with Title
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML2 OpenSceneGraph Example1");
   
    // Create & Load Splashscreen Image
    sf::Texture texture;
    if (!texture.loadFromFile("data/background.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);


    // Create 3D Viewport
    osgUtil::SceneView* viewer = new osgUtil::SceneView();
    viewer->setDefaults();
 
    // Create Camera
    osg::Camera* camera;
    camera = viewer->getCamera();
    camera->setViewport(0, 0, 640, 480); // Set 3D Viewport Position and Size
    camera->setClearColor(osg::Vec4(0, 0, 0, 0)); // Set Scene Background Color

    // Create Root Scene Node
    osg::Group* root = new osg::Group();
 
    // Transformation Object for our 3D Model
    osg::PositionAttitudeTransform* meshTrans = new osg::PositionAttitudeTransform();
    meshTrans->setPosition(osg::Vec3d( 3,  0, -6));

    // Create 3D Model, Apply Transformation
    osg::Node* mesh = osgDB::readNodeFile("data/untitled.3ds");
    root->addChild(meshTrans); // Add Trasformation to Scene
    meshTrans->addChild(mesh); // Add 3D Model to Transformation
     
    // Set Root Scene Node to View and Init Vew
    viewer->setSceneData(root);
    viewer->init();


    // Start the Game Loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
         
        window.clear();        
        window.draw(sprite);
       
        window.pushGLStates();
        viewer->update();
        viewer->cull();
        viewer->draw();
        window.popGLStates();

        window.display();
    }
}
 
