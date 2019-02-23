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
			case KEY_PRESS_SPACE:
			{
				  
				for (int i = 1; i < 4; i++)
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
						Actor* flame = new Flame(posi.first, posi.second, getDirection(), getWorld());
						getWorld()->addActor(flame);
					}
					else if (getDirection() == down)
					{
						posi.first = getX();
						posi.second = getY() - i * SPRITE_HEIGHT;
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						Actor* flame = new Flame(posi.first, posi.second, getDirection(), getWorld());
						getWorld()->addActor(flame);
					}
					else if (getDirection() == left)
					{
						posi.first = getX() - i * SPRITE_WIDTH;
						posi.second = getY();
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						Actor* flame = new Flame(posi.first, posi.second, getDirection(), getWorld());
						getWorld()->addActor(flame);
					}
					else if (getDirection() == right)
					{
						posi.first = getX() + i * SPRITE_WIDTH;
						posi.second = getY();
						if (getWorld()->willCollideAt(posi.first, posi.second, true))
							break;
						Actor* flame = new Flame(posi.first, posi.second, getDirection(), getWorld());
						getWorld()->addActor(flame);
					}
				}
			}
		}
	}
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







/*

void Penelope::shootFlame()
{




}*/