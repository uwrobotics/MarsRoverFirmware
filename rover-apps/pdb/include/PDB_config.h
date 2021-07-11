#pragma once

constexpr float PDB_VBAT_RAIL_NOMINAL_VOLTAGE = 24.0;
constexpr float PDB_VBAT_RAIL_MIN_THRESHOLD   = 18.0;
constexpr float PDB_VBAT_RAIL_MAX_THRESHOLD   = 25.2;

constexpr float PDB_24V_RAIL_NOMINAL_VOLTAGE = 24.0;
constexpr float PDB_24V_RAIL_MIN_THRESHOLD   = 22.0;
constexpr float PDB_24V_RAIL_MAX_THRESHOLD   = 26.0;

constexpr float PDB_17V_RAIL_NOMINAL_VOLTAGE = 17.0;
constexpr float PDB_17V_RAIL_MIN_THRESHOLD   = 16.0;
constexpr float PDB_17V_RAIL_MAX_THRESHOLD   = 18.0;

constexpr float PDB_5V_RAIL_NOMINAL_VOLTAGE = 5.0;
constexpr float PDB_5V_RAIL_MIN_THRESHOLD   = 4.8;
constexpr float PDB_5V_RAIL_MAX_THRESHOLD   = 6.0;

constexpr float PDB_TEMPERATURE_MIN_THRESHOLD = 10.0;
constexpr float PDB_TEMPERATURE_MAX_THRESHOLD = 50.0;

constexpr bool PDB_5V_LOAD1_DIAG_EN = 1;
constexpr bool PDB_5V_LOAD2_DIAG_EN = 0;
constexpr bool PDB_5V_LOAD3_DIAG_EN = 0;
constexpr bool PDB_5V_LOAD4_DIAG_EN = 0;
constexpr bool PDB_5V_LOAD5_DIAG_EN = 0;
constexpr bool PDB_17V_LOAD_DIAG_EN = 1;