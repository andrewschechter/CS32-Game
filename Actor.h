#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

  //ACTOR ABSTRACT BASE CLASS CLASS DEFINITION
class Actor : public GraphObject
{
  public:
	Actor(int imageID, double startX, double startY, Direction dir, StudentWorld* world, int depth = 0, double size = 1.0)
	:GraphObject(imageID, startX, startY, dir, depth, size)
	{
		m_world = world;
	}
	
	virtual void doSomething() = 0;
	 
	  //accessors
	StudentWorld* getWorld() const { return m_world; }
	virtual bool allowsOverlap() const { return m_allow_overlap; }
	virtual bool canUseExits() const { return m_can_use_exit; }
	virtual bool blocksFlames() const { return m_blocks_flames; }
	virtual bool canFallInPit() const { return m_can_fall_in_pit; }
	virtual bool canTriggerLandmine() const { return m_can_trigger_landmine; }
	virtual bool canTriggerInactiveLandmine() { return m_can_trigger_inactive_landmine; }
	virtual bool canDie() const { return m_can_die; }
	virtual bool canDieByFlames() const { return m_can_die_by_flames; }
	virtual bool isZombie() const { return m_is_zombie; }
	
	bool isDead() const { return m_is_dead; }

	
	  //mutators
	void setDead() { m_is_dead = true; }
	

  private:
	StudentWorld* m_world;
	bool m_allow_overlap = false;
	bool m_can_use_exit = false;
	bool m_can_fall_in_pit = false;
	bool m_can_trigger_landmine = false;
	bool m_can_trigger_inactive_landmine = false;
	bool m_can_die = false;
	bool m_blocks_flames = false;
	bool m_can_die_by_flames = false;
	bool m_is_dead = false;
	bool m_is_zombie = false;
};

 
class Agent : public Actor
{

  public:
	Agent(int ImageID, int start_x, int start_y, StudentWorld* world)
	:Actor(ImageID, start_x, start_y, right, world)
	{

	}

	virtual bool canFallInPit() const { return m_can_fall_in_pit; }
	virtual bool canTriggerLandmine() const { return m_can_trigger_landmine; }
	virtual bool canDie() const { return m_can_die; }
	virtual bool canDieByFlames() const { return m_can_die_by_flames; }


  private:
	bool m_can_fall_in_pit = true;
	bool m_can_trigger_landmine = true;
	bool m_can_die = true;
	bool m_can_die_by_flames = true;
};


class Human : public Agent
{
	
  public:
	Human(int ImageID, int start_x, int start_y, StudentWorld* world)
	:Agent(ImageID, start_x, start_y,  world)
	{
	
	}
	
	virtual bool canUseExits() const { return m_can_use_exit; }

  private:
	bool m_can_use_exit = true;


};


class Penelope : public Human
{
  public:
	Penelope(int start_x, int start_y, StudentWorld* world)
	:Human(IID_PLAYER, start_x, start_y, world)
	{
		
	}
	virtual void doSomething();
	int getFlameCharges() { return getWorld()->getFlameCharges(); }
	int getLandMines() { return getWorld()->getLandmines(); }
	void shootFlame(double src_x, double src_y, Direction dir);


};


class Citizen : public Human
{
  public:
	Citizen(int start_x, int start_y, StudentWorld* world)
		:Human(IID_CITIZEN, start_x, start_y, world)
	{
		//Citizen:
		// allow overlap = false
		// can use exits = true
		// can fall in pit = true
		// can trigger landmine = true
		// can die = true
		// can be damaged by flame = true

	}
	virtual void doSomething();

};


class Zombie : public Agent
{
  public:
	  Zombie(int start_x, int start_y, StudentWorld* world)
	  :Agent(IID_ZOMBIE, start_x, start_y, world)
	  {

	  }
	  virtual void doSomething() = 0;
	  virtual bool isZombie() const { return m_is_zombie; }
  private:
	bool m_is_zombie = true;

};

class Smart_Zombie : public Zombie
{
  public:
	Smart_Zombie(int start_x, int start_y, StudentWorld* world)
	:Zombie(start_x, start_y, world)
	{


    }
	virtual void doSomething() { return; }

};


class Dumb_Zombie : public Zombie
{
  public:
	Dumb_Zombie(int start_x, int start_y, StudentWorld* world)
	:Zombie(start_x, start_y, world)
	{


	}
	virtual void doSomething() { return; }


};


class Wall : public Actor
{
  public:
	Wall(int start_x, int start_y, StudentWorld* world)
	:Actor(IID_WALL, start_x, start_y, right, world)
	{
	}
	virtual void doSomething() { return; } //walls don't do anything

	virtual bool blocksFlames() { return m_blocks_flames; }

  private:
	bool m_blocks_flames = true;
};



