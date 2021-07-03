//
// Created by jonas on 7/3/21.
//

#ifndef GENETICSNAKE_SIMPLEACTOR_HPP
#define GENETICSNAKE_SIMPLEACTOR_HPP


#include <Eigen/Dense>
#include <random>
class SimpleActor {
	Eigen::MatrixXd V;
	Eigen::MatrixXd M;
	Eigen::MatrixXd C;

	Eigen::VectorXd memory;

public:

	SimpleActor mutate(double mutationRate, std::mt19937& randGen) const;

	Eigen::VectorXd react(const Eigen::MatrixXi& gameState);

	SimpleActor(int gameWidth, int gameHeight, int memorySize, int intermediateSize);

	static void outputToDirection(const Eigen::VectorXd& output, Eigen::Vector2i& direction);
};



#endif //GENETICSNAKE_SIMPLEACTOR_HPP
