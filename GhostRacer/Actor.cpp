#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor.h

static const double PI = 4 * atan(1.0);

actorBase::actorBase(int imgID, int dir, int size, int depth, int xspeed, int yspeed, int healthpoint, double x_pos, double y_pos, bool alive, bool avoid, StudentWorld* ptr)
	:GraphObject(imgID, x_pos, y_pos, dir, size, depth), m_xspeed(xspeed), m_yspeed(yspeed), m_health(healthpoint), m_alive(alive), m_avoidance(avoid), m_ptr(ptr)
{
}

bool actorBase::isOverLap(StudentWorld *ptr) const
{
	double delta_x = abs((this->getX() - ptr->getGRptr()->getX()));
	double delta_y = abs((this->getY() - ptr->getGRptr()->getY()));
	double sumOfRadius = this->getRadius() + ptr->getGRptr()->getRadius();

	if (delta_x < sumOfRadius * 0.25 && delta_y < sumOfRadius * 0.6)
		return true;

	else
		return false;
}

bool actorBase::isOffScreen(actorBase* ptr)
{
	if (ptr->getX() < 0 || ptr->getY() < 0 || ptr->getX() > VIEW_WIDTH || ptr->getY() > VIEW_HEIGHT)
	{
		ptr->changeAliveStatus(false);
		return true;
	}

	return false;

}

void actorBase::changeInYalg(double& new_x, double& new_y, actorBase* ptr)
{
	double vert_speed = ptr->getYSpeed() - ptr->getSWptr()->getGRptr()->getYSpeed();
	double hori_speed = ptr->getXSpeed();

	new_y = ptr->getY() + vert_speed;
	new_x = ptr->getX() + hori_speed;



}

void movable::determineNextPlan(actorBase* ptr)
{
	int tempxspeed = 0;

	for (; tempxspeed == 0; tempxspeed = randInt(-3, 3))
	{
	}

	ptr->changeXspeed(tempxspeed);
	changePlan(randInt(4, 32));

	if (ptr->getXSpeed() < 0)
		ptr->setDirection(180);
	else
		ptr->setDirection(0);

	return;


}


Ghost_Racer::Ghost_Racer(int grID, bool aliveState, int dir, double x_coord, double y_coord, int xspeed, int yspeed, int numOfSprays, int health, StudentWorld* ptr)
	:actorBase(grID, dir, 4, 0, xspeed, yspeed, health, x_coord, y_coord, aliveState, true, ptr), m_numOfSprays(numOfSprays)
{
}

void Ghost_Racer::getHurt(int damage)
{

	int new_health = getHealth() - damage;
	changeHealth(new_health);

	if (getHealth() <= 0)
	{
		changeAliveStatus(false);
		getSWptr()->playSound(SOUND_PLAYER_DIE);
	}




}


void Ghost_Racer::doSomething()
{
	if (getAliveStatus() == false)
		return;


	//check if GR hit left border
	if (getX() <= getSWptr()->getLE())
	{
		if (getDirection() > 90)
			getHurt(10);
		
		setDirection(82);
		
		getSWptr()->playSound(SOUND_VEHICLE_CRASH);

	}
	else if (getX() >= getSWptr()->getRE())
	{
		if (getDirection() < 90)
			getHurt(10);

		setDirection(98);
		
		getSWptr()->playSound(SOUND_VEHICLE_CRASH);

	}
	else
	{
		int key;
		getSWptr()->getKey(key);
			//holy water
			if (key == KEY_PRESS_SPACE && get_numOfSprays() >= 1)
			{
				getSWptr()->changeAddWater(true);
				getSWptr()->playSound(SOUND_PLAYER_SPRAY);
				changeNumofSprays(get_numOfSprays() - 1);

			}
			else if (key == KEY_PRESS_LEFT && getDirection() < 114)
			{
				int x = getDirection() + 8;
				setDirection(x);

			}
			else if (key == KEY_PRESS_RIGHT && getDirection() > 66)
			{
				int x = getDirection() - 8;
				setDirection(x);
			}
			else if (key == KEY_PRESS_UP && getYSpeed() < 5)
			{
				changeYspeed(getYSpeed() +1);
			}
			else if (key == KEY_PRESS_DOWN && getYSpeed() > -1)
			{
				changeYspeed(getYSpeed() - 1);
			}


	}


	
	

	//GR Movement Algorithm

	double max_shift_per_tick = 4.0;
	double direction = getDirection() * 2.0 * PI / 360;
	
	double delta_x = (cos(direction)) * max_shift_per_tick ;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);

}








//BorderLine

BorderLine::BorderLine(int borderID, int xspeed, int yspeed, double x_coord, double y_coord, bool aliveState, StudentWorld* ptr)
	:actorBase(borderID, 0, 2, 1, xspeed, yspeed, -999, x_coord, y_coord, aliveState, false, ptr), movable(0)
{
}


void BorderLine::doSomething()
{
	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);


	moveTo( new_x , new_y);


	if (isOffScreen(this))
		return;

}

//Human Ped