class Activating_Object : public Actor
{
	
public:
	Activating_Object(int ImageID, int start_x, int start_y, StudentWorld* world, Direction dir = right, int depth = 0)
	:Actor(ImageID, start_x, start_y, right, world, depth)
	{

	}
	virtual bool allowsOverlap() const { return m_allow_overlap; }


private:
	bool m_allow_overlap = true;
};


class Exit : public Activating_Object
{
  public:
	Exit(int start_x, int start_y, StudentWorld* world)
	:Activating_Object(IID_EXIT, start_x, start_y, world, 1)
	{
	  //Exits:
	  // allow overlap = true
	  // can use exits = false
	  // can fall in pit = false
	  // can trigger landmine = false
	  // can die = false
	  // blocks flames = true;
	}
	virtual void doSomething();
	virtual bool blocksFlames() { return m_blocks_flames; }
  private:
	  bool m_blocks_flames = true;


};


class Pit : public Activating_Object
{
  public:
	Pit(int start_x, int start_y, StudentWorld* world)
	:Activating_Object(IID_PIT, start_x, start_y, world)
	{
	  //Pits:
	  // allow overlap = true
	  // can use exits = false
	  // can fall in pit = false
	  // can trigger landmine = false
	  // can die = false
	  // blocks flames = false (i.e. doesnt block flame at the very least)
	}
	virtual void doSomething();

};

class Landmine : public Activating_Object
{
  public: 
	  Landmine(int start_x, int start_y, StudentWorld* world)
	  :Activating_Object(IID_LANDMINE, start_x, start_y, world, 1)
	  {
		  //Landmines:
          // allow overlap = true
          // can use exits = false
          // can fall in pit = false
		  // can trigger landmine = false
          // can die = true
          // can be damaged be flames = true 
	  }
	  virtual void doSomething();

	  virtual bool canDie() { return m_can_die; }
	  virtual bool canDieByFlames() { return m_can_die_by_flames; }

	  int getTicks() { return m_safety_ticks; }
	  void decTicks() { m_safety_ticks -= 1; }
	  bool isActive() { return m_active; }
	 

  private:
	bool m_active = false;
	int m_safety_ticks = 30;
	bool m_can_die = true;
	bool m_can_die_by_flames = true;
};


// can trigger landmine = true ***********SPECIAL CASE, DOES NOT REQUIRE LANDMINE TO BE ACTIVE*************
class Flame : public Activating_Object
{
  public:
	  Flame(int start_x, int start_y, Direction dir, StudentWorld* world)
	  :Activating_Object(IID_FLAME, start_x, start_y, world, dir, 1)
	  {
		//Flames:
		// allow overlap = true
	    // can use exits = false
		// can fall in pit = false
		// can trigger landmine = true; SPECIAL CASE, DOES NOT REQUIRE LANDMINE TO BE ACTIVE
	    // can die = true
		// can be damaged by flame = false; //but also doesnt block flames??
	  }
	  virtual void doSomething();
	  virtual bool canDie() { return m_can_die; }
	  virtual bool canTriggerInactiveLandmine() { return m_can_trigger_inactive_landmine; }

	  int getTicks() { return m_ticks; }
	  void decTicks() { m_ticks -= 1; }

  private:
	  int m_ticks = 2;
	  bool m_can_die = true;
	  bool m_can_trigger_inactive_landmine = true;

};



  //GOODIE ABSTRACT BASE CLASS
class Goodie : public Activating_Object
{
  public:
	  Goodie(int imageID, int start_x, int start_y, StudentWorld* world)
	  :Activating_Object(imageID, start_x, start_y, world, 1)
	  {
		  //Goodies:
		  // allow overlap = true
		  // can use exits = false
		  // can fall in pit = false
		  // can trigger landmine = false
		  // can die = true
		  // can be damaged by flame = true;
	  }
	  virtual void doSomething() = 0;
	  virtual bool canDieByFlames() { return m_can_die_by_flames; }
	  virtual bool canDie() { return m_can_die; }

private:
	bool m_can_die_by_flames = true;
	bool m_can_die = true;
};

  //TYPES OF GOODIES
class Vaccine_Goodie : public Goodie
{
  public:
	Vaccine_Goodie(int start_x, int start_y, StudentWorld* world)
	:Goodie(IID_VACCINE_GOODIE, start_x, start_y, world)
	{	
	}
	virtual void doSomething();
};


class Gas_Can_Goodie : public Goodie
{
  public: 
	Gas_Can_Goodie(int start_x, int start_y, StudentWorld* world)
	:Goodie(IID_GAS_CAN_GOODIE, start_x, start_y, world)
	{
	}
	virtual void doSomething();
};


class Landmine_Goodie : public Goodie
{
  public:
	Landmine_Goodie(int start_x, int start_y, StudentWorld* world)
	:Goodie(IID_LANDMINE_GOODIE, start_x, start_y, world)
	{
	}
	virtual void doSomething();
};


#endif // ACTOR_H_
