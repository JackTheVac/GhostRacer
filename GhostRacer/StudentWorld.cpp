
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;



GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    


}

int StudentWorld::init()
{

    //resetting stats/initialize stat
    m_totalSouls = 2 * getLevel() + 5;
    m_bonus_point = 5000;


    //initialize a  grptr to a new ghost racer
    grptr = new Ghost_Racer(IID_GHOST_RACER, true,  90 , 128, 32, 0, 0, 10, 100, this);

  
    //resetting check point
    m_checkwithptr = nullptr;
        

    
 
    //resetting Water
    m_addWater = false;

    //resetting OS
    m_addOS = false;

    //resetting addwater
    m_addheal = false;

 

  

    

    //constructing left yellow border
    for (int i = 0; i < N_yellow; i++)
    {
        int j = i * SPRITE_HEIGHT;
        BorderLine* BL = new BorderLine(IID_YELLOW_BORDER_LINE, 0 ,-4, LEFT_EDGE, j, true, this);
        actorList.push_back(BL);
        BL = new BorderLine(IID_YELLOW_BORDER_LINE, 0,-4, RIGHT_EDGE, j, true, this);
        actorList.push_back(BL);
    }

  

    for (int i = 0; i < M_white; i++)
    {
        int j = i * (4 * SPRITE_HEIGHT);
        BorderLine* BL = new BorderLine(IID_WHITE_BORDER_LINE,0, -4, LEFT_EDGE + ROAD_WIDTH / 3.0, j, true, this);
        actorList.push_back(BL);
        BL = new BorderLine(IID_WHITE_BORDER_LINE,0, -4, RIGHT_EDGE - ROAD_WIDTH / 3.0, j, true, this);
        actorList.push_back(BL);
        
        y_of_last_white = j;

        

    }



 



   

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
   
    //each tick = -1 bonus
    m_bonus_point--;
   


   //gr does something first
    grptr->doSomething();



    list<actorBase*>::iterator p = actorList.begin();
    for (p; p != actorList.end(); p++)
    {

        if ((*p)->getAliveStatus() == true)
        {
            (*p)->doSomething();         
        }

       

        //check if gr died
        if (grptr->getAliveStatus() == false)
        {
            
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        


        if (getRemainingSouls() == 0)
        {
            increaseScore(m_bonus_point);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }


    //deleting dead actors
    for (p = actorList.begin() ; p != actorList.end();)
    {
        if ((*p)->getAliveStatus() == false)
        {
            delete (*p);
            p = actorList.erase(p);
        }
        else
            p++;

    }
    


    //new actor for border
    y_of_last_white = y_of_last_white - 4 - grptr->getYSpeed();

    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT*1.0;
    double delta_y = new_border_y - y_of_last_white;
    if (delta_y >= SPRITE_HEIGHT)
    {
        BorderLine* BL = new BorderLine(IID_YELLOW_BORDER_LINE, 0, -4, LEFT_EDGE, new_border_y, true, this);
        actorList.push_back(BL);
        BL = new BorderLine(IID_YELLOW_BORDER_LINE, 0, -4, RIGHT_EDGE, new_border_y, true, this);
        actorList.push_back(BL);
    }
    if (delta_y >= 4.0 * SPRITE_HEIGHT)
    {
        BorderLine* BL = new BorderLine(IID_WHITE_BORDER_LINE, 0, -4,  LEFT_EDGE + ROAD_WIDTH / 3.0, new_border_y, true, this);
        actorList.push_back(BL);
        BL = new BorderLine(IID_WHITE_BORDER_LINE, 0, -4, RIGHT_EDGE - ROAD_WIDTH / 3.0, new_border_y, true, this);
        actorList.push_back(BL);
       
        y_of_last_white = new_border_y;
    }


    //add new Human Ped
    int ChanceHumanPed = max(200 - getLevel() * 10, 30);
    int PedInt = randInt(0, ChanceHumanPed - 1);
    if (PedInt == 0)
    {
        HumanPedestrians* HP = new HumanPedestrians(IID_HUMAN_PED, randInt(0, VIEW_WIDTH), VIEW_HEIGHT, 0, 2, 0, 0, 0, -4, true, 2, true, this);
        actorList.push_back(HP);
    }

    //add new Zombie Ped
    int ChanceZombiePed = max(100 - getLevel() * 10, 20);
    int ZomPed = randInt(0, ChanceZombiePed - 1);
    if (ZomPed  == 0)
    {
        ZombiePedestrian* ZP = new ZombiePedestrian(IID_ZOMBIE_PED, randInt(0, VIEW_WIDTH-1), VIEW_HEIGHT, 0, 3, 0, 0, 0, -4, true, 2, true, 0, this);
        actorList.push_back(ZP);
    }

    //determining ZombieCabs
    int chanceVehicle = max(100 - getLevel() * 10, 20);
    int ZC = randInt(0, chanceVehicle - 1);
    if (ZC == 0)
    {
        if (determineZombieCab() == true)
        {
            actorBase* ZC = new ZombieCab(IID_ZOMBIE_CAB, m_xforCab, m_yforCab, 90, 4, 0, 0, 0, m_initYspeed, true, 3, true, this, m_leftlane, m_rightlane);
            actorList.push_back(ZC);
        }
    }

    //add new oil slick
    int chanceOilSlick = max(150 - getLevel() * 10, 40);
    int OilS = randInt(0, chanceOilSlick - 1);
    if (OilS == 0 )
    {
        OilSlick* OS = new OilSlick(IID_OIL_SLICK, randInt(getLE(), getRE()), VIEW_HEIGHT, 0, randInt(2, 5), 2, 0, -4, false, this);
        actorList.push_back(OS);
    }

    //add new HolyWater
    int chanceHolyWater = 100 + 10 * getLevel();
    int HolyW = randInt(0, chanceHolyWater - 1);
    if (HolyW== 0)
    {
        HolyWaterGoodie* HW = new HolyWaterGoodie(IID_HOLY_WATER_GOODIE, randInt(getLE(), getRE()), VIEW_HEIGHT, 90, 2, 2, 0, -4, false, this);
        actorList.push_back(HW);
    }


    //add new soul
    int chanceOfLostSoul = 100;
    int LSG = randInt(0, chanceOfLostSoul - 1);
    if (LSG == 0)
    {
       SoulGoodie* SG = new SoulGoodie(IID_SOUL_GOODIE, randInt(getLE(), getRE()), VIEW_HEIGHT, 0, 4, 2, 0, -4, false, this);
       actorList.push_back(SG);
    }

    //add new projectile
    if (getaddWater() == true)
    {
        WaterProjectile* WP = new WaterProjectile(IID_HOLY_WATER_PROJECTILE, getGRptr()->getX(), getGRptr()->getY(), getGRptr()->getDirection(), 1, 1, false, this);
        actorList.push_back(WP);
    }


    //add newheal
    if (getAddHeal() == true)
    {
        HealingGoodie* HG = new HealingGoodie(IID_HEAL_GOODIE, getXheal(), getYheal(), 0, 1, 2, 0, -4, false, this);
        actorList.push_back(HG);
    }

    if (getaddOS() == true)
    {
        OilSlick* OS = new OilSlick(IID_OIL_SLICK, getXforOS(), getYforOS(), 0, randInt(2, 5), 2, 0, -4, false, this);
        actorList.push_back(OS);
    }


  


    //display stat
    ostringstream stat;
    int live = getLives();
    int score = getScore();
    int level = getLevel();
    int souls = getRemainingSouls();
    int health = grptr->getHealth();
    int water = grptr->get_numOfSprays();


    stat << "Score: " << score << "  ";
    stat << "Lvl: " << level << "  ";
    stat << "Souls2Save: " << souls << "  ";
    stat << "Lives: " << live << "  ";
    stat << "Health: " << health << "  ";
    stat << "Sprays: " << water << "  ";
    stat << "Bonus: " << m_bonus_point << "  ";
 

    string finalStat = stat.str();


    setGameStatText(finalStat);
    

    
  

    
    //readjust
 
    changeAddWater(false); 
    changeAddHeal(false);  
    changeAddOS(false);   
 
    return GWSTATUS_CONTINUE_GAME;

    
}

void StudentWorld::cleanUp()
{

   

    

    delete grptr;

    list<actorBase*>::iterator p = actorList.begin();

    for (; p != actorList.end();)
    {
        delete (*p);
        p = actorList.erase(p);
    }

    actorList.clear();



  

}

StudentWorld::~StudentWorld()
{
    cleanUp();

}

bool StudentWorld::isOverLapwithWater(actorBase* check) 
{

     list<actorBase*>::iterator p = actorList.begin();
     for (p; p != actorList.end(); p++)
     {
         actorBase* curr = (*p);
         if (curr != check)
         {
             double delta_x = abs((check->getX() - curr->getX()));
             double delta_y = abs((check->getY() - curr->getY()));
             double sumOfRadius = check->getRadius() + curr->getRadius();

             if (delta_x < sumOfRadius * 0.25 && delta_y < sumOfRadius * 0.6)
             {
                 changeCheckwith(curr);
                 return true;
             }

         }
         
     }

     return false;

}


//everything here below needed
bool StudentWorld::determineZombieCab()
{
    int randomLane = randInt(0, 2);

    int count = 0;

    while (count != 3)
    {
        
        if (randomLane == 0 ) //LEFT
        {

            
            int left = getLE();
            int right = LEFT_EDGE + ROAD_WIDTH / 3.0;

            m_leftlane = left;
            m_rightlane = right;

            actorBase* test = determineClosestToBottom(left, right);
            if (test == nullptr || test->getY() > (VIEW_HEIGHT / 3))
            {
                if (!(getGRptr()->getX() >= left && getGRptr()->getX() < right))
                {
                    m_xforCab = ROAD_CENTER - ROAD_WIDTH/3;
                    m_yforCab = SPRITE_HEIGHT / 2;
                    m_initYspeed = getGRptr()->getYSpeed() + randInt(2, 4);
                    
                    return true;
                }
                
            }

           

            test = determineClosestToTop(left, right);
            if (test == nullptr || test->getY() < (VIEW_HEIGHT * (2 / 3)))
            {
                
               
                    m_xforCab = ROAD_CENTER - ROAD_WIDTH/3;
                    m_yforCab = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
                    m_initYspeed = getGRptr()->getYSpeed() - randInt(2, 4);

                    return true;
                
                
            }

            count++;

            //determining next lane to check
            
            randomLane = 1;
            
          
            
        }
        else if (randomLane == 1) //middle
        {

            
            int left = LEFT_EDGE + ROAD_WIDTH / 3.0;
            int right = RIGHT_EDGE - ROAD_WIDTH / 3.0;

            m_leftlane = left;
            m_rightlane = right;

            actorBase* test = determineClosestToBottom(left, right);
            if (test == nullptr || test->getY() > (VIEW_HEIGHT / 3))
            {
                if (!(getGRptr()->getX() >= left && getGRptr()->getX() < right))
                {  
                m_xforCab = ROAD_CENTER;
                m_yforCab = SPRITE_HEIGHT / 2;
                m_initYspeed = getGRptr()->getYSpeed() + randInt(2, 4);

                return true;

                }
              
            }

            test = determineClosestToTop(left, right);
            if (test == nullptr || test->getY() < (VIEW_HEIGHT * (2 / 3)))
            {
                
               
                
                    m_xforCab = ROAD_CENTER;
                    m_yforCab = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
                    m_initYspeed = getGRptr()->getYSpeed() - randInt(2, 4);

                    return true;


                
               
            }

            count++;
            
            randomLane = 2;
           

        }
        else if (randomLane == 2 ) //right
        {

            
            int left = RIGHT_EDGE - ROAD_WIDTH / 3.0;
            int right = getRE();

            m_leftlane = left;
            m_rightlane = right;

            actorBase* test = determineClosestToBottom(left, right);
            if (test == nullptr || test->getY() > (VIEW_HEIGHT / 3))
            {

                if (!(getGRptr()->getX() >= left && getGRptr()->getX() < right))
                {
                    m_xforCab = ROAD_CENTER + ROAD_WIDTH / 3;
                    m_yforCab = SPRITE_HEIGHT / 2;
                    m_initYspeed = getGRptr()->getYSpeed() + randInt(2, 4);

                    return true;
                }
               
            }

            test = determineClosestToTop(left, right);
            if (test == nullptr || test->getY() < (VIEW_HEIGHT * (2 / 3)))
            {

               
                    m_xforCab = ROAD_CENTER + ROAD_WIDTH / 3;
                    m_yforCab = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
                    m_initYspeed = getGRptr()->getYSpeed() - randInt(2, 4);

                    return true;
                
                
            }

            count++;
            randomLane = 0;

        }


    }

    return false;


}

actorBase* StudentWorld::determineClosestToBottom(int left, int right)
{

    list <actorBase*>::iterator temp;
    bool proceed = false;

    list<actorBase*>::iterator p = actorList.begin();
    for (p; p != actorList.end(); p++)
    {
        actorBase* curr = (*p);
        if (curr->getAvoidance() && curr->getX() >= left && curr->getX() < right)
        {
            temp = p;
            proceed = true;
            break;
        }
    }

    if (proceed == false)
    {
        
        return nullptr;
    }

    double min_y = (*temp)->getY();
    p++;
    for ( ; p != actorList.end(); p++)
    {
        actorBase* curr = (*p);
        if (curr->getAvoidance() && curr->getX() >= left && curr->getX() < right)
        {
            if (curr->getY() < min_y)
            {
                temp = p;
            }
        }

    }

    return (*temp);




}


actorBase* StudentWorld::determineClosestToTop(int left, int right)
{


    list <actorBase*>::iterator temp;
    bool proceed = false;

    list<actorBase*>::iterator p = actorList.begin();
    for (p; p != actorList.end(); p++)
    {
        actorBase* curr = (*p);
        if (curr->getAvoidance() && curr->getX() >= left && curr->getX() < right)
        {
            temp = p;
            proceed = true;
            break;
        }
    }

    if (proceed == false)
    {
        return nullptr;
    }

    double max_y = (*temp)->getY();
    p++;
    for (; p != actorList.end(); p++)
    {
        actorBase* curr = (*p);
        if (curr->getAvoidance() && curr->getX() >= left && curr->getX() < right)
        {
            if (curr->getY() > max_y)
            {
                temp = p;
            }
        }

    }

    return (*temp);



}


bool StudentWorld::determineifinfront(ZombieCab* ptr)
{

    int cab_left = ptr->getLeftLane();
    int cab_right = ptr->getRightLane();

    int cab_y = ptr->getY();

    list<actorBase*>::iterator p = actorList.begin();
    for (p; p != actorList.end(); p++)
    {

        
        actorBase* curr = (*p);

        if (curr != ptr)
        {
            int cur_x = curr->getX();
            int cur_y = curr->getY();



            if (curr->getAvoidance() == true)
            {

                if (cur_x >= cab_left && cur_x < cab_right)
                {
         

                    if (cur_y - cab_y <= 96 && cur_y >= cab_y)
                    {
                        
                        return true;

                    }
                       
                }
            }

        }
        
      
       
    }

    return false;
}

bool StudentWorld::determineifBack(ZombieCab* ptr)
{

    int cab_left = ptr->getLeftLane();
    int cab_right = ptr->getRightLane();

    int cab_y = ptr->getY();

    list<actorBase*>::iterator p = actorList.begin();
    for (p; p != actorList.end(); p++)
    {
        actorBase* curr = (*p);

        if (curr != ptr)
        {
            int cur_x = curr->getX();
            int cur_y = curr->getY();

            if (curr->getAvoidance() == true)
            {
                if (cur_x >= cab_left && cur_x < cab_right)
                {
                    if (cab_y - cur_y < 96 && cab_y > cur_y)
                    {
                      
                        return true;

                    }
                        
                }
            }
        }
        


    }

    return false;
}