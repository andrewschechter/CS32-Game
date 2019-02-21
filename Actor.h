#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

  //ACTOR ABSTRACT BASE CLASS CLASS DEFINITION
class Actor : public GraphObject
{
  public:
	Actor(int imageID, double startX, double startY, Direction dir, StudentWorld* world,
		  bool allow_overlap = false, bool can_use_exit = false, bool can_die = false, int depth = 0, double size = 1.0)
	:GraphObject(imageID, startX, startY, dir, depth, size)
	{
		m_world = world;
		m_allow_overlap = allow_overlap;
		m_can_use_exit = can_use_exit;
		m_can_die = can_die;
		m_is_dead = false;
	}
	
	virtual void doSomething() = 0;
	 
	  //accessors
	StudentWorld* getWorld() const { return m_world; }
	bool allowsOverlap() const { return m_allow_overlap; }
	bool canUseExits() const { return m_can_use_exit; }
	bool canDie() const { return m_can_die; }
	bool isDead() const { return m_is_dead; }
	
	  //mutators
	void setDead() { m_is_dead = true; }
	

  private:
	StudentWorld* m_world;
	bool m_allow_overlap;
	bool m_can_use_exit;
	bool m_can_die;
	bool m_is_dead;
};
 

  //TYPES OF ACTORS
class Penelope : public Actor
{
  public:
	Penelope(int start_x, int start_y, StudentWorld* world)
	:Actor(IID_PLAYER, start_x, start_y, right, world, false, true, true)
	{
		//Actors:
		// allow overlap = false
		// can use exits = true
		// can die = true
	}
	virtual void doSomething();
	
};


class Citizen : public Actor
{
  public:
	Citizen(int start_x, int start_y, StudentWorld* world)
		:Actor(IID_CITIZEN, start_x, start_y, right, world, false, true, true)
	{
		//Citizen:
		// allow overlap = false
		// can use exits = true
		// can die = true

	}
	virtual void doSomething() { return; }


};


class Wall : public Actor
{
  public:
	Wall(int start_x, int start_y, StudentWorld* world)
	:Actor(IID_WALL, start_x, start_y, right, world)
	{
	  //Walls:
	  // DEFAULT PARMETERS ACCEPTED
	  // allow overlap = false
	  // can use exits = false
	  // can die = false
	}
	virtual void doSomething() { return; } //walls don't do anything

};


class Exit : public Actor
{
  public:
	Exit(int start_x, int start_y, StudentWorld* world)
	:Actor(IID_EXIT, start_x, start_y, right, world, true, false, false, 1)
	{
	  //Exits:
	  // allow overlap = true
	  // can use exits = false
	  // can die = false
	}
	virtual void doSomething();

};

class Pit : public Actor
{
  public:
	Pit(int start_x, int start_y, StudentWorld* world)
	:Actor(IID_PIT, start_x, start_y, right, world, true, false, false)
	{
	  //Pits:
	  // allow overlap = true
	  // can use exits = false
	  // can die = false
	}
	virtual void doSomething();

};


  //GOODIE ABSTRACT BASE CLASS
class Goodie : public Actor
{
  public:
	  Goodie(int imageID, int start_x, int start_y, StudentWorld* world)
	  :Actor(imageID, start_x, start_y, right, world, true, false, true, 1)
	  {
		  //Goodies:
		  // allow overlap = true
		  // can use exits = false
		  // can die = true
	  }
	  virtual void doSomething() = 0;
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
