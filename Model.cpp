#include <iostream>

#include <osg/PositionAttitudeTransform>
#include <osg/Vec3d>

#include "Model.h"
#include "FDMInterface.h"

using namespace std;

// constructor
modelData::modelData(osg::PositionAttitudeTransform* n, FDMInterface* f) {
    model_pat = n;
    fdmi = f;
    position = osg::Vec3d(0,0,0);
}

void modelData::updatePosition() {
    // get position from interface

    // v.1) get alt_asl using getter, returns Prop.->GetAlt...
    //double alt_asl = fdmi->get_alt_asl();

    // v.2) get alt_asl from interface using position struct,
    //      Prop.->GetAlt has to be done in separate call copy_from_JSBSim
    //double alt_asl = fdmi->position.alt_asl;
    //double lat = fdmi->position.lat;
    //double lon = fdmi->position.lon;
    //double r = fdmi->position.r;
    double er = fdmi->position.er;
    double X = fdmi->position.X;
    double Y = fdmi->position.Y;
    double Z = fdmi->position.Z;

    // v.1/2 ==> hard to see a difference atm... using v.2)

    //cout << "Altitude: " << alt_asl << endl;
    //cout << "lon: " << lon << endl;
    //cout << "lat: " << lat << endl;
    //cout << "Radius: " << r << endl;
    //cout << "Terrain radius: " << er << endl;
    //cout << "X: " << X << endl;
    //cout << "Y: " << Y << endl;
    //cout << "Z: " << Z << endl;

    position = osg::Vec3d(X, Y, Z);
    model_pat->setPosition(position);
}

void modelData::updateAttitude() {
    //...
}

