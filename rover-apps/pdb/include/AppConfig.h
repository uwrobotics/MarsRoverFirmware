#pragma once

#include <vector>

#include "Module.h"
#include "PDBMonitoring.h"

PDBMonitoring PDB;

std::vector<Module*> gModules = {
    // put modules here
    &PDB,
};
