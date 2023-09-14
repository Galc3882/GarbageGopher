// path_planning.cpp

#include "path_planning.hpp"
#include <cmath>
#include <limits>
#include <iostream>

PathPlanner::PathPlanner(int gridWidth, int gridHeight) {
    grid.resize(gridHeight, std::vector<int>(gridWidth, 0));
}

void PathPlanner::setObstacle(Point obstacle) {
    if (isValid(obstacle)) {
        grid[obstacle.y][obstacle.x] = 1; // 1 indicates an obstacle
    }
}

float PathPlanner::heuristic(Point start, Point goal) {
    // Using Euclidean distance for heuristic
    return std::sqrt(std::pow(start.x - goal.x, 2) + std::pow(start.y - goal.y, 2));
}

bool PathPlanner::isValid(Point point) {
    return point.x >= 0 && point.x < grid[0].size() && point.y >= 0 && point.y < grid.size();
}

std::vector<Point> PathPlanner::getNeighbors(Point current) {
    std::vector<Point> neighbors;
    // Check 4 possible directions: up, down, left, right
    std::vector<Point> directions = { {0,1}, {0,-1}, {1,0}, {-1,0} };
    for (const auto& dir : directions) {
        Point next(current.x + dir.x, current.y + dir.y);
        if (isValid(next) && grid[next.y][next.x] == 0) {
            neighbors.push_back(next);
        }
    }
    return neighbors;
}

std::vector<Point> PathPlanner::AStarSearch(Point start, Point goal) {
    std::priority_queue<Node*, std::vector<Node*>, 
        [](Node* a, Node* b) -> bool { return a->fCost > b->fCost; }> openList;

    Node* startNode = new Node(start);
    Node* goalNode = new Node(goal);
    openList.push(startNode);

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        // Check if the goal is reached
        if (current->position.x == goalNode->position.x && current->position.y == goalNode->position.y) {
            std::vector<Point> path;
            while (current) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path; // Goal is found
        }

        // Expand neighbors
        for (const auto& neighborPos : getNeighbors(current->position)) {
            Node* neighbor = new Node(neighborPos, current);
            neighbor->gCost = current->gCost + 1; // 1 is the cost for moving from current to neighbor
            neighbor->hCost = heuristic(neighbor->position, goalNode->position);
            neighbor->fCost = neighbor->gCost + neighbor->hCost;

            // Push to open list
            openList.push(neighbor);
        }
    }

    return {}; // Return empty path if no path is found
}

void PathPlanner::visualizePath(const std::vector<Point>& path) {
    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[0].size(); ++x) {
            bool isPath = std::find(path.begin(), path.end(), Point(x, y)) != path.end();
            if (isPath) {
                std::cout << "X ";
            } else if (grid[y][x] == 1) {
                std::cout << "# ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}
