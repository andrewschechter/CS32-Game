#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;






GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	penelope = nullptr;
}


StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::getPlayerRow() const
{
	return penelope->getY() / SPRITE_HEIGHT;
}
int StudentWorld::getPlayerCol() const
{
	return penelope->getX() / SPRITE_WIDTH;
}

int StudentWorld::getInfectionCount() const
{
	return penelope->getInfectionCount();
}

int StudentWorld::init()
{	 
	
	  //restore initial values of private members
	m_level_complete = false;
	m_vaccines = 0;
	m_flame_charges = 0;
	m_landmines = 0;
	  
	  //initialize a level
	Level level(assetPath());

	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << getLevel() << ".txt";
	string levelFile = oss.str();

	Level::LoadResult result = level.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found || getLevel() == 100)
	{
		cerr << "Cannot find " << levelFile << " data file" << endl;
		return GWSTATUS_PLAYER_WON;
	}
	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Your level was improperly formatted" << endl;
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded " << levelFile << endl;
		for (int x = 0; x < LEVEL_WIDTH; x++)
			for (int y = 0; y < LEVEL_HEIGHT; y++)
			{
				Level::MazeEntry ge = level.getContentsOf(x, y); //get game element at x, y in the level file
				switch (ge)
				{
					case Level::empty:
						break;
					case Level::player:
					{
						penelope = new Penelope(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						break;
					}
					case Level::citizen:
					{
						Actor* citizen = new Citizen(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(citizen);
						break;
					}
					case Level::dumb_zombie:
					{
						Actor* dumb_zombie = new Dumb_Zombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(dumb_zombie);
						break;
					}
					case Level::smart_zombie:
					{
						Actor* smart_zombie = new Smart_Zombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(smart_zombie);
						break;
					}
					case Level::wall:
					{
						Actor* wall = new Wall(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(wall);
						break;
					}
					case Level::exit:
					{
						Actor* exit = new Exit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(exit);
						break;
					}
					case Level::pit:
					{
						Actor* pit = new Pit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(pit);
						break;
					}
					case Level::vaccine_goodie:
					{
						Actor* vaccine = new Vaccine_Goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(vaccine);
						break;
					}
					case Level::gas_can_goodie:
					{
						Actor* gas_can = new Gas_Can_Goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(gas_can);
						break;
					}
					case Level::landmine_goodie:
					{
						Actor* landmine = new Landmine_Goodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
						actors.push_back(landmine);
						break;
					}
				}
			}
	}

	
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	
	if (m_level_complete == true)
	{
		playSound(SOUND_LEVEL_FINISHED);
		return GWSTATUS_FINISHED_LEVEL;
	}
	
	if (penelope->isDead())
	{
		playSound(SOUND_PLAYER_DIE);
		return GWSTATUS_PLAYER_DIED;
	}
	penelope->doSomething();
	

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		
		(*it)->doSomething();
		
		if (penelope->isDead())
		{				
			decLives();
			playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}
		if (m_level_complete == true)
		{
			playSound(SOUND_LEVEL_FINISHED);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	  // Remove newly-dead actors after each tick
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if ((*it)->isDead())
		{
			(*it)->performDeathAction();
			
			delete *it;
			actors.erase(it);
			it = actors.begin();
		}
	}

	  // Update the game status line
	ostringstream oss;
	
	oss.fill('0');
	if (getScore() < 0)
		oss << "Score: " << "-" << setw(5) << -1*getScore();
	else
	{
		
		oss << "Score: " << setw(6) << getScore();
	}
	oss << "  Level: " << getLevel()
		<< "  Lives: " << getLives()
		<< "  Vacc:  " << getVaccines()
		<< "  Flames: " << getFlameCharges()
		<< "  Mines: " << getLandmines()
		<< "  Infected: " << getInfectionCount();
	string stats = oss.str();
	setGameStatText(stats);

	  // the player hasn’t completed the current level and hasn’t died, so continue playing the current level
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (penelope != nullptr)
	{
		delete penelope;
		penelope = nullptr;
	}
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		delete *it;
	}
	actors.clear();
}


  // Helper Function

//note: certain projectiles may allow collisions to occur on objects that otherwise don't allow overlap, hence a projectile exception is needed
bool StudentWorld::willCollideAt(double new_x, double new_y, Actor* src, bool flame_exception, bool vomit_exception)
{
	
	if (!penelope->allowsOverlap() && src != penelope)
	{
		
		if (euclideanDistance(new_x, new_y, penelope->getX(), penelope->getY()) < SPRITE_WIDTH)
			return true;

	}
	  
	  // for each actor that doesn't allow overlap, 
	  //check if it's bouding box would overlap with the hypothetical bounding box created at the new x, y
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		
		Actor* curr_actor = *it; //set the current actor to the actor pointed to by the vector iterator
		
		if (src == curr_actor)
			continue;
		
		
		if (flame_exception)
		{
			if (!curr_actor->blocksFlames())
			{
				continue;
			}
		}
		else if (vomit_exception)
		{
			if (!curr_actor->blocksFlames())
			{
				continue;
			}
		}
		else if(curr_actor->allowsOverlap())
		{
			continue;
		}
		 
		if (euclideanDistance(new_x, new_y, curr_actor->getX(), curr_actor->getY()) < SPRITE_WIDTH)
		{
			return true;
		}
	}
	return false;

}


