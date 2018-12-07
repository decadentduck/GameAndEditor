#include "FileReader.h"

using namespace GAME;

FileReader::FileReader()
{
}


FileReader::~FileReader()
{
}

std::vector<Model*> FileReader::LoadFile(char file[])
{
	models.clear();

	pugi::xml_document doc;
	pugi::xml_parse_result result;


	string name = string("");
	Vec3 pos = Vec3(0, 0, 0);
	int rot = 0;

	result = doc.load_file(file);

	if (result)
	{
		pugi::xml_node parent = doc.child("Objects");

		for (pugi::xml_node child : parent.children())
		{
			if (child.attribute("name"))
			{
				name = child.attribute("name").value();
			}

			for (pugi::xml_node grandChild : child.children("pos"))
			{
				float x, y, z;
				x = y = z = 0.0f;

				if (grandChild.attribute("X"))
				{
					x = atof(grandChild.attribute("X").value());
				}
				if (grandChild.attribute("Y"))
				{
					y = atof(grandChild.attribute("Y").value());
				}
				if (grandChild.attribute("Z"))
				{
					z = atof(grandChild.attribute("Z").value());
				}
				pos = Vec3(x, y, z);

			}

			for (pugi::xml_node grandChild : child.children("rot"))
			{
				if (grandChild.attribute("value"))
				{
					rot = atof(grandChild.attribute("value").value());
				}
			}

			//instantiate the object here
			if (!addModel(name, pos, rot)) return models;
			//empty temp values
			name = "";
			pos = Vec3(0.0f, 0.0f, 0.0f);
			rot = 0.0f;

		}
	}
	else { return models; }

	return models;
}



bool FileReader::addModel(const string tree, const Vec3 pos, const float rot)
{

	models.push_back(new Model(pos, Vec3(0.0f, 0.0f, 0.0f), rot, Vec3(0.05f, 0.05f, 0.05f), Vec3(0.0f, 0.0f, 0.0f)));
	models[models.size() - 1]->OnCreate();
	if (tree == "Tree1")
	{
		if (models[models.size() - 1]->LoadMesh("Tree1.obj") == false)
		{
			return false;
		}
	}
	else
	{
		if (models[models.size() - 1]->LoadMesh("Tree2.obj") == false)
		{
			return false;
		}
	}

	return true;
}

