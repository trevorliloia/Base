
#include "sfwdraw.h"
#include "GameState.h"
#include "BattleState.h"


/*
	The main function should be used for application state management.
	Currently, only one state is implemented here, you'll need to add additional
	and get them to behave properly.
*/
void main()
{
	sfw::initContext();
	Player player;
	int Gstate = 0;

	GameState gs;
	BattleState bs;
	
	gs.init(player); // called once
	bs.init(player);

	//gs.play(); // Should be called each time the state is transitioned into
	//bs.play();

	while (sfw::stepContext())
	{
		switch (Gstate)
		{
		case 0: // Enter Game
			gs.play();
		case 1: // Continue
			gs.step(); // called each update
			gs.draw(); // called each update
			Gstate = gs.next();
			break;

		case 2:
			bs.play();
		case 3:
			bs.step();
			bs.draw();
			Gstate = bs.next();
			break;
		}
		

		//gs.next(); Determine the ID of the next state to transition to.
	}

	gs.stop(); // should be called each time the state is transitioned out of
	bs.stop();

	gs.term(); // called once
	bs.term();


	sfw::termContext();

}