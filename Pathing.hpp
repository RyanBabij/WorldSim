#ifndef WORLDSIM_PATHING_HPP
#define WORLDSIM_PATHING_HPP

/* Pathing.hpp
	#include"Pathing.hpp"
  
  This class handles all pathing within the world and local maps. Pathing objects can automatically update
  and also be built incrementally over several turns, which can improve performance. Threading will also be an option
*/

#include "World_Local.hpp"

#include <math.h>
#include "Item.hpp"

//#define PATHING_MARKERS

class Pathing_Local
{

	struct Node
	{
		int x,y,z;
		int distanceFromTarget;
		int distanceFromSource;
		bool exhausted;
		bool created;

		Node* parent;
		char pathName;

		Node()
		{
			exhausted=false;
			x=0; y=0;
			distanceFromTarget=0; distanceFromSource=0;
			created=false;

			parent=0;
			pathName=0;
		}
		void findDistance(int _currentX, int _currentY, int _sX, int _sY, int _tX, int _tY)
		{
			/*manhattan distance*/
			distanceFromTarget=abs((_currentX-_tX))+abs((_currentY-_tY));
			distanceFromSource=abs((_currentX-_sX))+abs((_currentY-_sY));
		}
		void setCoords(const int _x, const int _y)
		{
			x=_x;
			y=_y;
		}
	};

	World_Local* map;

	ArrayS2 <Node*> aNode;
	Vector <Node*> vNode;
	/* The last node which was closest to/furthest from the target. Useful for providing incomplete solution. */
	// For now we find it from the vector. In future we might track it.
	///Node* bestNode;


	Node* finalNode;

	int sourceOriginalX;
	int sourceOriginalY;

	int pathSize;

	int sX;
	int sY;

	int tX;
	int tY;


	bool finished;
	bool flipBest; /* Prevent always going a single direction. */


	public:
  
	Vector <char> vPath;

	Pathing_Local();
	~Pathing_Local();

	void init(World_Local* _map);

	// Do a simple local A* path. _pathSize 0 = unlimited
	// pathAway true sets best nodes as those furthest away from target.
	bool pathLocal(int _startX, int _startY, int _endX, int _endY, int _pathSize=50, bool pathAway=false);

	// Simple path basically just walks in the general direction of the target.
	// This is done before using the more expensive A*.
	bool appendSimplePath();

	// This only expands local tiles for pathing. Path will only be able to consider tiles in the current map.
	void expandLocal(int _x, int _y);

	void expandLocal (Node* pNode);

	bool expandBest(bool pathAway);

	void getPath(bool pathAway);
	void printPath();

};

#endif
