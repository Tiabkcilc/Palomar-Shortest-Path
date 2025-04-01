# Palomar-Shortest-Path
Palomar Campus Shortest Path is a C++ application that implements a graph data structure and uses Dijkstra’s algorithm to compute the shortest walking path between any two points of interest on Palomar's campus.

Overview
This project reads campus map data from a text file (palomarMap.txt) and uses a graph representation along with Dijkstra's algorithm to determine the optimal walking route between two specified points on campus. The solution is designed to help users navigate efficiently between locations by providing both the path and its total cost.

Features
Graph-Based Navigation: Models the campus as a graph where nodes represent points of interest and edges represent walkable paths with associated costs.

Dijkstra’s Algorithm: Implements the well-known Dijkstra’s algorithm to find the shortest (minimum cost) path between two nodes.

Customizable Input: Uses an input file (palomarMap.txt) for map data, allowing easy updates or modifications to campus layout.

Console Output: Displays the computed path as a sequence of nodes along with the total walking cost.
