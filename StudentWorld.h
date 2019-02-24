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
	bool willCollideAt(double x, double y, bool projectile_exception = false);
	double euclideanDistance(double x1, double y1, double x2, double y2) const;
	bool overlaps(Actor* a1, Actor* a2, int threshold) const;
	
	  // Actor Abilities
	bool useExit(Actor* exit);
	bool fallInPit(Actor* pit);
	bool pickUpGoodie(Goodie* goodie);
	bool hitByFlame(Actor* flame);
	bool triggerLandmine(Actor* landmine);
	
	  // Accessors
	int getVaccines() const { return m_vaccines; }
	int getFlameCharges() const { return m_flame_charges; }
	int getLandmines() const { return m_landmines; }
	double getDistanceToPenelope(Actor* src) const;
	double getDistanceToNearestZombie(Actor* src);
	
	  // Mutators
	void setLevelCompletion(bool completion_status) 
	{
		m_level_complete = completion_status;
	}
	void addVaccine() { m_vaccines++; }
	void addFlameCharges(int n) { m_flame_charges += n; }
	void decFlameCharges() { m_flame_charges--; }
	void addLandmines(int n) { m_landmines += n; }
	void decLandmines() { m_landmines--; }
	void addActor(Actor* actor) { actors.push_back(actor); }




  private:
	std::vector<Actor*> actors;
	Actor* penelope = nullptr;
	bool m_level_complete = false;
	int m_vaccines = 0;
	int m_flame_charges = 0;
	int m_landmines = 0;

};

#endif // STUDENTWORLD_H_
