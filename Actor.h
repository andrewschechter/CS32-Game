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
	virtual ~Actor() {};
	virtual void doSomething() = 0;
	
	 
	  //accessors
	StudentWorld* getWorld() const { return m_world; }
	virtual int getInfectionCount() const { return m_infection_count; }
	
	  //privileges, statuses, and properties
	virtual bool allowsOverlap() const { return m_allow_overlap; }
	virtual bool canUseExits() const { return m_can_use_exit; }
	virtual bool blocksFlames() const { return m_blocks_flames; }
	virtual bool blocksVomit() const { return m_blocks_vomit; }
	virtual bool canFallInPit() const { return m_can_fall_in_pit; }
	virtual bool canTriggerLandmine() const { return m_can_trigger_landmine; }
	virtual bool canTriggerInactiveLandmine() { return m_can_trigger_inactive_landmine; }
	virtual bool canDie() const { return m_can_die; }
	virtual bool canDieByFlames() const { return m_can_die_by_flames; }
	virtual bool canBeInfectedByVomit() const { return m_can_be_infected_by_vomit; }
	virtual bool canBeRescued() const { return m_can_be_rescued; }
	virtual bool isZombie() const { return m_is_zombie; }
	virtual bool isZombieTarget() const { return m_is_zombie_target; }
	virtual bool isActiveLandmine() const { return m_is_active; }
	virtual void performDeathAction() { return; }
	        bool isDead() const { return m_is_dead; }
	        bool isInfected() const { return m_is_infected; }

	
	  //mutators
	void activateLandmine() { m_is_active = true; }
	void setDead() { m_is_dead = true; }
	void setInfected() { m_is_infected = true; }
	void setInfectionCount(int n) { m_infection_count = n; }
	void incInfectionCount() { m_infection_count++; }
	void setHealthy() { m_is_infected = false; }

  private:
	StudentWorld* m_world;
	bool m_allow_overlap = false;
	bool m_can_use_exit = false;
	bool m_can_fall_in_pit = false;
	bool m_can_trigger_landmine = false;
	bool m_can_trigger_inactive_landmine = false;
	bool m_can_die = false;
	bool m_blocks_flames = false;
	bool m_blocks_vomit = false;
	bool m_can_die_by_flames = false;
	bool m_can_be_infected_by_vomit = false;
	bool m_can_be_rescued = false;
	bool m_is_infected = false;
	bool m_is_dead = false;
	bool m_is_zombie = false;
	bool m_is_zombie_target = false;
	bool m_is_active = false;
	int m_infection_count = 0;
};

 
class Agent : public Actor
{

  public:
	Agent(int ImageID, int start_x, int start_y, StudentWorld* world)
	:Actor(ImageID, start_x, start_y, right, world)
	{

	}
	virtual ~Agent() {};
	  //redefintion of inherited properties to have different outcomes than the defaults in the Actor base class
	virtual bool canFallInPit() const { return m_can_fall_in_pit; }
	virtual bool canTriggerLandmine() const { return m_can_trigger_landmine; }
	virtual bool canDie() const { return m_can_die; }
	virtual bool canDieByFlames() const { return m_can_die_by_flames; }


  protected:
	virtual Direction getVerticalDirectionToAgent(int src_row, int target_row)
	{
		return (src_row > target_row) ? down : up;
	}
	virtual Direction getHorizontalDirectionToAgent(int src_col, int target_col)
	{
		return (src_col > target_col) ? left : right;
	}


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
	virtual ~Human() {};
	virtual bool canUseExits() const { return m_can_use_exit; }
	virtual bool isZombieTarget() const { return m_is_zombie_target; }
	virtual bool canBeInfectedByVomit() const { return m_can_be_infected_by_vomit; }

protected:
	


  private:
	bool m_can_use_exit = true;
	bool m_is_zombie_target = true;
	bool m_can_be_infected_by_vomit = true;
	
};


class Penelope : public Human
{
  public:
	Penelope(int start_x, int start_y, StudentWorld* world)
	:Human(IID_PLAYER, start_x, start_y, world)
	{
		
	}
	virtual ~Penelope() {};
	virtual void doSomething();
	

  private:
	void shootFlame(double src_x, double src_y, Direction dir);
	int getFlameCharges() { return getWorld()->getFlameCharges(); }
	int getLandMines() { return getWorld()->getLandmines(); }
	int getVacccines() { return getWorld()->getVaccines(); }
	

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
	virtual ~Citizen() {};
	virtual void doSomething();
	virtual bool canBeRescued() const { return m_can_be_rescued; }

  private:
	Direction getVerticalDirToPenelope(int src_row)
	{
		return (src_row > getWorld()->getPlayerRow()) ? down : up;
	}

	Direction getHorizontalDirToPenelope(int src_col)
	{
		return (src_col > getWorld()->getPlayerCol()) ? left : right;

	}
	bool canMoveInDirection(Direction dir, std::pair<double, double>& new_pos);
	bool paralyzed = false;
	bool m_can_be_rescued = true;

};


