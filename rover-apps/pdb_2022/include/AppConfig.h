#pragma once

#include <vector>

#include "Module.h"
#include "PDBMonitoring.h"

PDBMonitoring PDB_monitoring;

std::vector<Module*> gModules = {
    &PDB_monitoring,
};