HumanPedestrians::HumanPedestrians(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int plan2move, int xspeed, int yspeed, bool aliveState, int health, bool avoid, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, health, xcoord, ycoord, aliveState, avoid, ptr), movable(plan2move)
{
}

void HumanPedestrians::doSomething()
{
	if (getAliveStatus() == false)
		return;

	if (isOverLap(getSWptr()) == true)
	{
		getSWptr()->getGRptr()->changeAliveStatus(false);
		return;
	}
		

	//human ped movement algorithm
	double new_y;
	double new_x;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);


	if (isOffScreen(this))
		return;


	//determine next plan
	changePlan(getPlan() - 1);


	if (getPlan() > 0)
		return;
	else
	{
		determineNextPlan(this);
	}

}

//Zombie Ped
ZombiePedestrian::ZombiePedestrian(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int plan2move, int xspeed, int yspeed, bool aliveState, int health, bool avoid, int ticks, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, health, xcoord, ycoord, aliveState, avoid, ptr), movable(plan2move), m_numOfTicks(ticks)
{
}


void ZombiePedestrian::doSomething()
{
	if (getAliveStatus() == false)
		return;

	if (isOverLap(getSWptr()) == true)
	{
		getSWptr()->getGRptr()->getHurt(5);

		//damaging zombie
		changeHealth(getHealth() - 2);
		changeAliveStatus(false);
		getSWptr()->playSound(SOUND_PED_DIE);
		getSWptr()->increaseScore(150);
		return;


	}

	//check if zombie is near GR
	if (abs((getX() - getSWptr()->getGRptr()->getX())) <= 30  &&  getY() > getSWptr()->getGRptr()->getY() )
	{
		setDirection((3 / 2) * PI);

		if ((getX() - getSWptr()->getGRptr()->getX()) < 0)
		{
			changeXspeed(1);
		}
		else if ((getX() - getSWptr()->getGRptr()->getX()) > 0)
		{
			changeXspeed(-1);
		}
		else
			changeXspeed(0);

		changeTicks(getTicks() - 1);

		if (getTicks() <= 0)
		{
			getSWptr()->playSound(SOUND_ZOMBIE_ATTACK);
			changeTicks(20);
		}

	}

	

	//movement
	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);


	if (isOffScreen(this))
		return;


	//next movement plan
	if (getPlan() > 0)
	{
		changePlan(getPlan() - 1);
	}
	else
	{
		determineNextPlan(this);
	}

	return;

	//final movment plan




}



//ZombieCab class
ZombieCab::ZombieCab(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int plan2move, int xspeed, int yspeed, bool aliveState, int health, bool avoid, StudentWorld* ptr, int leftlane, int rightlane)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, health, xcoord, ycoord, aliveState, avoid, ptr), movable(plan2move), m_isDamageGR(false), m_leftlane(leftlane), m_rightlane(rightlane)
{
}

void ZombieCab::determineNextPlan(actorBase* ptr)
{


	changePlan(randInt(4, 32));
	
	int tempyspeed = randInt(-2, 2);
	ptr->changeYspeed(ptr->getYSpeed() + tempyspeed);
	




}


void ZombieCab::doSomething()
{
	if (getAliveStatus() == false)
		return;

	

	if (isOverLap(getSWptr()))
	{
		if (getIsDamageGR() == false)
		{
			getSWptr()->playSound(SOUND_VEHICLE_CRASH);
			getSWptr()->getGRptr()->getHurt(20);

			double cur_x = getX();
			double gr_cur_x = getSWptr()->getGRptr()->getX();

			if (cur_x <= gr_cur_x)
			{
				changeXspeed(-5);
				setDirection(120 + randInt(0, 19));
			}

			if (cur_x > gr_cur_x)
			{
				changeXspeed(5);
				setDirection(60 - randInt(0, 19));
			}

			changeIsDamageGR(true);
		}
	}

	

	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);
	if (isOffScreen(this))
	{
		
		changeAliveStatus(false);
		return;
	}



	if (getYSpeed() > getSWptr()->getGRptr()->getYSpeed() && getSWptr()->determineifinfront(this))
	{
		
		changeYspeed(getYSpeed() - 0.5);
		return;
	} 

	if (getYSpeed() <= getSWptr()->getGRptr()->getYSpeed() && getSWptr()->determineifBack(this)) 
	{
		
		changeYspeed(getYSpeed() + 0.5);
		return;
	}

	
	changePlan(getPlan() - 1);

	if (getPlan() > 0)
		return;
	else
	{
		determineNextPlan(this);
	}

	



}



//Oil Slick
OilSlick::OilSlick(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, -999, xcoord, ycoord, true, avoid, ptr)
{
}

void OilSlick::doSomething()
{

	//determining new position
	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);

	//see if it's offscreen
	if (isOffScreen(this))
		return;

	if (isOverLap(getSWptr()))
	{
		getSWptr()->playSound(SOUND_OIL_SLICK);
		spinGR();
		
	}
	


}


