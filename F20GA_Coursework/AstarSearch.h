#pragma once

#include <list>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

class aStar {
public:

	std::list<std::tuple<int, int>> closed;

	const int mapData[16][16] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};


	aStar(std::tuple<int, int> destination, std::tuple<int, int> currentLocation) {
		//For adjacent spaces, can you enter them? what is their heuristic cost, what is the current path cost
		std::vector<int> path;

		int tempMapData[16][16] = {
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		};

		while (destination != currentLocation) {
			printf("I'm still running....");
			//Get adjacent spaces
			std::vector<int> directions = getDirections(currentLocation);
			//Check heuristic values of each. tuple<0> is direction value, tuple<1> is heuristic value
			int directionToGo = checkHeuristicValues(destination, currentLocation, directions);
			path.push_back(directionToGo);

			//tempMapData[std::get<0>(currentLocation), std::get<1>(currentLocation)] = 1;

			currentLocation = updateCurrentLocation(currentLocation, directionToGo);
		}

		printf("PATH:\n");
		for (int i = 0; i < path.size(); i++) {
			printf("%d \n", path[i]);
		}
	}

	std::tuple<int, int> updateCurrentLocation(std::tuple<int, int> currentLocation, int direction) {

		int currentLocationX = std::get<0>(currentLocation);
		int currentLocationY = std::get<1>(currentLocation);

		switch (direction) {
		case 1:
			currentLocationX -= 1;
			break;
		case 2:
			currentLocationX += 1;
			break;
		case 3:
			currentLocationY += 1;
			break;
		case 4:
			currentLocationY -= 1;
			break;
		}

		return std::make_tuple(currentLocationX, currentLocationY);
	}

	int checkHeuristicValues(std::tuple<int, int> destination, std::tuple<int, int> currentLocation, std::vector<int> directions) {

		std::vector<std::tuple<int, int>> heuristics;
		for (int i = 0; i < directions.size(); i++) {
			int currentDirection = directions[i];
			int currentHeuristic = heuristicCost(destination, currentLocation, currentDirection);

			heuristics.push_back(std::make_tuple(currentDirection, currentHeuristic));
		}
		//Compare the costs of each one
		int directionToGo = 0;
		int currentCost = 100;
		for (int i = 0; i < heuristics.size(); i++) {
			if (std::get<1>(heuristics[i]) < currentCost) {

				std::tuple<int, int> closedSpace = updateCurrentLocation(currentLocation, directionToGo);
				closed.push_back(closedSpace);

				currentCost = std::get<1>(heuristics[i]);
				directionToGo = std::get<0>(heuristics[i]);
			}
			else {
				std::tuple<int, int> closedSpace = updateCurrentLocation(currentLocation, std::get<0>(heuristics[i]));
				closed.push_back(closedSpace);
			}
		}

		return directionToGo;
	}

	std::vector<int> getDirections(std::tuple<int, int> currentLocation) {
		
		bool left = (mapData[std::get<0>(currentLocation) - 1, std::get<1>(currentLocation)] == 0);
		bool right = (mapData[std::get<0>(currentLocation) + 1, std::get<1>(currentLocation)] == 0);
		bool up = (mapData[std::get<0>(currentLocation), std::get<1>(currentLocation) + 1] == 0);
		bool down = (mapData[std::get<0>(currentLocation), std::get<1>(currentLocation) - 1] == 0);

		std::vector<int> directions;

		if (left) directions.push_back(1);
		if (right) directions.push_back(2);
		if (up) directions.push_back(3);
		if (down) directions.push_back(4);

		return directions;
	}


	int heuristicCost(std::tuple<int, int> destination, std::tuple<int, int> currentLocation, int direction) {
		int destinationX = std::get<0>(destination);
		int destinationY = std::get<1>(destination);
		int currentLocationX = std::get<0>(currentLocation);
		int currentLocationY = std::get<1>(currentLocation);

		switch (direction) {
		case 1:
			currentLocationX -= 1;
			break;
		case 2:
			currentLocationX += 1;
			break;
		case 3:
			currentLocationY += 1;
			break;
		case 4:
			currentLocationY -= 1;
			break;
		}

		return sqrt(((destinationX - currentLocationX)*(destinationX - currentLocationX)) + ((destinationY - currentLocationY)*(destinationY - currentLocationY)));
	}
};
