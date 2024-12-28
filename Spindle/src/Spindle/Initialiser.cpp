#include "Initialiser.h"
#include "./SubsystemManagers/Manager.h"

namespace Spindle {
    void Spindle::Initialiser::Init()
    {
        // subsystem managers start up
        Manager::get().startUp();

        // subsystem managers shut down 
        Manager::get().shutDown();
    }
}
