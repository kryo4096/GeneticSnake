//
// Created by jonas on 7/3/21.
//

#include "SimpleActor.hpp"

void mutateMatrix(Eigen::MatrixXd& matrix, double stdDev, std::mt19937& randGen) {

	std::normal_distribution<double> distribution(0,stdDev);

	for(int i = 0; i< matrix.rows(); i++) {
		for(int j = 0; j < matrix.cols(); j++) {
			matrix(i,j) += distribution(randGen);
		}
	}
}

SimpleActor::SimpleActor(int gameWidth, int gameHeight, int memorySize, int intermediateSize) {
	V = Eigen::MatrixXd::Zero(intermediateSize, gameWidth * gameHeight);
	C = Eigen::MatrixXd::Zero(5 + memorySize, intermediateSize + memorySize);
	M = Eigen::MatrixXd::Identity(memorySize, memorySize);
	memory = Eigen::VectorXd::Zero(memorySize);
}

SimpleActor SimpleActor::mutate(double mutationRate, std::mt19937& randGen) const {
	SimpleActor actor(*this);

	mutateMatrix(actor.V, mutationRate,  randGen);
	mutateMatrix(actor.C, mutationRate, randGen);
	mutateMatrix(actor.M, mutationRate, randGen);

	actor.memory.setZero();

	return actor;
}

Eigen::VectorXd SimpleActor::react(const Eigen::MatrixXi &gameState) {
	Eigen::VectorXd g(Eigen::Map<const Eigen::VectorXi>(gameState.data(), gameState.cols() * gameState.rows()).cast<double>());

	Eigen::VectorXd im = V * g;
	Eigen::VectorXd m = M * memory;

	Eigen::VectorXd c(m.size() + im.size());

	c << im, m;

	Eigen::VectorXd out = C * c;

	memory = tanh(out.tail(memory.size()).array());
	return out.head(5);
}

void SimpleActor::outputToDirection(const Eigen::VectorXd& output, Eigen::Vector2i &direction) {
	Eigen::Vector2i dirs[5] = {{0,0}, {1,0}, {0,1}, {-1,0}, {0,-1}};

	int dir;

	output.maxCoeff(&dir);

	Eigen::Vector2i d = dirs[dir];

	if(dir != 0) {
		direction = d;
	}
}