void OilSlick::spinGR()
{
	double randClock = randInt(0, 1);
	if (randClock == 0)
		randClock = 1;
	else
		randClock = -1;
	double newDirection = randInt(5, 20) * randClock;
	double GR_direction = getSWptr()->getGRptr()->getDirection();


	

	if (GR_direction + newDirection < 60 || GR_direction + newDirection > 120)
		return;
	else
	{
		getSWptr()->getGRptr()->setDirection(GR_direction + newDirection);
		return;
	}

}


//Healing Goodie
HealingGoodie::HealingGoodie(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, -999, xcoord, ycoord, true, avoid, ptr)
{
}

void HealingGoodie::doSomething()
{
	//determine new position
	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);


	if (isOffScreen(this))
		return;

	if (isOverLap(getSWptr()))
	{
		getSWptr()->getGRptr()->changeHealth(getSWptr()->getGRptr()->getHealth() + 10);
		changeAliveStatus(false);
		getSWptr()->playSound(SOUND_GOT_GOODIE);
		getSWptr()->increaseScore(250);
	}

}



//holy water Goodie
HolyWaterGoodie::HolyWaterGoodie(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, -999, xcoord, ycoord, true, avoid, ptr)
{
}


void HolyWaterGoodie::doSomething()
{
	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);

	if (isOffScreen(this))
		return;

	if (isOverLap(getSWptr()))
	{
		int cur_num = getSWptr()->getGRptr()->get_numOfSprays();
		getSWptr()->getGRptr()->changeNumofSprays(cur_num + 10);
		changeAliveStatus(false);
		getSWptr()->playSound(SOUND_GOT_GOODIE);
		getSWptr()->increaseScore(50);
	}

}


//soul goodie
SoulGoodie::SoulGoodie(int imgID, double xcoord, double ycoord, int dir, int size, int depth, int xspeed, int yspeed, bool avoid, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, xspeed, yspeed, -999, xcoord, ycoord, true, avoid, ptr)
{
}

void SoulGoodie::doSomething()
{
	double new_x;
	double new_y;
	changeInYalg(new_x, new_y, this);
	moveTo(new_x, new_y);


	if (isOffScreen(this))
		return;


	if (isOverLap(getSWptr()))
	{
		int cur_soul = getSWptr()->getRemainingSouls();
		getSWptr()->changeSoul(cur_soul - 1);
		changeAliveStatus(false);
		getSWptr()->playSound(SOUND_GOT_SOUL);
		getSWptr()->increaseScore(100);
	}

	setDirection(getDirection() + 10);
	

}

//Projectile

WaterProjectile::WaterProjectile(int imgID, double xcoord, double ycoord, int dir, int size, int depth, bool avoid, StudentWorld* ptr)
	:actorBase(imgID, dir, size, depth, 0, 0, 0, xcoord, ycoord, true, avoid, ptr), m_pixels(160)
{
}

void WaterProjectile::doSomething()
{
	if (getAliveStatus() == false)
		return;

	bool proceed = true;

	if (getSWptr()->isOverLapwithWater(this) == true)
	{

		actorBase* other = getSWptr()->getCheckWith();


		if (other->isWaterInteractive())
		{
			proceed = false;

			if (other->isZombie()) //for zombie
			{
				if (other->isBeing())//for zombie ped
				{

					other->changeHealth(other->getHealth() - 1);

					if (other->getHealth() <= 0)
					{
						other->changeAliveStatus(false);
						getSWptr()->playSound(SOUND_PED_DIE);
						if (other->isOverLap(getSWptr()) == false) //adding heal
						{
							if (randInt(0, 4) == 1)
							{
								getSWptr()->changeAddHeal(true);
								getSWptr()->changeHealPos(other->getX(), other->getY());
							}
						}
						getSWptr()->increaseScore(150);
					}
				}
				else //for zombie cab
				{
					other->changeHealth(other->getHealth() - 1);

					if (other->getHealth() <= 0)
					{
						other->changeAliveStatus(false);
						getSWptr()->playSound(SOUND_VEHICLE_DIE);
						if (randInt(0, 4) == 1)
						{
							getSWptr()->changeAddOS(true);
							getSWptr()->changeOSpos(other->getX(), other->getY());
						}
						getSWptr()->increaseScore(200);



					}
					else
					{
						getSWptr()->playSound(SOUND_VEHICLE_HURT);
					}

				}

			}
			else //for nonzombie
			{
				if (other->isBeing()) //HUMAN PED
				{
					other->changeXspeed(other->getXSpeed() * (-1));
				
					double cur_dir = other->getDirection();
					if (cur_dir == 0)
						other->setDirection(180);
					else
						other->setDirection(0);

					
					
				
					getSWptr()->playSound(SOUND_PED_HURT);
				}
				else //both HEALING GOODIES AND HOLY WATER WILL BE DESTROYED
				{
					other->changeAliveStatus(false);
				}
			}
		}

		
	
	}

	if (proceed == false)
	{
		changeAliveStatus(false);
		return;
	}
	
	
	
	
	moveForward(SPRITE_HEIGHT);
	changePixels(getPixels() - SPRITE_HEIGHT);


	if (isOffScreen(this))
		return;


	if (getPixels() <= 0)
		changeAliveStatus(false);

}

