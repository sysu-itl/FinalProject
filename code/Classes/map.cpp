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
	//精灵
	auto sprite = Sprite::create(mapTexture[texture].c_str());
	Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	sprite->setTag(texture);
	sprite->getTexture()->setTexParameters(texParams);
	sprite->setAnchorPoint(Vec2::ZERO);

	return sprite;
}

void MapClass::setPhysic(Sprite* sprite)
{
	//物理body
	auto playerBody1 = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	playerBody1->setCategoryBitmask(0xFFFFFFFF);
	playerBody1->setCollisionBitmask(0xFFFFFFFF);
	playerBody1->setContactTestBitmask(0xFFFFFFFF);
	playerBody1->setDynamic(false);
	sprite->setPhysicsBody(playerBody1);
}

void MapClass::saveMap(string filename)
{
	if (mapElement.size()) {
		origin = mapElement.front()->getPosition();

		auto path = FileUtils::getInstance()->getWritablePath();
		ofstream file(path + filename);
		for (auto const &element : mapElement) {
			auto position = element->getPosition() - mapElement.front()->getPosition() + origin;
			file << position.x << " " << position.y << " " <<
				element->getContentSize().width << " " << element->getContentSize().height << " " << element->getTag() << endl;
		}
	}
}

bool MapClass::loadMap(string filename)
{
	auto path = FileUtils::getInstance()->getWritablePath();
	ifstream file(path + filename);

	if (file) {
		CCLOG("mapelement");
		mapElement.clear();
		while (!file.eof()) {
			float x, y, w, h;
			int tag;
			file >> x >> y >> w >> h >> tag;

			auto sprite = createSprite(tag);
			sprite->setTextureRect(Rect(0, 0, w, h));
			setPhysic(sprite);
			sprite->setPosition(x, y);
			mapElement.emplace_back(sprite);

			if (mapElement.size())
				origin = mapElement.front()->getPosition();
		}

		return true;
	}
	return false;
}

void MapClass::push(Sprite * element)
{
	mapElement.push_back(element);
	origin = mapElement.front()->getPosition();
}

void MapClass::remove(Sprite * element)
{
	mapElement.remove(element);
	if (mapElement.size()) {
		origin = mapElement.front()->getPosition();
	}
		
}

void MapClass::clear()
{
	mapElement.clear();
}

void MapClass::init()
{
	mapTexture = vector<string>({ "block1.png", "block2.png" });
}

//重置
void MapClass::reset()
{
	if (mapElement.size()) {
		auto tran = origin - mapElement.front()->getPosition();
		for (auto & element : mapElement) {
			element->getPhysicsBody()->setVelocity(Vec2::ZERO);
			element->setPosition(element->getPosition() + tran);
		}
	}
	
}

//移动
void MapClass::move()
{
	if (mapElement.size()) {
		for (auto & element : mapElement) {
			element->getPhysicsBody()->setVelocity(Vec2(-GameMinSpeed, 0));
		}
	}
}

void MapClass::stop()
{
	if (mapElement.size()) {
		CCLOG("test");
		for (auto & element : mapElement) {
			element->getPhysicsBody()->setVelocity(Vec2::ZERO);
		}
	}
}
