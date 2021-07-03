//
// Created by jonas on 7/2/21.
//

#ifndef GENETICSNAKE_GAME_HPP
#define GENETICSNAKE_GAME_HPP


#include <list>
#include <random>

#include <Eigen/Dense>

namespace snake {

	const int CELL_EMPTY = 0;
	const int CELL_FOOD = 1;
	const int CELL_SNAKE = -1;

	class Game {
		Eigen::MatrixXi field;
		std::list<Eigen::Vector2i> snake;
		int score;

		int &getCell(const Eigen::Vector2i &coords);
		void spawnFood(std::mt19937& randGen);


	public:
		Game(int width, int height, std::mt19937& randGen);

		bool tick(Eigen::Vector2i &direction, std::mt19937& randGen);

		int getScore() const;

		const Eigen::MatrixXi& getField();

		void reset(std::mt19937& randGen);
	};

}

#endif //GENETICSNAKE_GAME_HPP
