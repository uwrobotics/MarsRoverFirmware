#pragma once

#include <vector>

#include "Module.h"
#include "PDBMonitoring.h"

PDBMonitoring PDB_monitoring;

std::vector<Module*> gModules = {
    // put modules here
    &PDB_monitoring,
};
