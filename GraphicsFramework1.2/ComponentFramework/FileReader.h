#pragma once
#include "pugixml.hpp"
#include "Vector.h"
#include "Model.h"

using namespace std;

namespace GAME
{
	class FileReader
	{
	public:
		FileReader();
		~FileReader();
		std::vector<Model*> LoadFile(char file[]);
	private:
		bool addModel(const string file, const Vec3 pos, const float rot);
		std::vector<Model*> models;
	};

}