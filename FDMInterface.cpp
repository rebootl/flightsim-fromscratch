#include <iostream>
//#include <cmath>

// JSBSim
#include "JSBSim/src/math/FGLocation.h"
#include "JSBSim/src/math/FGColumnVector3.h"
#include "JSBSim/src/math/FGMatrix33.h"

#include "FDMInterface.h"

using namespace std;

const double FT2M = 0.3048;

// (constructor)
FDMInterface::FDMInterface( string& aircraft_name,
                            string& aircraft_path,
                            string& ic_name ) {
    // create JSBSim instance
    cout << "initializing FDM" << endl;
    fdmex = new JSBSim::FGFDMExec();

    Propagate = fdmex->GetPropagate();

    fdmex->SetRootDir("");
    fdmex->SetAircraftPath(aircraft_path);
    //fdmex->SetEnginePath("engine");
    //fdmex->SetSystemsPath("systems");

    // load aircraft FDM
    fdmex->LoadModel(aircraft_name);
    // (error is catched by JSBSim 255)

    // load initial conditions
    ic = fdmex->GetIC();
    ic->Load(ic_name);
    // (error is catched by JSBSim 255)

    // initial run (once w/o integrating)
    // --> copy to JSBSim
    fdmex->RunIC();
    // --> do trim here
    // --> copy from JSBSim
    cout << "FDM initialization complete" << endl;
}

void FDMInterface::update() {
    // --> copy to JSBSim
    fdmex->Run();
    // --> copy from JSBSim
}

void FDMInterface::update_from_JSBSim() {
    // position
    position.alt_asl = Propagate->GetAltitudeASLmeters();
    position.lon = Propagate->GetLongitude();
    position.lat = Propagate->GetLatitude();
    position.r = Propagate->GetRadius()*FT2M;
    position.er = Propagate->GetLocalTerrainRadius()*FT2M;

    const JSBSim::FGLocation& fgloc = Propagate->GetLocation();
    position.X = fgloc.Entry(1)*FT2M;
    position.Y = fgloc.Entry(2)*FT2M;
    position.Z = fgloc.Entry(3)*FT2M;

    // attitude

}

/*double FDMInterface::get_alt_asl() {
    return Propagate->GetAltitudeASLmeters();
}*/
