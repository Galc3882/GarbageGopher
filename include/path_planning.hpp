// path_planning.hpp

#ifndef PATH_PLANNING_HPP
#define PATH_PLANNING_HPP

#include <vector>
#include <algorithm>
#include <queue>

// Define a simple Point structure to represent nodes or coordinates in the grid/map
struct Point {
    int x;
    int y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

// Node structure for A* search
struct Node {
    Point position;
    Node* parent;
    float gCost;
    float hCost;
    float fCost;
    Node(Point p, Node* _parent = nullptr) : position(p), parent(_parent), gCost(0), hCost(0), fCost(0) {}
};

class PathPlanner {
public:
    PathPlanner(int gridWidth, int gridHeight);
    
    // Setter function to set obstacles
    void setObstacle(Point obstacle);

    // A* search algorithm to find the shortest path between start and goal
    std::vector<Point> AStarSearch(Point start, Point goal);

    // Function to visualize the path
    void visualizePath(const std::vector<Point>& path);

private:
    std::vector<std::vector<int>> grid;

    // Helper functions
    float heuristic(Point start, Point goal);
    bool isValid(Point point);
    std::vector<Point> getNeighbors(Point current);
};

#endif // PATH_PLANNING_HPP
