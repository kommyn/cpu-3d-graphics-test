#pragma once

#include <string>

// Turns a relative asset name, e.g. "models/Izba/OBJ/Farmhouse OBJ.obj",
// into an absolute path resolved from the executable location instead of the
// current working directory, so the program can be started from anywhere.
//
// The returned path always uses forward slashes: Model3D derives the material
// and texture folders from the model path with regular expressions that match
// '/' only.
std::string assetPath(const std::string& relative);
