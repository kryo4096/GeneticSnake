//
// Created by jonas on 7/2/21.
//

#include <iostream>
#include "Game.hpp"

namespace snake {

	bool Game::tick(Eigen::Vector2i &direction, std::mt19937& randGen) {

		Eigen::Vector2i newCellCoords = snake.front() + direction;

		if (newCellCoords.x() < 0 || newCellCoords.y() < 0 || newCellCoords.x() >= field.cols() || newCellCoords.y() >= field.rows()) {
			return true;
		}

		switch (getCell(newCellCoords)) {
			case CELL_EMPTY:
				getCell(snake.back()) = CELL_EMPTY;
				snake.pop_back();
				getCell(newCellCoords) = CELL_SNAKE;
				snake.emplace_front(newCellCoords);
				break;

			case CELL_SNAKE:
				return true;
			case CELL_FOOD:
				score++;
				spawnFood(randGen);
				snake.emplace_front(newCellCoords);
				getCell(newCellCoords) = CELL_SNAKE;

				break;
			default:
				assert(0 && "Field contains invalid cells.");
		}

		assert(snake.size() == score + 1);

		return false;
	}

	Game::Game(int width, int height, std::mt19937& randGen) : field(Eigen::MatrixXi::Zero(height, width)), score(0){
		reset(randGen);
		std::cout << "New Game!" << std::endl;
	}

	const Eigen::MatrixXi &Game::getField() {
		return field;
	}

	int &Game::getCell(const Eigen::Vector2i &coords) {
		return field(coords.y(), coords.x());
	}

	void Game::spawnFood(std::mt19937& randGen) {
		std::uniform_int_distribution<int> x_dist(1, field.cols() - 2);
		std::uniform_int_distribution<int> y_dist(1, field.rows() - 2);


		Eigen::Vector2i food_coords;

		do {
			food_coords = Eigen::Vector2i(x_dist(randGen), y_dist(randGen));
		} while (getCell(food_coords) != CELL_EMPTY);

		getCell(food_coords) = CELL_FOOD;
	}

	int Game::getScore() const {
		return score;
	}

	void Game::reset(std::mt19937& randGen) {

		snake.clear();
		field.setZero();
		score = 0;

		snake.emplace_front(field.cols()/2,field.rows()/2);
		getCell(snake.front()) = CELL_SNAKE;

		spawnFood(randGen);
	}
}
