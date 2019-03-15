#ifndef WORLDSIM_PATHING_HPP
#define WORLDSIM_PATHING_HPP

/* Pathing.hpp
	#include"Pathing.hpp"
  
  This class handles all pathing within the world and local maps. Pathing objects can automatically update
  and also be built incrementally over several turns, which can improve performance. Threading will also be an option
*/

#include <math.h>

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
    
//    bestNode=0;
    finalNode=0;
    
    finished=false;
    flipBest = false;
	}
  ~Pathing_Local()
  {
    for(int y=0;y<aNode.nY;++y)
    {
      for(int x=0;x<aNode.nX;++x)
      {
        delete aNode(x,y);
      }
    } 
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
        aNode(x,y)->created=false;
      }
    }
	}
  
  // Do a simple local A* path. _pathSize 0 = unlimited
  // pathAway true sets best nodes as those furthest away from target.
  bool pathLocal(int _startX, int _startY, int _endX, int _endY, int _pathSize=50, bool pathAway=false)
  {

    if ( map == 0) { return false; }
    if (map->isSafe (_startX,_startY) == false || map->isSafe (_endX, _endY) == false)
    { return false; }
  
    if ( pathAway )
    { //std::cout<<"Pathing away from: "<<_endX<<", "<<_endY<<".\n";
    }
    else
    { //std::cout<<"Pathing to: "<<_endX<<", "<<_endY<<".\n";
    }
  
  
		
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
    
    
    //pathAway=false;
    
    while (expandBest(pathAway) && finished==false && pathSize-- > 0)
    {
      //std::cout<<".";
    }
    
    #ifdef PATHING_MARKERS
    for (int i=0;i<vNode.size();++i)
    {
      map->put(new Item_Marker_Red  ,vNode(i)->x,vNode(i)->y);
    }
    #endif
    
    //if ( )
    //{
      //std::cout<<"Getting path\n";
      getPath(pathAway);
      //std::cout<<"Done\n";
    //}
    

    
    
    std::reverse(vPath.begin(), vPath.end());
    
    if (vPath.size() > 0 )
    {
      for (int i=0;i<vNode.size();++i)
      {
        if (vNode(i)->x==tX && vNode(i)->y==tY)
        {
          std::cout<<"Has target\n";
          return true;
        }
      }
      //std::cout<<"Final path:\n";
      //for (int i=0;i<vPath.size();++i) { std::cout<<vPath(i); } std::cout<<".\n";
      //return true;
    }
    std::cout<<"No path found\n";

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
			&& map->aLocalTile(pNodeX, pNodeY).canTravelNorth()
      && map->aLocalTile(pNodeX, pNodeY+1).hasMovementBlocker()==false )
		{
			aNode(pNodeX, pNodeY+1)->findDistance(_x,_y+1,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX, pNodeY+1));
      aNode(pNodeX, pNodeY+1)->created=true;
      
      if ( aNode(pNodeX, pNodeY+1)->parent == 0 )
      {
        aNode(pNodeX, pNodeY+1)->parent = aNode(_x,_y);
        aNode(pNodeX, pNodeY+1)->pathName = 'N';
      }
		}
    // SOUTH
		if(pNodeY>0 && aNode(pNodeX,pNodeY-1)->exhausted==false
			&& map->aLocalTile(pNodeX, pNodeY-1).canTravelNorth()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelSouth()
      && map->aLocalTile(pNodeX, pNodeY-1).hasMovementBlocker()==false )
		{
			aNode(pNodeX, pNodeY-1)->findDistance(_x,_y-1,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX, pNodeY-1));
      aNode(pNodeX, pNodeY-1)->created=true;

      if ( aNode(pNodeX, pNodeY-1)->parent == 0 )
      {
        aNode(pNodeX, pNodeY-1)->parent = aNode(_x,_y);
        aNode(pNodeX, pNodeY-1)->pathName = 'S';
      }
      
      
		}
    // EAST
		if(pNodeX<aNode.nX-1 && aNode(pNodeX+1,pNodeY)->exhausted==false
			&& map->aLocalTile(pNodeX+1, pNodeY).canTravelWest()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelEast()
      && map->aLocalTile(pNodeX+1, pNodeY).hasMovementBlocker()==false )
		{
			aNode(pNodeX+1, pNodeY)->findDistance(_x+1,_y,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX+1, pNodeY));
      aNode(pNodeX+1, pNodeY)->created=true;

      if ( aNode(pNodeX+1, pNodeY)->parent == 0 )
      {
        aNode(pNodeX+1, pNodeY)->parent = aNode(_x,_y);
        aNode(pNodeX+1, pNodeY)->pathName = 'E';
      }
		}
    // WEST
		if(pNodeX>0 && aNode(pNodeX-1,pNodeY)->exhausted==false
			&& map->aLocalTile(pNodeX-1, pNodeY).canTravelEast()
			&& map->aLocalTile(pNodeX, pNodeY).canTravelWest()
      && map->aLocalTile(pNodeX-1, pNodeY).hasMovementBlocker()==false )
		{
			aNode(pNodeX-1, pNodeY)->findDistance(_x-1,_y,sX,sY,tX,tY);
			vNode.push(aNode(pNodeX-1, pNodeY));
      aNode(pNodeX-1, pNodeY)->created=true;
      aNode(pNodeX-1, pNodeY)->parent = aNode(_x,_y);
      aNode(pNodeX-1, pNodeY)->pathName = 'W';
      
      if ( aNode(pNodeX-1, pNodeY)->parent == 0 )
      {
        aNode(pNodeX-1, pNodeY)->parent = aNode(_x,_y);
        aNode(pNodeX-1, pNodeY)->pathName = 'W';
      }
		}
	}
  void expandLocal (Node* pNode)
  {
    if (pNode==0) { return; }
    expandLocal(pNode->x,pNode->y);
  }
  
  bool expandBest(bool pathAway)
  {
    //std::cout<<"Expandbest\n";
    if (vNode.size() == 0) { return false; }
    
    int bestDistance = vNode(0)->distanceFromTarget;
    Node* _bestNode = 0;
    
    if (pathAway==false) /* Expand node closest to target */
    {
      //std::cout<<"pathto\n";
      for (int i=0;i<vNode.size();++i)
      {
        if (vNode(i)->exhausted==false && (_bestNode == 0 || vNode(i)->distanceFromTarget < bestDistance
        || (vNode(i)->distanceFromTarget == bestDistance && flipBest)))
        {
          flipBest = !flipBest;
          bestDistance = vNode(i)->distanceFromTarget;
          _bestNode = vNode(i);
        }
      }
      if (bestDistance==0)
      {
        finished=true;
        finalNode = _bestNode;
        //std::cout<<"finalnode: "<<finalNode->x<<", "<<finalNode->y<<".\n";
        return true;
      }
      if ( _bestNode!=0)
      {
        expandLocal(_bestNode);
        return true;
      }
      
    }
    else /* Expand node furthest from target */
    {
      //std::cout<<"pathaway\n";
      for (int i=0;i<vNode.size();++i)
      {
        if (vNode(i)->exhausted==false && (_bestNode == 0 || vNode(i)->distanceFromTarget > bestDistance
        || (vNode(i)->distanceFromTarget == bestDistance && flipBest)))
        {
          flipBest = !flipBest;
          bestDistance = vNode(i)->distanceFromTarget;
          _bestNode = vNode(i);
        }
        
      }
      if (vNode.size() > pathSize)
      {
        finished=true;
        return true;
      }
      if ( _bestNode!=0)
      {
        expandLocal(_bestNode);
        return true;
      }
    }
    

    

    return false;
  }
  
  void getPath(bool pathAway)
  //void getPath(Node* currentNode = 0)
  {
    //std::cout<<"Getpath\n";
    //if (currentNode == 0 ) {return;}
    if (vNode.size()==0 || vNode.size() == 1) { return; }
    
    
    //Node* currentNode = vNode(0);
    if (finalNode==0)
    {
      finalNode = vNode(vNode.size()-1);
    }
    
    Node* currentNode = finalNode;
    
    //std::cout<<"Backtracking from: "<<currentNode->x<<", "<<currentNode->y<<".\n";
    
    while(currentNode->parent!=0)
    {
      //std::cout<<"Pushing parent\n";
      
      vPath.push(currentNode->pathName);
      #ifdef PATHING_MARKERS
      map->put(new Item_Marker_Green_Small, currentNode->x, currentNode->y);
      #endif
      

      
      
      currentNode = currentNode->parent;
    }
    
    
    // //int bestDistance = currentNode->distanceFromTarget;
    
    // std::cout<<"Vnode size: "<<vNode.size()<<".\n";
    
    // if (pathAway) // Find node furthest from target.
    // {
      // std::cout<<"Pathing away\n";
      // for (int i=1;i<vNode.size();++i)
      // {
        // if (vNode(i)->distanceFromTarget > bestDistance)
        // {
          // currentNode = vNode(i);
          // bestDistance = vNode(i)->distanceFromTarget;
        // }
      // }
    // }
    // else // Find node closest to target
    // {
      // for (int i=1;i<vNode.size();++i)
      // {
        // if (vNode(i)->distanceFromTarget < bestDistance)
        // {
          // currentNode = vNode(i);
          // bestDistance = vNode(i)->distanceFromTarget;
        // }
      // }
    // }
    
    // //if (currentNode==0) { currentNode = aNode(tX,tY); }
    // if (currentNode==0) { return; }
    
    // int currentX = tX;
    // int currentY = tY;
    

    // while (currentNode->x != sX || currentNode->y != sY)
    // {
      // std::cout<<",";
      
      // bestDistance = -1;
      // char bestDir = 0;
    
    
      // if (aNode.isSafe(currentX-1,currentY) && aNode(currentX-1,currentY)->created
        // && (aNode(currentX-1,currentY)->distanceFromSource<bestDistance || bestDistance==-1)) // WEST
      // {
        // currentNode = aNode(currentX-1,currentY);
        // bestDistance = aNode(currentX-1,currentY)->distanceFromSource;
        // bestDir = 'E';
      // }
      // if (aNode.isSafe(currentX+1,currentY) && aNode(currentX+1,currentY)->created
        // && (aNode(currentX+1,currentY)->distanceFromSource<bestDistance || bestDistance==-1)) // WEST
      // {
        // currentNode = aNode(currentX+1,currentY);
        // bestDistance = aNode(currentX+1,currentY)->distanceFromSource;
        // bestDir = 'W';
      // }
      // if (aNode.isSafe(currentX,currentY-1) && aNode(currentX,currentY-1)->created
        // && (aNode(currentX,currentY-1)->distanceFromSource<bestDistance || bestDistance==-1)) // SOUTH
      // {
        // currentNode = aNode(currentX,currentY-1);
        // bestDistance = aNode(currentX,currentY-1)->distanceFromSource;
        // bestDir = 'N';
      // }
      // if (aNode.isSafe(currentX,currentY+1) && aNode(currentX,currentY+1)->created
        // && (aNode(currentX,currentY+1)->distanceFromSource<bestDistance || bestDistance==-1)) // SOUTH
      // {
        // currentNode = aNode(currentX,currentY+1);
        // bestDistance = aNode(currentX,currentY+1)->distanceFromSource;
        // bestDir = 'S';
      // }

      // vPath.push(bestDir);
      
      // currentX = currentNode->x;
      // currentY = currentNode->y;
      
      // if (bestDistance == 0)
      // {
        // return;
      // }
    // } std::cout<<"\n";
  }
};

#endif
