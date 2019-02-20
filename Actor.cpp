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