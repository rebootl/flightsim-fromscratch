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

        //bool cam_reorient_req;

    protected:
        sf::Window& window;
        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> gw;

        struct {
            int pos_x;
            int pos_y;
            int dx;
            int dy;
        } m;
        bool lmb_pressed = false;
        bool rmb_pressed = false;
        bool mmb_pressed = false;

};

#endif
