


#include "Header.h"

void GG::MainWindow()
{		
	    
	   bool Draw=false;
		while (this->window->isOpen())
		{
			while (this->window->pollEvent(ev))
			{
				switch (ev.type)
				{
				case sf::Event::Closed:
					this->window->close();
				case sf::Event::KeyPressed:
					if (ev.key.code == sf::Keyboard::Escape)
					{
						this->window->close();
					}
					if (ev.key.code == sf::Keyboard::S)
					{	
						
						Draw = Solve_AStar();
					}
					if (ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::Down || ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::Left)
					{
						//Draw = false;
						ChangeDestination(ev.key.code);
					}
				
				default:
					break;
				}
			}
			this->window->clear();

			
			DrawNodes(Draw);
			
			
				
			//draw Here
			this->window->display();
		}
}

