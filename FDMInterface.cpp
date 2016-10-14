#include "FDMInterface.h"

using namespace std;

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

double FDMInterface::get_alt_asl() {
    return Propagate->GetAltitudeASLmeters();
}
