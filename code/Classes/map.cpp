#include <iostream>
#include <fstream>
#include "map.h"


vector<string> MapClass::mapTexture = vector<string>();

MapClass::MapClass()
{
	if (mapTexture.size() == 0) {
		init();
	}
}

Sprite* MapClass::createSprite(int texture)
{
	auto sprite = Sprite::create(mapTexture[texture].c_str());
	Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	sprite->setTag(texture);
	sprite->getTexture()->setTexParameters(texParams);
	sprite->setAnchorPoint(Vec2::ZERO);

	return sprite;
}


void MapClass::saveMap(string filename)
{
	auto path = FileUtils::getInstance()->getWritablePath();
	ofstream file(path + filename);
	for (auto const &element : mapElement) {
		file << element->getPosition().x << " " << element->getPosition().y << " " <<
			element->getContentSize().width << " " << element->getContentSize().height << " " << element->getTag() << endl;
	}
}

void MapClass::loadMap(string filename)
{
	auto path = FileUtils::getInstance()->getWritablePath();
	ifstream file(path + filename);

	if (file) {
		mapElement.clear();
		while (!file.eof()) {
			float x, y, w, h;
			int tag;
			file >> x >> y >> w >> h >> tag;

			auto sprite = createSprite(tag);
			sprite->setTextureRect(Rect(0, 0, w, h));
			sprite->setPosition(x, y);
			mapElement.emplace_back(sprite);

		}
	}
}

void MapClass::push(Sprite * element)
{
	mapElement.push_back(element);
}

void MapClass::remove(Sprite * element)
{
	mapElement.remove(element);
}

void MapClass::init()
{
	mapTexture = vector<string>({ "block1.png", "block2.png" });
}
