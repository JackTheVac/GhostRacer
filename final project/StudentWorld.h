#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    //accessor
    int getLE() const { return LEFT_EDGE; }
    int getRE() const { return RIGHT_EDGE;}
    int getRemainingSouls() const{ return m_totalSouls; }

    double getXforOS() const { return m_xforOS; }
    double getYforOS() const { return m_yforOS; }

    bool getaddOS() const { return m_addOS; }
    bool getaddWater() const { return m_addWater; }

   
    

    Ghost_Racer* getGRptr() const { return grptr; }
    actorBase* getCheckWith() const { return m_checkwithptr; }


    //needed?
   
    bool getAddHeal() const { return m_addheal; }
    double getXheal() const { return m_xforheal; }
    double getYheal() const { return m_yforheal; }


    //mutator
    void changeSoul(int souls) { m_totalSouls = souls; }            //updating the total amount of souls needed
    void changeAddWater(bool changeTo) { m_addWater = changeTo; }   //change the condition of if a water is needed
    void changeCheckwith(actorBase* ptr) { m_checkwithptr = ptr; }  //changing the ptr that needs to be checked by each actor's dosmtg

    void changeAddHeal(bool changeTo) { m_addheal = changeTo; }                                                 //change the condition of if a heal is needed to be dropped
    void changeHealPos(double changeTox, double changetoY) { m_xforheal = changeTox; m_yforheal = changetoY; }  //setting the heal's x and y's when it is spawned
    void changeAddOS(bool changeTo) { m_addOS = changeTo; }                                                     //change the condition of if a oilslick is needed to be spawned
    void changeOSpos(double changeTox, double changetoY) { m_xforOS = changeTox; m_yforOS = changetoY; }        //setting OS's x and y



    //helper
    bool isOverLapwithWater(actorBase* check);                      //check if anything within the container that overlaps with projectile
    bool determineZombieCab();                                      //determine if ZombieCab can be spawned based on raod condition
    actorBase* determineClosestToBottom(int left, int right);       //helper function for DetermineZombieCab()
    actorBase* determineClosestToTop(int left, int right);          //helper function for DetermineZombieCab()
    bool determineifinfront(ZombieCab* ptr);                        //helper function for DetermineZombieCab()
    bool determineifBack(ZombieCab* ptr);                           //helper function for DetermineZombieCab()
   





   


   
    
    

    

private:


    Ghost_Racer* grptr;
    list<actorBase*> actorList; 
    actorBase* m_checkwithptr;

    //useful constants
    double y_of_last_white;
    double N_yellow = VIEW_HEIGHT / SPRITE_HEIGHT;
    double M_white = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

    //stats
    int m_totalSouls;
    int m_bonus_point;
    int m_total_point;


    //water properties
    bool m_addWater;
   
  

    //heal properties
    bool m_addheal;
    double m_xforheal;
    double m_yforheal;

    //OS Properties
    bool m_addOS;  
    double m_xforOS;
    double m_yforOS;


    //Cab properties
    double m_yforCab;
    double m_xforCab;
    int m_initYspeed;
    int m_leftlane;
    int m_rightlane;



};

#endif // STUDENTWORLD_H_
