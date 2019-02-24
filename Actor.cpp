#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>


  //doSomething() IMPLEMENTATIONS
void Penelope::doSomething()
{
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

				if(getWorld()->willCollideAt(newX, newY) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_RIGHT:
			{
				setDirection(right);
				double newX = getX() + 4;
				double newY = getY();

				if (getWorld()->willCollideAt(newX, newY) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_UP:
			{
				setDirection(up);
				double newX = getX();
				double newY = getY() + 4;
				
				if (getWorld()->willCollideAt(newX, newY) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_DOWN:
			{
				setDirection(down);
				double newX = getX();
				double newY = getY() - 4;

				if (getWorld()->willCollideAt(newX, newY) == false)
					moveTo(newX, newY);
				break;
			}
			case KEY_PRESS_SPACE: //attempt to shoot 3 flames in the direction penelope is facing
			{
				if (getFlameCharges() == 0)
					break;
				
				for (int i = 1; i <= 3; i++)
				{
					  //compute potential flame position
					std::pair<double, double> posi; //potential position (x, y)
					if (getDirection() == up)
					{
						posi.first = getX();
						posi.second = getY() + i * SPRITE_HEIGHT;
						  //check for collision with walls
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						shootFlame(posi.first, posi.second, getDirection());
					}
					else if (getDirection() == down)
					{
						posi.first = getX();
						posi.second = getY() - i * SPRITE_HEIGHT;
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						shootFlame(posi.first, posi.second, getDirection());
					}
					else if (getDirection() == left)
					{
						posi.first = getX() - i * SPRITE_WIDTH;
						posi.second = getY();
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						shootFlame(posi.first, posi.second, getDirection());
					}
					else if (getDirection() == right)
					{
						posi.first = getX() + i * SPRITE_WIDTH;
						posi.second = getY();
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						shootFlame(posi.first, posi.second, getDirection());
					}
				}
				getWorld()->decFlameCharges();
				break;
			}
			case KEY_PRESS_TAB:
			{
				//if(getLandMines() == 0)
					//break;

				Actor* landmine = new Landmine(getX() + 16, getY() + 16, getWorld());
				getWorld()->addActor(landmine);
				getWorld()->decLandmines();
				break;
			}
		}
	}
}

void Citizen::doSomething()
{
	double dist_p = getWorld()->getDistanceToPenelope(this);
	double dist_z = getWorld()->getDistanceToNearestZombie(this);




}




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

void Penelope::shootFlame(double src_x, double src_y, Direction dir)
{
	Actor* flame = new Flame(src_x, src_y, dir , getWorld());
	getWorld()->addActor(flame);
}