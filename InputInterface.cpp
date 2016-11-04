// SFML
#include <SFML/Window.hpp>

// OpenSceneGraph
#include <osgViewer/Viewer>

#include "InputInterface.h"

void InputInterface::set_graphicswindow(osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> g) {

    gw = g;
}

void InputInterface::process_event(sf::Event& event) {
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
