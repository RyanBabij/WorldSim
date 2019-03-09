#ifndef WORLDSIM_PATHING_HPP
#define WORLDSIM_PATHING_HPP

/* Pathing.hpp
	#include"Pathing.hpp"
  
  This class handles all pathing within the world and local maps. Pathing objects can automatically update
  and also be built incrementally over several turns, which can improve performance. Threading will also be an option
*/

#include <math.h>



class Pathing_Local
{

	struct Node
	{
		int x,y,z;
		int distanceFromTarget;
		int distanceFromSource;
		bool exhausted;
    bool created;
		
		Node()
		{
			exhausted=false;
			x=0; y=0;
			distanceFromTarget=0; distanceFromSource=0;
      created=false;
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

	Pathing_Local()
	{
		map=0;
		sourceOriginalX=0;
		sourceOriginalY=0;
    pathSize=0;
    sX=0;
    sY=0;
    tX=0;
    tY=0;
    
    finished=false;
    flipBest = false;
	}
	
	void init(World_Local* _map)
	{    
		map=_map;
    if (_map==0) { return; }
    
    if (map->aLocalTile.nX < 1 || map->aLocalTile.nY < 1)
    { return; }
    
		//Node* n = new Node;
		aNode.init(map->aLocalTile.nX,map->aLocalTile.nY,0);

    for(int y=0;y<aNode.nY;++y)
    {
      for(int x=0;x<aNode.nX;++x)
      {
        aNode(x,y) = new Node;
        aNode(x,y)->setCoords(x,y);
        aNode(x,y)->exhausted=false;
        aNode(x,y)->created=true;
      }
    }
	}
  
  // Do a simple local A* path. _pathSize 0 = unlimited
  bool pathLocal(int _startX, int _startY, int _endX, int _endY, int _pathSize=50)
  {

    if ( map == 0) { return false; }
    if (map->isSafe (_startX,_startY) == false || map->isSafe (_endX, _endY) == false)
    { return false; }
  
		
		sourceOriginalX=+_startX;
		sourceOriginalY=+_startY;
		
    pathSize = _pathSize;
    vPath.clear();
    vNode.clear();

		
		sX=_startX;
		sY=_startY;
		
		tX=_endX;
		tY=_endY;
    
    vNode.push(aNode(_startX,_startY));
    aNode(_startX,_startY)->findDistance(_startX,_startY,sX,sY,tX,tY);
    
    while (expandBest() && finished==false)
    {
    }
    
    if ( finished )
    {
      getPath();
    }
    
    for (int i=0;i<vNode.size();++i)
    {
      map->put(new Item_Marker_Red,vNode(i)->x,vNode(i)->y);
    }
    
    std::reverse(vPath.begin(), vPath.end());

    return false;
  }
	
  // Simple path basically just walks in the general direction of the target.
  // This is done before using the more expensive A*.
	bool appendSimplePath()
	{
		return false;
	}

  // This only expands local tiles for pathing. Path will only be able to consider tiles in the current map.
	void expandLocal(int _x, int _y)
	{
		aNode(_x,_y)->exhausted=true;
		const int pNodeX=_x;
		const int pNodeY=_y;
		
    // NORTH
		if(pNodeY<aNode.nY-1 && aNode(pNodeX,pNodeY+1)->exhausted==false
			&& map->aLocalTile(pNodeX, pNodeY+1).canTravelSouth()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelNorth())
		{
			aNode(pNodeX, pNodeY+1)->findDistance(_x,_y,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX, pNodeY+1));
      aNode(pNodeX, pNodeY+1)->created=true;
		}
    // SOUTH
		if(pNodeY>0 && aNode(pNodeX,pNodeY-1)->exhausted==false
			&& map->aLocalTile(pNodeX, pNodeY-1).canTravelNorth()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelSouth())
		{
			aNode(pNodeX, pNodeY-1)->findDistance(_x,_y,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX, pNodeY-1));
      aNode(pNodeX, pNodeY-1)->created=true;
		}
    // EAST
		if(pNodeX<aNode.nX-1 && aNode(pNodeX+1,pNodeY)->exhausted==false
			&& map->aLocalTile(pNodeX+1, pNodeY).canTravelWest()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelEast())
		{
			aNode(pNodeX+1, pNodeY)->findDistance(_x,_y,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX+1, pNodeY));
      aNode(pNodeX+1, pNodeY)->created=true;
		}
    // WEST
		if(pNodeX>0 && aNode(pNodeX-1,pNodeY)->exhausted==false
			&& map->aLocalTile(pNodeX-1, pNodeY).canTravelEast()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelWest())
		{
			aNode(pNodeX-1, pNodeY)->findDistance(_x,_y,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX-1, pNodeY));
      aNode(pNodeX-1, pNodeY)->created=true;
		}
	}
  void expandLocal (Node* pNode)
  {
    if (pNode==0) { return; }
    expandLocal(pNode->x,pNode->y);
  }
  
  bool expandBest()
  {
    if (vNode.size() == 0) { return false; }
    int bestDistance = vNode(0)->distanceFromTarget;
    Node* bestNode = 0;
    for (int i=0;i<vNode.size();++i)
    {
      if (vNode(i)->exhausted==false && (bestNode == 0 || vNode(i)->distanceFromTarget < bestDistance
      || (vNode(i)->distanceFromTarget == bestDistance && flipBest)))
      {
        flipBest = !flipBest;
        bestDistance = vNode(i)->distanceFromTarget;
        bestNode = vNode(i);
      }
    }
    
    if (bestDistance==0)
    {
      finished=true;
      return true;
    }
    
    if ( bestNode!=0)
    {
      expandLocal(bestNode);
      return true;
    }
    return false;
  }
  
  void getPath()
  {
    if (vNode.size()==0) { return; }
    
    Node* currentNode = aNode(tX,tY);
    int currentX = tX;
    int currentY = tY;

    while (currentNode->x != sX || currentNode->y != sY)
    {
      
      int bestDistance = -1;
      char bestDir = 0;
    
    
      if (aNode.isSafe(currentX-1,currentY) && aNode(currentX-1,currentY)->created
        && (aNode(currentX-1,currentY)->distanceFromSource<bestDistance || bestDistance==-1)) // WEST
      {
        currentNode = aNode(currentX-1,currentY);
        bestDistance = aNode(currentX-1,currentY)->distanceFromSource;
        bestDir = 'E';
      }
      if (aNode.isSafe(currentX+1,currentY) && aNode(currentX+1,currentY)->created
        && (aNode(currentX+1,currentY)->distanceFromSource<bestDistance || bestDistance==-1)) // WEST
      {
        currentNode = aNode(currentX+1,currentY);
        bestDistance = aNode(currentX+1,currentY)->distanceFromSource;
        bestDir = 'W';
      }
      if (aNode.isSafe(currentX,currentY-1) && aNode(currentX,currentY-1)->created
        && (aNode(currentX,currentY-1)->distanceFromSource<bestDistance || bestDistance==-1)) // SOUTH
      {
        currentNode = aNode(currentX,currentY-1);
        bestDistance = aNode(currentX,currentY-1)->distanceFromSource;
        bestDir = 'N';
      }
      if (aNode.isSafe(currentX,currentY+1) && aNode(currentX,currentY+1)->created
        && (aNode(currentX,currentY+1)->distanceFromSource<bestDistance || bestDistance==-1)) // SOUTH
      {
        currentNode = aNode(currentX,currentY+1);
        bestDistance = aNode(currentX,currentY+1)->distanceFromSource;
        bestDir = 'S';
      }

      vPath.push(bestDir);
      
      currentX = currentNode->x;
      currentY = currentNode->y;
      
      if (bestDistance == 0)
      {
        return;
      }
    }
  }
};

#endif
