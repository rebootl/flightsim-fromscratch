// for now use osg to handle inputs
//
//

#ifndef INPUT_H
#define INPUT_H

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
//#include <osgGA/GUIEventHandlerVisitor>

// input interface
class InputInterface {

    public:
        // (constructor)
        //InputInterface();

        void update_input();

        // control surfaces
        // (--> use mouse for these)
        /*bool elevator_up_kreq;
        bool elevator_down_kreq;
        bool aileron_up_kreq;
        bool aileron_down_kreq;*/

        // view inputs
        struct {
            double heading;
            double elevation;
            double distance;
        } view;

        double zoom_fac = 0.1;
        void zoom_out();

        // control inputs
        struct {
            double aileron;
            double elevator;
            //double "yaw??";
            double throttle;

            double fac = 0.01;
        } ctrl;

};

InputInterface::zoom_out() {
    view.distance += zoom_fac;
}

InputInterface::zoom_in() {
    view.distance -= zoom_fac;
}

//InputInterface::update_input() {
    // (--> use mouse for this)
    /*if (elevator_up) {
        elevator += fac_ctrl_srf;
    }*/
//}

// override GUIEventHandler (acc. to tutorial)
// (declaration)
class myKeyboardEventHandler : public osgGA::GUIEventHandler {

    public:
        virtual bool handle( const osgGA::GUIEventAdapter& ea,
                             osgGA::GUIActionAdapter& );
        /*virtual void accept(osgGA::GUIEventHandlerVisitor& v) {
            v.visit(*this);
        };*/

};

bool myKeyboardEventHandler::handle( const osgGA::GUIEventAdapter& ea,
                                     osgGA::GUIActionAdapter& aa ) {
    switch(ea.getEventType()) {
        case(osgGA::GUIEventAdapter::KEYDOWN): {
            switch(ea.getKey()) {
                case 'w':
                    std::cout << " w key pressed" << std::endl;
                    return false;
                    break;
                case 'KEY_Up';
                    std::cout << " up key pressed" << std::endl;
                    return true;
                    break;
                default:
                    return false;
            }
        }
        case osgGA::GUIEventAdapter::SCROLL: {
            osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
            switch(sm) {
                case osgGA::GUIEventAdapter::SCROLL_UP:
                    ii->zoom_out();
                    return true;
                case osgGA::GUIEventAdapter::SCROLL_DOWN:
                    ii->zoom_in();
                    return true;
                default:
                    return false;
            }
        }
        default:
            return false;
    }
}

#endif
