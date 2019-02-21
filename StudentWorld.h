#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>


class Actor;
class Goodie;

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
      
	  // Inherited Functions From GameWorld
	virtual int init();
    virtual int move();
    virtual void cleanUp();

	  // Helper Functions
	bool willCollideAt(double x, double y);
	double euclideanDistance(double x1, double y1, double x2, double y2) const;
	bool overlaps(Actor* a1, Actor* a2, int threshold) const;
	
	  // Actor Abilities
	bool useExit(Actor* exit);
	bool fallInPit(Actor* pit);
	bool StudentWorld::pickUpGoodie(Goodie* goodie);
	
	  // Accessors
	int getVaccines() { return m_vaccines; }
	int getFlameCharges() { return m_flame_charges; }
	int getLandmines() { return m_landmines; }
	
	  // Mutators
	void setLevelCompletion(bool completion_status) 
	{
		m_level_complete = completion_status;
	}
	void addVaccine() { m_vaccines++; }
	void addFlameCharges(int n) { m_flame_charges += n; }
	void addLandmines(int n) { m_landmines += n; }

  private:
	std::vector<Actor*> actors;
	Actor* penelope = nullptr;
	bool m_level_complete = false;
	int m_vaccines = 0;
	int m_flame_charges = 0;
	int m_landmines = 0;

};

#endif // STUDENTWORLD_H_