double StudentWorld::euclideanDistance(double x1, double y1, double x2, double y2) const
{
	//calculate the euclideanDistance between any two points D^2 = (dX)^2 + (dy)^2 
	x1 += SPRITE_WIDTH / 2;
	x2 += SPRITE_WIDTH / 2;

	y1 += SPRITE_HEIGHT / 2;
	y2 += SPRITE_HEIGHT / 2;
	
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

bool StudentWorld::overlaps(Actor* a1, Actor* a2, int threshold) const // threshold is min separation in pixels
{	
	if (euclideanDistance(a1->getX(), a1->getY(), a2->getX(), a2->getY()) <= threshold)
		return true;
	return false;
}

bool StudentWorld::anyOverlapAt(double x, double y, int threshold) 
{
	if (euclideanDistance(x, y, penelope->getX(), penelope->getY()) <= threshold)
		return true;
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if (euclideanDistance(x, y, (*it)->getX(), (*it)->getY()) <= threshold)
			return true;
	}

	return false;

}


  // Actor Abilities 
bool StudentWorld::useExit(Actor* exit)
{
	if (penelope->canUseExits() && getNumNeedRescue() == 0 && overlaps(penelope, exit, 10)) //penelope should be able to exit if no citizens are present
	{
		return true;
	}
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if ((*it)->canUseExits() && overlaps(*it, exit, 10))
		{
			//citizen saved!
			(*it)->setDead();
			return true;
		}

	}
	return false;
}

bool StudentWorld::fallInPit(Actor* pit)
{
	if (penelope->canFallInPit()  && overlaps(penelope, pit, 10))
	{
		penelope->setDead();
		return true;
	}
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if ((*it)->canFallInPit() && overlaps(*it, pit, 10))
		{
			
			if ((*it)->canBeRescued())
			{
				playSound(SOUND_CITIZEN_DIE);
				increaseScore(-1000);
			}
			(*it)->setDead();

			return true;
		}
		
	}
	return false;

}

bool StudentWorld::hitByFlame(Actor* flame)
{
	if (!penelope->blocksFlames() && penelope->canDieByFlames() && overlaps(penelope, flame, 10))
	{
		penelope->setDead();
		return true;
	}

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if (!(*it)->blocksFlames() && (*it)->canDieByFlames() && overlaps(*it, flame, 10))
		{
			
			if ((*it)->canBeRescued())
			{
				playSound(SOUND_CITIZEN_DIE);
				increaseScore(-1000);
			}
			(*it)->setDead();
			return true;
		}
	
	}
	return false;
}

bool StudentWorld::hitByVomit(Actor* vomit)
{
	if (!penelope->blocksVomit() && penelope->canBeInfectedByVomit() && overlaps(penelope, vomit, 10))
	{
		penelope->setInfected();
		return true;
	}

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if (!(*it)->blocksVomit() && (*it)->canBeInfectedByVomit() && overlaps(*it, vomit, 10))
		{
			if ((*it)->isInfected())
				continue;
			(*it)->setInfected();
			playSound(SOUND_CITIZEN_INFECTED);
			return true;
		}
	}
	return false;

}


bool StudentWorld::triggerLandmine(Actor* landmine)
{
	if (penelope->canTriggerLandmine() && landmine->isActiveLandmine() && overlaps(penelope, landmine))
	{	
		return true;
	}

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		
		if ((*it)->canTriggerInactiveLandmine() && overlaps(*it, landmine))
		{
			return true;
		}
		else if ((*it)->canTriggerLandmine() && landmine->isActiveLandmine() && overlaps(*it, landmine))
		{
			return true;
		}

	}
	return false;

}

bool StudentWorld::pickUpGoodie(Goodie* goodie)
{
	if (overlaps(penelope, goodie))
	{
		goodie->setDead();
		return true;
	}
	return false;

}

double StudentWorld::getDistanceToPenelope(Actor* src) const
{
	return euclideanDistance(penelope->getX(), penelope->getY(), src->getX(), src->getY());
	
}


double StudentWorld::getDistanceToNearestZombieAt(double src_x, double src_y)
{
	double curr_minimum = VIEW_WIDTH * VIEW_WIDTH + VIEW_HEIGHT * VIEW_HEIGHT;

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if (!(*it)->isZombie())
			continue;
		
		double distance = euclideanDistance((*it)->getX(), (*it)->getY(), src_x, src_y);
		if (distance < curr_minimum)
			curr_minimum = distance;
	}

	return curr_minimum;
}

void StudentWorld::getNearestZombieTargetAt(double src_x, double src_y, double& target_x, double& target_y, double& target_distance)
{
	double curr_minimum = VIEW_WIDTH * VIEW_WIDTH + VIEW_HEIGHT * VIEW_HEIGHT;

	if (penelope->isZombieTarget())
	{
		double distance = euclideanDistance(penelope->getX(), penelope->getY(), src_x, src_y);
		if (distance < curr_minimum)
		{
			curr_minimum = distance;
			target_x = penelope->getX();
			target_y = penelope->getY();
		}
	}
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if (!(*it)->isZombieTarget())
			continue;

		double distance = euclideanDistance((*it)->getX(), (*it)->getY(), src_x, src_y);
		if (distance < curr_minimum)
		{
			curr_minimum = distance;
			target_x = (*it)->getX();
			target_y = (*it)->getY();
		}
	}

}

bool StudentWorld::vomitTargetAt(double target_x, double target_y, int range)
{
	if (penelope->isZombieTarget())
	{
		double distance = euclideanDistance(penelope->getX(), penelope->getY(), target_x, target_y);
		if (distance <= range)
			return true;
	}

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if (!(*it)->isZombieTarget())
			continue;

		double distance = euclideanDistance((*it)->getX(), (*it)->getY(), target_x, target_y);
		if (distance <= range)
			return true;
	}
	return false;

}


int StudentWorld::getNumNeedRescue()
{
	int total = 0;
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if ((*it)->canBeRescued())
			total++;
	}
	return total;
}