class Zombie : public Agent
{
public:
	Zombie(int start_x, int start_y, StudentWorld* world)
		:Agent(IID_ZOMBIE, start_x, start_y, world)
	{

	}
	virtual ~Zombie() {};
	virtual bool isZombie() const { return m_is_zombie; }

protected:
	Direction getRandomDir() const;
	void getNewDestination();
	bool vomitAt(Direction dir, double target_x, double target_y);
	void computeVomitCoords(Direction dir, double& target_x, double& target_y);

	void decMovementPlanDistance() { movement_plan_distance--; }
	void setMovementPlanDistance(int n) { movement_plan_distance = n; }
	int getMovementPlanDistance() { return movement_plan_distance; }
	
	  
  private:
	bool m_is_zombie = true;
	int movement_plan_distance = 0;

};

class Smart_Zombie : public Zombie
{
  public:
	Smart_Zombie(int start_x, int start_y, StudentWorld* world)
	:Zombie(start_x, start_y, world)
	{


    }
	virtual ~Smart_Zombie() {};
	virtual void doSomething();
	virtual void performDeathAction() 
	{
		getWorld()->playSound(SOUND_ZOMBIE_DIE);
		getWorld()->increaseScore(2000);
	}
	

  private:
	  void getNewMovementPlan();
	  bool paralyzed = false;
	 

};


class Dumb_Zombie : public Zombie
{
  public:
	Dumb_Zombie(int start_x, int start_y, StudentWorld* world)
	:Zombie(start_x, start_y, world)
	{
		//1 in 10 chance of having a vaccine goodie
		int chance_vaccine_carrier = randInt(0, 9);
		if (chance_vaccine_carrier == 0)
			vaccine_carrier = true;
		else
			vaccine_carrier = false;
	}
	virtual ~Dumb_Zombie() {};
	virtual void doSomething();
	virtual void performDeathAction();
	
  private: 
	  void getNewMovementPlan();
	  void dropVaccineGoodie();
	  bool vaccine_carrier;
	  bool paralyzed = false;
};


class Wall : public Actor
{
  public:
	Wall(int start_x, int start_y, StudentWorld* world)
	:Actor(IID_WALL, start_x, start_y, right, world)
	{
	}
	virtual ~Wall() {};
	virtual void doSomething() { return; } //walls don't do anything

	virtual bool blocksFlames() const { return m_blocks_flames; }
	virtual bool blocksVomit() const { return m_blocks_vomit; }

  private:
	bool m_blocks_flames = true;
	bool m_blocks_vomit = true;
};



class Activating_Object : public Actor
{
	
public:
	Activating_Object(int ImageID, int start_x, int start_y, StudentWorld* world, Direction dir = right, int depth = 0)
	:Actor(ImageID, start_x, start_y, dir, world, depth)
	{

	}
	virtual ~Activating_Object() {};
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
	virtual ~Exit() {};
	virtual void doSomething();
	virtual bool blocksFlames() const { return m_blocks_flames; }

	
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
	virtual ~Pit() {};
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
	  virtual ~Landmine() {};
	  virtual void doSomething();

	  virtual bool canDie() const { return m_can_die; }
	  virtual bool canDieByFlames() const { return m_can_die_by_flames; }

  protected:
	int getTicks() { return m_safety_ticks; }
	void decTicks() { m_safety_ticks -= 1; }
	 

  private:
	int m_safety_ticks = 30;
	bool m_can_die = true;
	bool m_can_die_by_flames = true;
};


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
	  virtual ~Flame() {};
	  virtual void doSomething();
	  virtual bool canDie() const { return m_can_die; }
	  virtual bool canTriggerInactiveLandmine() const { return m_can_trigger_inactive_landmine; }
  
  protected:
	  int getTicks() { return m_ticks; }
	  void decTicks() { m_ticks -= 1; }

  private:
	  int m_ticks = 2;
	  bool m_can_die = true;
	  bool m_can_trigger_inactive_landmine = true;

};

class Vomit : public Activating_Object
{
  public:
	  Vomit(int start_x, int start_y, Direction dir, StudentWorld* world)
	  :Activating_Object(IID_VOMIT, start_x, start_y, world, dir)
	  {

	  }
	  virtual ~Vomit() {};
	  virtual void doSomething();
	  virtual bool canDie() { return m_can_die; }

  protected:
	  int getTicks() { return m_ticks; }
	  void decTicks() { m_ticks -= 1; }

  private:
	  int m_ticks = 2;
	  bool m_can_die = true;
	 
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
	  virtual ~Goodie() {};
	  virtual bool canDieByFlames() const { return m_can_die_by_flames; }
	  virtual bool canDie() const { return m_can_die; }

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
	virtual ~Vaccine_Goodie() {};
	virtual void doSomething();
};


class Gas_Can_Goodie : public Goodie
{
  public: 
	Gas_Can_Goodie(int start_x, int start_y, StudentWorld* world)
	:Goodie(IID_GAS_CAN_GOODIE, start_x, start_y, world)
	{
	}
	virtual ~Gas_Can_Goodie() {};
	virtual void doSomething();
};


class Landmine_Goodie : public Goodie
{
  public:
	Landmine_Goodie(int start_x, int start_y, StudentWorld* world)
	:Goodie(IID_LANDMINE_GOODIE, start_x, start_y, world)
	{
	}
	virtual ~Landmine_Goodie() {};
	virtual void doSomething();
};


#endif // ACTOR_H_
