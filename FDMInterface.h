// JSBSim
#include "JSBSim/src/FGFDMExec.h"
#include "JSBSim/src/initialization/FGInitialCondition.h"

// (declaration)
class FDMInterface {
    // interface to FDM (JSBSim)
    // initialization and input/output interface
    public:
        // instantiate and initialize FDM
        // (constructor)
        FDMInterface( string& aircraft_name,
                      string& aircraft_path,
                      string& ic_name );
        // position
        double get_alt_asl(); // altitude above sea level in m
        double get_lon();
        double get_lan();
        // attitude
        double get_heading();
        double get_roll();
        double get_pitch();
    private:
        // main FDM instance
        JSBSim::FGFDMExec* fdmex;
        JSBSim::FGInitialCondition *ic;

        // "interface classes"
        // (contains e.g. position)
        JSBSim::FGPropagate* Propagate;

        // update data from/to JSBSim
        //void copy_from_JSBSim();
        //void copy_to_JSBSim();
};
