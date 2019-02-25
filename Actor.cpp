#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>



//*********************************************************//
//			  PENELOPE IMPLEMENTATION
//******************************************************** //
void Penelope::doSomething()
{
		
	if (isInfected())
	{
		incInfectionCount();
		if (getInfectionCount() >= 500)
		{
			setDead();
			return;
		}
	}


	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
			case KEY_PRESS_LEFT:
			{
				setDirection(left);
				double newX = getX() - 4;
				double newY = getY();

				if(getWorld()->willCollideAt(newX, newY, this) == false)
					moveTo(newX, newY);

				break;
			}
			case KEY_PRESS_RIGHT:
			{
				setDirection(right);
				double newX = getX() + 4;
				double newY = getY();

				if (getWorld()->willCollideAt(newX, newY, this) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_UP:
			{
				setDirection(up);
				double newX = getX();
				double newY = getY() + 4;
				
				if (getWorld()->willCollideAt(newX, newY, this) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_DOWN:
			{
				setDirection(down);
				double newX = getX();
				double newY = getY() - 4;

				if (getWorld()->willCollideAt(newX, newY, this) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_SPACE: //attempt to shoot 3 flames in the direction penelope is facing
			{
				if (getFlameCharges() == 0)
					break;
				
				for (int i = 1; i <= 3; i++)
				{
					  //compute potential flame new_postion
					std::pair<double, double> new_pos; //potential new_postion (x, y)
					if (getDirection() == up)
					{
						new_pos.first = getX();
						new_pos.second = getY() + i * SPRITE_HEIGHT;
						  //check for collision with walls
						if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this, true))
							break;
						shootFlame(new_pos.first, new_pos.second, getDirection());
					}
					else if (getDirection() == down)
					{
						new_pos.first = getX();
						new_pos.second = getY() - i * SPRITE_HEIGHT;
						if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this, true))
							break;
						shootFlame(new_pos.first, new_pos.second, getDirection());
					}
					else if (getDirection() == left)
					{
						new_pos.first = getX() - i * SPRITE_WIDTH;
						new_pos.second = getY();
						if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this, true))
							break;
						shootFlame(new_pos.first, new_pos.second, getDirection());
					}
					else if (getDirection() == right)
					{
						new_pos.first = getX() + i * SPRITE_WIDTH;
						new_pos.second = getY();
						if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this, true))
							break;
						shootFlame(new_pos.first, new_pos.second, getDirection());
					}
				}
				getWorld()->decFlameCharges();
				break;
			}
			case KEY_PRESS_TAB:
			{
				//if(getLandMines() == 0)
					//break;

				Actor* landmine = new Landmine(getX(), getY(), getWorld());
				getWorld()->addActor(landmine);
				getWorld()->decLandmines();
				break;
			}
		}
	}
}

void Penelope::shootFlame(double src_x, double src_y, Direction dir)
{
	Actor* flame = new Flame(src_x, src_y, dir, getWorld());
	getWorld()->addActor(flame);
}


//*********************************************************//
//			   CITIZEN IMPLEMENTATION 
//******************************************************** //

