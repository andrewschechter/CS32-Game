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
}

StudentWorld::~StudentWorld()
{
	cleanUp();
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
	oss << "level0" << getLevel() << ".txt";
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
		return GWSTATUS_FINISHED_LEVEL;
	
	if (penelope->isDead())
		return GWSTATUS_PLAYER_DIED;
	penelope->doSomething();
	

	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if (!(*it)->isDead())
		{
			(*it)->doSomething();
		
			if (penelope->isDead())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (m_level_complete == true)
				return GWSTATUS_FINISHED_LEVEL;
		}
	}

	  // Remove newly-dead actors after each tick
	for (it = actors.begin(); it != actors.end(); it++)
	{
		if ((*it)->isDead())
		{
			delete *it;
			actors.erase(it);
			it = actors.begin();
		}
	}

	  // Update the game status line
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(6) << getScore()
		<< "  Level: " << getLevel()
		<< "  Lives: " << getLives()
		<< "  Vacc:  " << getVaccines()
		<< "  Flames: " << getFlameCharges()
		<< "  Mines: " << getLandmines()
		<< "  Infected: " << "?";
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


  // Helper Functions
bool StudentWorld::willCollideAt(double new_x, double new_y) 
{
	  // new position's bounding box
	double new_x_max = new_x + SPRITE_WIDTH - 1;
	double new_y_max = new_y + SPRITE_HEIGHT - 1;
	
	  // for each actor that doesn't allow overlap, 
	  //check if it's bouding box would overlap with the hypothetical bounding box created at the new x, y
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		Actor* curr_actor = *it; //set the current actor to the actor pointed to by the vector iterator
		if (curr_actor->allowsOverlap() == true)
			continue;
		   
		  // current actor's bounding box
		double obj_x = curr_actor->getX(); 
		double obj_y = curr_actor->getY();
		double obj_x_max = obj_x + SPRITE_WIDTH - 1;
		double obj_y_max = obj_y + SPRITE_HEIGHT - 1;

		  // check each of the new bounding box and see if it is within the current actor's bounding box
		if ((new_x >= obj_x && new_x < obj_x_max) && (new_y >= obj_y && new_y < obj_y_max))
			return true;
		else if ((new_x_max >= obj_x && new_x_max < obj_x + SPRITE_WIDTH - 1) && (new_y_max >= obj_y && new_y_max < obj_y_max))
			return true;
		else if ((new_x_max >= obj_x && new_x_max < obj_x + SPRITE_WIDTH - 1) && (new_y >= obj_y && new_y < obj_y_max))
			return true;
		else if ((new_x >= obj_x && new_x < obj_x_max) && (new_y_max >= obj_y && new_y_max < obj_y_max))
			return true;
			
	}
	return false;
}

double StudentWorld::euclideanDistance(double x1, double y1, double x2, double y2) const
{
	//calculate the euclideanDistance between any two points D^2 = (dX)^2 + (dy)^2 
	return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
}

bool StudentWorld::overlaps(Actor* a1, Actor* a2, int threshold) const // threshold is min separation in pixels
{	
	if (euclideanDistance(a1->getX(), a1->getY(), a2->getX(), a2->getY()) <= threshold*threshold)
		return true;
	return false;
}

  
  // Actor Abilities 
bool StudentWorld::useExit(Actor* exit)
{
	if (penelope->canUseExits() && overlaps(penelope, exit, 10)) //penelope should be able to exit if no citizens are present
		return true;
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if ((*it)->canUseExits() && overlaps(*it, exit, 10))
			return true;

	}
	return false;
}

bool StudentWorld::fallInPit(Actor* pit)
{
	if (penelope->canDie() && overlaps(penelope, pit, 10))
	{
		penelope->setDead();
		return true;
	}
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{

		if ((*it)->canDie() && overlaps(*it, pit, 10))
			return true;

	}
	return false;

}

bool StudentWorld::pickUpGoodie(Goodie* goodie)
{
	if (overlaps(penelope, goodie, 10))
	{
		goodie->setDead();
		return true;
	}
	return false;

}
/*
bool StudentWorld::shootFlame(Actor* src)
{
	if (src == penelope)
		if (getFlameCharges == 0)
			return false;




}*/