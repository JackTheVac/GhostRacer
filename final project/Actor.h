#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class actorBase: public GraphObject{

public:

	actorBase(int imgID, int dir, int size, int depth, int xspeed, int yspeed, int healthpoint, double x_pos, double y_pos, bool alive, bool avoid, StudentWorld* ptr);
	
	virtual void doSomething() = 0;

	//accessor
	double getXSpeed() const { return m_xspeed; }
	double getYSpeed() const { return m_yspeed; }
	int getHealth() const { return m_health; }
	bool getAvoidance() const { return m_avoidance; }
	bool getAliveStatus() const { return m_alive; }
	StudentWorld* getSWptr() const { return m_ptr; }


	//mutator
	void changeHealth(int new_health) { m_health = new_health; if (m_health <= 0) changeAliveStatus(false); }
	void changeAliveStatus(bool status) { m_alive = status; }
	void changeYspeed(double speed) { m_yspeed = speed; }
	void changeXspeed(double speed) { m_xspeed = speed; }
	void changeInYalg(double& new_x, double& new_y, actorBase* ptr);
	

	//helper function
	bool isOffScreen(actorBase* ptr);
	bool isOverLap(StudentWorld* ptr) const;

	//common method
	virtual bool isZombie() { return true; }
	virtual bool isBeing() { return true; }
	virtual bool isWaterInteractive() { return false; }




private:

	//input value
	double m_xspeed;
	double m_yspeed;
	int m_health;
	bool m_avoidance;
	bool m_alive;

	StudentWorld* m_ptr;

	


};

class movable
{
public:
	movable(int plan) : m_plan(plan) {}

	//accssor
	int getPlan() const { return m_plan; }

	//mutator
	void changePlan(int num){ m_plan = num; }
	virtual void determineNextPlan(actorBase* ptr);

private:
	int m_plan;

};



class Ghost_Racer : public actorBase {

public:

	Ghost_Racer(int grID, bool aliveState, int dir, double x_coord, double y_coord, int xspeed, int yspeed, int numOfSprays, int health, StudentWorld* ptr);

	virtual void doSomething();

	//helper
	void getHurt(int damage);  //see if damage killed  GR and if set alivestatus to false
	

    //mutator
	void changeNumofSprays(int num) { m_numOfSprays = num; }
	

	//accessor
	int get_numOfSprays() const { return m_numOfSprays; }
	




private:

	int m_numOfSprays;



};



class BorderLine : public actorBase, public movable {
public:

	BorderLine(int imgID, int xspeed, int yspeed, double x_coord, double y_coord, bool aliveState, StudentWorld* ptr);

	//differentiated common method
	//virtual bool isActive() { return false; }  
	virtual bool isBeing() { return false; }
	virtual bool isZombie() { return false; }
	
	virtual void doSomething();

};


class HumanPedestrians : public actorBase, public movable
{
public:
	HumanPedestrians(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int plan2move, int xspeed, int yspeed, bool aliveState, int health, bool avoid, StudentWorld* ptr);

	//differentiated common method
	virtual bool isZombie() { return false; }
	
	virtual bool isWaterInteractive() { return true; }
	
	virtual void doSomething();
};



//Zombie Ped
class ZombiePedestrian : public actorBase, public movable
{
public:
	ZombiePedestrian(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int plan2move, int xspeed, int yspeed, bool aliveState, int health, bool avoid, int tick, StudentWorld* ptr);


	virtual bool isWaterInteractive() { return true; }
	
	virtual void doSomething();

	//accessor
	int getTicks() const { return m_numOfTicks; }

	//mutator
	void changeTicks(int howMuch) { m_numOfTicks = howMuch; }


private:
	int m_numOfTicks;
};


//Zombie Cab
class ZombieCab : public actorBase, public movable
{
public:
	ZombieCab(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int plan2move, int xspeed, int yspeed, bool alivestate, int health, bool avoid, StudentWorld* ptr, int leftlane, int rightlane);

	virtual void doSomething();

	//accessor
	bool getIsDamageGR() const { return m_isDamageGR; }
	int getLeftLane() const { return m_leftlane; }
	int getRightLane() const { return m_rightlane; }


	//mutator
	void changeIsDamageGR(bool change) { m_isDamageGR = change;  }
	virtual void determineNextPlan(actorBase* ptr);   //different from movable's DNP, so redefined it


	//differentiaed common method
	//virtual bool isGood() { return false; }
	virtual bool isBeing() { return false; }
	virtual bool isWaterInteractive() { return true; }

	


private:
	bool m_isDamageGR;

	int m_leftlane;
	int m_rightlane;

	
	

};


//oil slick
class OilSlick : public actorBase
{
public:
	OilSlick(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr);

	virtual void doSomething();

	//different method
	virtual bool isZombie() { return false; }
	virtual bool isBeing() { return false; }

	//helper
	void spinGR();  //spins the ghost racer using the algorithm

};



//healing goodie
class HealingGoodie : public actorBase
{
public:
	HealingGoodie(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr);

	virtual void doSomething();

	//different method
	virtual bool isZombie() { return false; }
	virtual bool isBeing() { return false; } 
	virtual bool isWaterInteractive() { return true; }

};




//water
class HolyWaterGoodie : public actorBase
{
public:
	HolyWaterGoodie(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr);

	virtual void doSomething();

	//different method
	virtual bool isZombie() { return false; }  
	virtual bool isBeing() { return false; } 
	virtual bool isWaterInteractive() { return true; }
};





//Soul
class SoulGoodie : public actorBase
{
public:
	SoulGoodie(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr);

	virtual void doSomething();

	//different method
	virtual bool isZombie() { return false; }
	virtual bool isBeing() { return false; }

};




//Water Projectile
class WaterProjectile : public actorBase
{
public:
	WaterProjectile(int imgID, double xcoord, double ycoord, int dir, int size, int depth, bool avoid, StudentWorld* ptr);

	virtual void doSomething();

	//different method
	virtual bool isZombie() { return false; }

	//accessor
	int getPixels() const { return m_pixels; }



	//mutator
	void changePixels(int changeTo) { m_pixels = changeTo; }


private:
	int m_pixels;

	//bool m_isActivate;

};


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp






#endif // ACTOR_H_