void Citizen::doSomething()
{
	if (paralyzed)
	{
		paralyzed = false;
		return;
    }
	else
		paralyzed = true;


	double dist_p = getWorld()->getDistanceToPenelope(this);
	double dist_z = getWorld()->getDistanceToNearestZombieAt(this->getX(), this->getY());

	if ((dist_z == -1 || dist_p < dist_z) && dist_p <= 80)
	{
		
		int citizen_row = getY() / SPRITE_HEIGHT;
		int citizen_col = getX() / SPRITE_WIDTH;

		std::pair<double, double> new_pos;


		if (citizen_row == getWorld()->getPlayerRow())
		{
			Direction dir = getHorizontalDirToPenelope(citizen_col);
			if (canMoveInDirection(dir, new_pos))
			{
				setDirection(dir);
				moveTo(new_pos.first, new_pos.second);
				return;
			}
		}	
		else if(citizen_col == getWorld()->getPlayerCol())
		{
			Direction dir = getVerticalDirToPenelope(citizen_row);
			if (canMoveInDirection(dir, new_pos))
			{
				setDirection(dir);
				moveTo(new_pos.first, new_pos.second);
				return;
			}
		}
		else //citizen and penelope are not on the same row or col
		{
			Direction dir_0 = getVerticalDirToPenelope(citizen_row); //dir 0
			Direction dir_1 = getHorizontalDirToPenelope(citizen_col);  //dir 1
			int choose_dir = randInt(0, 1);
			if (choose_dir == 0)
			{
				if (canMoveInDirection(dir_0, new_pos))
				{
					setDirection(dir_0);
					moveTo(new_pos.first, new_pos.second);
					return;
				}
				else if (canMoveInDirection(dir_1, new_pos))
				{
					setDirection(dir_1);
					moveTo(new_pos.first, new_pos.second);
					return;
				}
			}
			else if (choose_dir == 1)
			{
				if (canMoveInDirection(dir_1, new_pos))
				{
					setDirection(dir_1);
					moveTo(new_pos.first, new_pos.second);
					return;
				}
				else if (canMoveInDirection(dir_0, new_pos))
				{
					setDirection(dir_0);
					moveTo(new_pos.first, new_pos.second);
					return;
				}
			}
		}
	}
	else if (dist_z <= 80)
	{
		
		std::pair<double, double> new_pos;     // new final position x, y
		std::pair<double, double> pot_new_pos; //potential new position x, y
		double new_dist_z = dist_z;
		Direction new_dir;
		if (canMoveInDirection(up, pot_new_pos))
		{	
			double pot_new_dist_z = getWorld()->getDistanceToNearestZombieAt(pot_new_pos.first, pot_new_pos.second);
			if (pot_new_dist_z > new_dist_z)
			{
			
				new_dist_z = pot_new_dist_z;
				new_pos.first = pot_new_pos.first;
				new_pos.second = pot_new_pos.second;
				new_dir = up;
			}
		}
		if (canMoveInDirection(right, pot_new_pos))
		{	
			double pot_new_dist_z = getWorld()->getDistanceToNearestZombieAt(pot_new_pos.first, pot_new_pos.second);
			if (pot_new_dist_z > new_dist_z)
			{

				new_dist_z = pot_new_dist_z;
				new_pos.first = pot_new_pos.first;
				new_pos.second = pot_new_pos.second;
				new_dir = right;
			}
		}
		if (canMoveInDirection(left, pot_new_pos))
		{		
			double pot_new_dist_z = getWorld()->getDistanceToNearestZombieAt(pot_new_pos.first, pot_new_pos.second);
			if (pot_new_dist_z > new_dist_z)
			{
				new_dist_z = pot_new_dist_z;
				new_pos.first = pot_new_pos.first;
				new_pos.second = pot_new_pos.second;
				new_dir = left;
			}
		}
		if (canMoveInDirection(down, pot_new_pos))
		{
			double pot_new_dist_z = getWorld()->getDistanceToNearestZombieAt(pot_new_pos.first, pot_new_pos.second);
			if (pot_new_dist_z > new_dist_z)
			{
				new_dist_z = pot_new_dist_z;
				new_pos.first = pot_new_pos.first;
				new_pos.second = pot_new_pos.second;
				new_dir = down;
			}
		}
		if (new_dist_z > dist_z)
		{
			setDirection(new_dir);
			moveTo(new_pos.first, new_pos.second);
		}
		else
			return;
	}
}


bool Citizen::canMoveInDirection(Direction dir, std::pair<double,double>& new_pos)
{
	//compute potential new_postion to move to
	switch (dir)
	{
		case up:
		{
			new_pos.first = getX();
			new_pos.second = getY() + 2;
			//check for collision with walls
			if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this))
				return false;
			break;
		}
		case down:
		{
			new_pos.first = getX();
			new_pos.second = getY() - 2;
			//check for collision with walls
			if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this))
				return false;
			break;
		}
		case left:
		{
			new_pos.first = getX() - 2;
			new_pos.second = getY();
			//check for collision with walls
			if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this))
				return false;
			break;
		}
		case right:
		{
			new_pos.first = getX() + 2;
			new_pos.second = getY();
			//check for collision with walls
			if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this))
				return false;
			break;
		}
	}
	return true;
}


