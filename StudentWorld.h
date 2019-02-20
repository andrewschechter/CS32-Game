#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>


class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld() { cleanUp(); }
      
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
	
	  // Mutators
	void setLevelCompletion(bool completion_status) 
	{
		m_level_complete = completion_status;
	}
	

private:
	std::vector<Actor*> actors;
	Actor* penelope;
	bool m_level_complete = false;

};

#endif // STUDENTWORLD_H_
