#include <iostream>
#include "FDMInterface.h"
#include "JSBSim/src/math/FGLocation.h"
#include "JSBSim/src/math/FGColumnVector3.h"

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

    // get position in local frame starting at 90deg North on the earth surf. (0,0,earth_radius)
    const JSBSim::FGLocation& fgloc = Propagate->GetLocation();
    double er = Propagate->GetLocalTerrainRadius();
    // --> leads to wrong coordinates...???
    JSBSim::FGColumnVector3 vec_local = fgloc.LocationToLocal(JSBSim::FGColumnVector3(0,0,er));
    position.loc_X = vec_local(1)*FT2M;
    position.loc_Y = vec_local(2)*FT2M;
    position.loc_Z = vec_local(3)*FT2M;
    position.X = fgloc.Entry(1);
    position.Y = fgloc.Entry(2);
    position.Z = fgloc.Entry(3);
    //position.ecef_X = fgloc.Entry(1);

    // attitude

}

/*double FDMInterface::get_alt_asl() {
    return Propagate->GetAltitudeASLmeters();
}*/