//*********************************************************//
//		    ZOMBIE IMPLEMENTATIONS
//******************************************************** //

Direction Zombie::getRandomDir() const
{
	int choose_dir = randInt(0, 3);
	switch (choose_dir)
	{
		case 0:
			return up;
			break;
		case 1:
			return down;
			break;
		case 2:
			return right;
			break;
		case 3:
			return left;
			break;
	}

}


void Dumb_Zombie::doSomething()
{
	if (paralyzed)
	{
		paralyzed = false;
		return;
	}
	else
		paralyzed = true;

	if (movement_plan_distance == 0)
		getNewMovementPlan();
	getNewDestination();

}

void Dumb_Zombie::getNewMovementPlan()
{
	movement_plan_distance = randInt(3, 10);

	Direction new_dir = getRandomDir();
	setDirection(new_dir);
}


void Zombie::getNewDestination()
{
	Direction curr_dir = getDirection();
	std::pair<double, double> new_pos;
	switch (curr_dir)
	{
		case up:
			new_pos.first = getX();
			new_pos.second = getY() + 1;
			break;
		case down:
			new_pos.first = getX();
			new_pos.second = getY() - 1;
			break;
		case left:
			new_pos.first = getX() - 1;
			new_pos.second = getY();
			break;
		case right:
			new_pos.first = getX() + 1;
			new_pos.second = getY();
			break;
	}

	if (getWorld()->willCollideAt(new_pos.first, new_pos.second, this) == false)
	{
		moveTo(new_pos.first, new_pos.second);
		decMovementPlanDistance();
	}
	else
		setMovementPlanDistance(0);

}

void Smart_Zombie::doSomething()
{
	if (paralyzed)
	{
		paralyzed = false;
		return;
	}
	else
		paralyzed = true;

	if (movement_plan_distance == 0)
		getNewMovementPlan();
	getNewDestination();


}

void Smart_Zombie::getNewMovementPlan()
{
	movement_plan_distance = randInt(3, 10);

	double target_dist, target_x, target_y;
	getWorld()->getNearestZombieTargetAt(getX(), getY(), target_x, target_y, target_dist);
	if (target_dist > 80) //if the distance to the target is out of range get a random direction
	{
		Direction new_dir = getRandomDir();
		setDirection(new_dir);
	}
	else if (target_dist <= 80) //if the distance to the target is within range choose direction that gets the source closer
	{
		// get target and source row and col
		int zombie_row = getY() / SPRITE_HEIGHT;
		int zombie_col = getX() / SPRITE_WIDTH;
		
		int target_row = target_y / SPRITE_HEIGHT;
		int target_col = target_x / SPRITE_WIDTH;

		Direction new_dir;

		// if target and source are on the same col or row choose the direction that gets source closer
		if (zombie_col == target_col)
		{
			new_dir = getHorizontalDirectionToAgent(zombie_col, target_col);
		}
		else if (zombie_row == target_row)
		{
			new_dir = getVerticalDirectionToAgent(zombie_row, target_row);
		}
		else //otherwise choose a random direction that gets source closer 
		{
			Direction new_dir_0 = getHorizontalDirectionToAgent(zombie_col, target_col);
			Direction new_dir_1 = getVerticalDirectionToAgent(zombie_row, target_row);
			int choose_dir = randInt(0, 1);
			if (choose_dir == 0)
				new_dir = new_dir_0;
			else if (choose_dir == 1)
				new_dir = new_dir_1;
		}
		setDirection(new_dir); //set the new direction
	}

}




//*********************************************************//
//		    ACTIVATING OBJECT IMPLEMENTATIONS
//******************************************************** //

void Exit::doSomething()
{
	//determine if penelope overlaps
	if (getWorld()->useExit(this))
		getWorld()->setLevelCompletion(true);

}

