#pragma once

#ifndef gameShared_H
#define gameShared_H

const double GameMaxSpeed = 400;
const double GameMinSpeed = 150;
const double SpeedChangeRate = 10;
enum SpriteTag {
	ChickenTag = 1,
	BlockTag = 2,
	GoldTag = 100,
	ShoeTag = 101,
	RocketTag = 102,
	MedicineTag = 103,
	ExplosionTag = 200
};
#endif // ! gameShared_H
