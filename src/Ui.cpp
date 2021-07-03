
#include <iostream>
#include "Ui.hpp"
#include "SimpleActor.hpp"



UI::UI() {

	const int GEN_SIZE = 300;
	const int GEN_COUNT = 10000;
	const int CUTOFF = 20;
	const double MUT_RATE = 2;

	std::random_device r;
	std::mt19937 randGen(r()) ;

	snake::Game game(8, 8, randGen);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window *win = SDL_CreateWindow("GAME",
	                                   SDL_WINDOWPOS_CENTERED,
	                                   SDL_WINDOWPOS_CENTERED,
	                                   1000, 1000, 0);


	Uint32 render_flags = SDL_RENDERER_ACCELERATED;


	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

	Eigen::Vector2i direction = Eigen::Vector2i(1,0);

	SimpleActor originalActor(8, 8, 16, 16);

	std::vector<SimpleActor> generation;
	Eigen::VectorXd scores(GEN_SIZE);


	for(int i = 0; i < GEN_SIZE; i++) {
		generation.push_back(originalActor.mutate(MUT_RATE * 100, randGen));
	}


	for(int g = 0; g < GEN_COUNT; g++) {



		if(g > 0) {

			std::vector<SimpleActor> bestActors;



			for(int i = 0; i < CUTOFF; i++) {
				int best;

				scores.maxCoeff(&best);

				bestActors.push_back(generation[best]);
				scores[best] = -1;
			}

			generation.clear();

			for(const auto& actor : bestActors) {
				generation.push_back(actor);
				for(int k = 0; k < GEN_SIZE / CUTOFF - 1; k++) {
					generation.push_back(actor.mutate(MUT_RATE, randGen));
				}
			}
		}

		for(int i = 0; i < GEN_SIZE; i++) {

			auto actor = generation[i];
			game.reset(randGen);

			for(int step = 0; step < 100; step++) {

				SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
				SDL_RenderClear(rend);

				auto field = game.getField();

				for (int i = 0; i < field.cols(); i++) {
					for (int j = 0; j < field.rows(); j++) {
						SDL_Rect dest;

						dest.w = 30;
						dest.h = 30;

						dest.x = (1000 - 32 * field.cols()) / 2 + i * 32 + 1;
						dest.y = (1000 - 32 * field.rows()) / 2 + j * 32 + 1;

						switch (field(j, i)) {
							case snake::CELL_FOOD:
								SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);
								SDL_RenderFillRect(rend, &dest);
								break;
							case snake::CELL_EMPTY:
								SDL_SetRenderDrawColor(rend, 127, 127, 127, 0);
								SDL_RenderDrawRect(rend, &dest);
								break;
							case snake::CELL_SNAKE:
								SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
								SDL_RenderFillRect(rend, &dest);
								break;
						}
					}
				}

				SDL_RenderPresent(rend);

				Eigen::VectorXd output = actor.react(game.getField());

				SimpleActor::outputToDirection(output, direction);

				if (game.tick(direction, randGen)) {
					break;
				}

			}
			int score = game.getScore();


			scores[i] = score;
		}

		std::cout << g << ", " << scores.sum() / GEN_SIZE << ", Best: " << scores.maxCoeff() << std::endl;
	}


	SDL_DestroyRenderer(rend);


	SDL_DestroyWindow(win);


	SDL_Quit();
}