void Pit::doSomething()
{
	//check if actor overlaps
	if (getWorld()->fallInPit(this))
		std::cout << "something is gonna fall in a pit" << std::endl;

}

void Landmine::doSomething()
{

	if (getTicks() == 0)
		m_active = true;
	else
	{
		decTicks();
		return;
	}

	//check if landmine overlaps with penelope, a citizen, or a zombie
	if (getWorld()->triggerLandmine(this))
	{
		std::cout << "landmine triggered" << std::endl;
		setDead();
		// add flames in the north, south, east, west, NW, SW, NE, SE directions around the landmine
		// Actor* flameC = new Flame(getX(), getY(), up, getWorld());
		//Actor* flameN = new Flame(getX(), getY() + SPRITE_HEIGHT, up, getWorld());
		//Actor* flameS = new Flame(getX(), getY() - SPRITE_HEIGHT, down, getWorld());
		//Actor* flameW = new Flame(getX() - SPRITE_WIDTH, getY(), left, getWorld());
	   	//Actor* flameE = new Flame(getX() + SPRITE_WIDTH, getY(), right, getWorld());
		//Actor* flameNW = new Flame(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, left, getWorld());
		//Actor* flameNE = new Flame(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, right, getWorld());
		//Actor* flameSW = new Flame(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, left, getWorld());
		//Actor* flameSE = new Flame(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, right, getWorld());
		
		//getWorld()->addActor(flameC);
		//getWorld()->addActor(flameN);
		//getWorld()->addActor(flameS);
		//getWorld()->addActor(flameW);
		//getWorld()->addActor(flameE);
		//getWorld()->addActor(flameNW);
		//getWorld()->addActor(flameNE);
		//getWorld()->addActor(flameSW);
		//getWorld()->addActor(flameSE);

		//Actor* pit = new Pit(getX(), getY(), getWorld());
		//getWorld()->addActor(pit);
			   	
		/*

		//Actor* flameC = new Flame(getX(), getY(), up, getWorld());
		//getWorld()->addActor(flameC);
		Actor* flameN = new Flame(getX(), getY() + SPRITE_HEIGHT, up, getWorld());
		getWorld()->addActor(flameN);
		Actor* flameS = new Flame(getX(), getY() - SPRITE_HEIGHT, down, getWorld());
		getWorld()->addActor(flameS);
		Actor* flameW = new Flame(getX() - SPRITE_WIDTH, getY(), left, getWorld());
		getWorld()->addActor(flameW);
		Actor* flameE = new Flame(getX() + SPRITE_WIDTH, getY(), right, getWorld());
		getWorld()->addActor(flameE);
		Actor* flameNW = new Flame(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, left, getWorld());
		getWorld()->addActor(flameNW);
		Actor* flameNE = new Flame(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, right, getWorld());
		getWorld()->addActor(flameNE);
		Actor* flameSW = new Flame(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, left, getWorld());
		getWorld()->addActor(flameSW);
		Actor* flameSE = new Flame(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, right, getWorld());
		getWorld()->addActor(flameSE);
		*/

	}


}

void Flame::doSomething()
{
	if (getTicks() == 0)
	{
		setDead();
		return;
	}
	else
		decTicks();

	getWorld()->hitByFlame(this);

}

void Vomit::doSomething()
{
	if (getTicks() == 0)
	{
		setDead();
		return;
	}
	else
		decTicks();

	getWorld()->hitByVomit(this);

}



//*********************************************************//
//			   GOODIE IMPLEMENTATIONS 
//******************************************************** //

void Vaccine_Goodie::doSomething()
{
	if (getWorld()->pickUpGoodie(this))
	{
		getWorld()->increaseScore(50);
		getWorld()->addVaccine();
	}
}

void Gas_Can_Goodie::doSomething()
{
	if (getWorld()->pickUpGoodie(this))
	{
		getWorld()->increaseScore(50);
		getWorld()->addFlameCharges(5);
	}
}

void Landmine_Goodie::doSomething()
{
	if (getWorld()->pickUpGoodie(this))
	{
		getWorld()->increaseScore(50);
		getWorld()->addLandmines(2);
	}
}

