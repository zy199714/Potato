/**
 * 
 */

#ifndef DEF_H
#define DEF_H

#pragma once

#include <exception>
#include <cassert>
#include <vector>
#include <map>
#include <stack>
#include <memory>
#include <string>
#include <iostream>

#include "DXTrace.h"


// ÊÍ·Å×ÊÔ´
#define DeleteObject(object) if((object) != nullptr) { delete object; object = nullptr; }
#define DeleteObjects(objects) if((objects) != nullptr) { delete[] objects; objects = nullptr; }
#define DeleteObjectsVector(objects) for(auto obj : objects){DeleteObject(obj);}
#define ReleaseObject(object) if((object) != nullptr) { object->Release(); object = nullptr; }

#endif // !DEF_H
