#include <iostream>

// SFML
#include <SFML/Window.hpp>

// OpenSceneGraph
#include <osgViewer/Viewer>

#include "InputInterface.h"

using namespace std;

void InputInterface::set_graphicswindow(osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> g) {
    gw = g;
}

void InputInterface::process_event(sf::Event& event) {
    switch(event.type) {
    case sf::Event::MouseMoved:
        cout << "Mx: " << event.mouseMove.x << " My: " << event.mouseMove.y << endl;

        // RMB pressed: reorient model (for now)
        if (rmb_pressed) {
            m.dx = event.mouseMove.x - m.pos_x;
            m.dy = -event.mouseMove.y + m.pos_y;
            cout << "Mdx: " << m.dx << " Mdy: " << m.dy << endl;
            // --> cam->reorient_model(mdx, mdy);
        }
        // register the mouse position
        m.pos_x = event.mouseMove.x;
        m.pos_y = event.mouseMove.y;
        break;
    case sf::Event::MouseButtonPressed:
        if( event.mouseButton.button == sf::Mouse::Left ) {
            lmb_pressed = true;
            cout << "LMB pressed" << endl;
        }
        else if( event.mouseButton.button == sf::Mouse::Right ) {
            rmb_pressed = true;
            cout << "RMB pressed" << endl;
            //cam_reorient_req = true;
        }
        break;
    case sf::Event::MouseButtonReleased:
        if( event.mouseButton.button == sf::Mouse::Left ) {
            lmb_pressed = false;
            cout << "LMB released" << endl;
        }
        else if( event.mouseButton.button == sf::Mouse::Right ) {
            rmb_pressed = false;
            cout << "RMB released" << endl;
            //cam_reorient_req = false;
        }
        break;
    case sf::Event::KeyReleased:
        break;
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
        break;
    case sf::Event::Resized:
        gw->resized(0, 0, event.size.width, event.size.height);
        break;
    case sf::Event::Closed:
        window.close();
        break;
    default:
        break;
    }
}
