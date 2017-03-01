
#include "sfwdraw.h"
#include "GameState.h"
#include "BattleState.h"
#include "GameOver.h"
#include "Input.h"

/*
	The main function should be used for application state management.
	Currently, only one state is implemented here, you'll need to add additional
	and get them to behave properly.
*/
void main()
{
	sfw::initContext();
	Player player;
	player.invSize = 0;
	bool play = true;
	int Gstate = 0;

	GameState gs;
	BattleState bs;
	GameOver go;

	
	gs.init(player); // called once
	bs.init(player);
	go.init(player);

	//gs.play(); // Should be called each time the state is transitioned into
	//bs.play();

	while (sfw::stepContext() && play)
	{
		kpc::poll();
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
		case 7:
			go.play();
		case 8:
			go.step();
			go.draw();
			Gstate = go.next();
			break;

		case 9:
			play = false;
			break;
		}
		
		//gs.next(); Determine the ID of the next state to transition to.
	}

	gs.stop(); // should be called each time the state is transitioned out of
	bs.stop();
	go.stop();

	gs.term(); // called once
	bs.term();
	go.term();


	sfw::termContext();

}