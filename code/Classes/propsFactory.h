#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "math\MathUtil.h"

#include "GameShared.h"

USING_NS_CC;
using namespace std;

class propsFactory
{
public:
	
	void setPhysicsWorld(PhysicsWorld* physicWorld);
	void setLayer(Layer *layer);
	static propsFactory* getInstance();
	//��������ĵ���
	Vector<Sprite*> createRandomProps(Sprite* block, Layer* a);
	//�������
	Sprite* createGold(Vec2& pos, Layer* placeLayer = nullptr);
	//����Ь��
	Sprite* createShoe(Vec2& pos, Layer* placeLayer = nullptr);
	//�������
	Sprite* createRocket(Vec2& pos, Layer* placeLayer = nullptr);
	//��������ҩ
	Sprite* createMedicine(Vec2& pos, Layer* placeLayer = nullptr);
	//�Ƴ��뿪�����ĵ���
	void removeIfOut();
	//�Ƴ����е���
	void removeAll();
	//ֹͣ���е��ߵ��ƶ�
	void stopAll();
	//�ı�����ƶ����ٶ�
	void speedUp(double multi);
	//�Ƴ�ĳ���ض��ĵ���
	void removeProps(Sprite* obj);
	//���ص��߾�����б�
	list<Sprite*> getPropsList();
	//���õ��ߵ��ٶ�
	void resetSpeed();
	//���ý�����ɵĸ���
	void resetGoldPosibilityRange();
	//����д���ɵĸ���
	void resetShoePosibilityRange();
	//����ҩ���ɵĸ���
	void resetMedicinePosibilityRange();
	//����
	void resetAll();

private:
	propsFactory();
	//��ͬ�������ɵĳ�ʼ����
	const double	oriGoldPossibilityRange = 0.4,
					oriShoePossibilityRange = 0.45,
					oriMedicinePossibilityRange = 0.5;
	
	//��ǰ�������ɵĸ���
	double	shoePossibilityRange,
		goldPossibilityRange,
		medicinePossibilityRange;
	//��ǰ���ߵ��ٶ�
	double	speed,
			medicineLimitSpeed = GameMinSpeed + 30;
	PhysicsWorld * m_physicsWorld = NULL;
	Layer* defaultLayer = NULL;
	Sprite* createProps(Vec2& pos , const char path[] , Layer* placeLayer );
	static propsFactory* instance;
	//���߾����б�
	list<Sprite*> propsList;
};

