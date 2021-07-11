#pragma once

#include <vector>

#include "Module.h"
#include "PDB_config.h"

PDB_Module PDB;

std::vector<Module*> gModules = {
    // put modules here
    &PDB,
};
