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

namespace Potato
{
	
	// ÊÍ·Å×ÊÔ´
	#define DeleteObject(object) if((object) != nullptr) { delete object; object = nullptr; }
	#define DeleteObjects(objects) if((objects) != nullptr) { delete[] objects; objects = nullptr; }
	#define DeleteObjectsVector(objects) for(auto obj : objects){DeleteObject(obj);}
	#define ReleaseObject(object) if((object) != nullptr) { object->Release(); object = nullptr; }

	struct BitMap
	{
		int bitWidth;
		int bitHeight;
		std::vector<DirectX::XMFLOAT3> color;

		DirectX::XMFLOAT3 Get(DirectX::XMFLOAT2 tex)
		{
			int x = (int)(bitWidth-1) * tex.x;
			int y = (int)(bitHeight-1) * tex.y;

			return color.at(x + y * bitWidth);
		}
	};

}

#endif // !DEF_H
