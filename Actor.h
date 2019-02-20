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
	bool setDead() { m_is_dead = true; }
	

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

	}
	virtual void doSomething();
	
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




#endif // ACTOR_H_
