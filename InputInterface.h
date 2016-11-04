#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

// SFML
#include <SFML/Window.hpp>

// OpenSceneGraph
#include <osgViewer/Viewer>

// using SFML window and input events
class InputInterface {

    public:
        // (constructor)
        InputInterface(sf::Window& w) : window(w) {}
        // needed for window resize
        void set_graphicswindow(osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> g);

        void process_event(sf::Event& event);

    protected:
        sf::Window& window;
        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> gw;
};

#endif
