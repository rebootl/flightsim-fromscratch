/* OpenSceneGraph example, osgviewerSFML.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

// (C) 2005 Mike Weiblen http://mew.cx/ released under the OSGPL.
// Simple example using GLUT to create an OpenGL window and OSG for rendering.
// Derived from osgGLUTsimple.cpp and osgkeyboardmouse.cpp

#include <memory>
#include <iostream>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <SFML/Window.hpp>

template<typename G,typename W>
bool convertEvent(sf::Event& event, G gw, W window)
{
    auto eventQueue = gw->getEventQueue();

    switch (event.type)
    {
        case sf::Event::MouseMoved:
            eventQueue->mouseMotion(event.mouseMove.x, event.mouseMove.y);
            return true;

        case sf::Event::MouseButtonPressed:
            if( event.mouseButton.button == sf::Mouse::Left )
            {
                event.mouseButton.button = static_cast<decltype(event.mouseButton.button)>(1);
            }
            else if( event.mouseButton.button == sf::Mouse::Right )
            {
                event.mouseButton.button = static_cast<decltype(event.mouseButton.button)>(3);
            }
            eventQueue->mouseButtonPress(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            return true;

        case sf::Event::MouseButtonReleased:
            if( event.mouseButton.button == sf::Mouse::Left )
            {
                event.mouseButton.button = static_cast<decltype(event.mouseButton.button)>(1);
            }
            else if( event.mouseButton.button == sf::Mouse::Right )
            {
                event.mouseButton.button = static_cast<decltype(event.mouseButton.button)>(3);
            }
            eventQueue->mouseButtonRelease(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            return true;

        case sf::Event::KeyReleased:
            eventQueue->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) event.key.code);
            return true;

        case sf::Event::KeyPressed:
            eventQueue->keyPress( (osgGA::GUIEventAdapter::KeySymbol) event.key.code);
            if ( event.key.code == sf::Keyboard::Escape )
            {
                window->close();
            }
            return true;

        case sf::Event::Resized:
            eventQueue->windowResize(0, 0, event.size.width, event.size.height );
            gw->resized(0, 0, event.size.width, event.size.height );
            return true;

        case sf::Event::Closed:
            window->close();
            return true;

        default:
            break;
    }

    return false;
}

static std::shared_ptr<sf::Window> CreateWindowS( std::string title, unsigned int width, unsigned int height )
{
    return std::make_shared<sf::Window>(
            sf::VideoMode(width, height),
            sf::String::fromUtf8(title.begin(),title.end()),
            sf::Style::Default,
            sf::ContextSettings(24) );
}

int main( int argc, char **argv )
{
    if (argc<2)
    {
        std::cout << argv[0] <<": requires filename argument." << std::endl;
        return EXIT_FAILURE;
    }

    // load the scene.
    auto   loadedModel = osgDB::readRefNodeFile(argv[1]);

    if (!loadedModel)
    {
        std::cout << argv[0] <<": No data loaded." << std::endl;
        return EXIT_FAILURE;
    }

    int width = 800;
    int height = 600;

    auto  window = CreateWindowS("OSG Viewer",width,height);

    osgViewer::Viewer    viewer;

    auto gw = viewer.setUpViewerAsEmbeddedInWindow(0,0,width,height);

    viewer.setSceneData(loadedModel.get());
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.realize();

    while( window->isOpen() )
    {
        sf::Event   event;

        while ( window->pollEvent(event) )
        {
            // pass the SFML event into the viewers event queue
            convertEvent( event, gw, window );
        }

        viewer.frame();
        window->display();
    }

    return EXIT_SUCCESS;
}
