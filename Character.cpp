#pragma once
#ifndef WORLDSIM_CHARACTER_CPP
#define WORLDSIM_CHARACTER_CPP

/* WorldSim: Character
  #include "Character.cpp"

  Implementation of Character.hpp
*/

#include "Social.cpp"

#include "World.hpp"

#include "Character.hpp"
#include "Character_Knowledge.hpp"
#include "Tribe.hpp"

const int MAX_CHILDREN = 5;

//#include <Graphics/Texture/Texture.hpp>
class Texture;

Character::Character(): social(this)
{
	firstName="";
	lastName="";
	epithet="";

	/* Give every character a unique number */
	static long unsigned int STATIC_ID = 0;
	id = STATIC_ID++;

	isMale = true;
	age = -1;
	daysCounter=0;
	tickCounter=0;

	isAlive=true;
	isMarried=false;
	isPregnant=false;
	pregnantCounter=0;

	actionPoints = 0;
	money=0;

	father=0;
	mother=0;
	spouse = 0;
	
	skillFishing=0;
	skillMarksmanship=0;
	skillMining=0;
	skillFarming=0;
	skillMetalsmithing.init("metalsmithing",100);

	health=0;
	maxHealth=0;
	hunger=0;
	thirst=0;

	birthLocation.setXY(0,0);
	deathLocation.setXY(0,0);

	tribe = 0;

	causeOfDeath = UNKNOWN;

	isFavourite=false;

	knowledge=0;

	worldX = -1;
	worldY = -1;

	idleCounter=0;

	isSneaking=false;

	skillFishing=0;
	skillMarksmanship=0;

	nPelt = 1;
	nMeat = 1;
	//enum enumCauseOfDeath { UNKNOWN=0, STARVATION=1, OLD_AGE=2 };

	map = 0;

	isUnderground=false;
	
}

//_sex: 0 - Roll, 1 - Male, 2 - Female.
void Character::init(const int _sex /* =0 */)
{

	if (_sex == 1) {
		isMale = true;
	}
	else if (_sex == 2) {
		isMale = false;
	}
	else {
		isMale = Random::flip();
	}

	firstName = globalNameGen.generate();
	lastName = globalNameGen.generate();
	age=0;

	// There will be some differences in stats between genders, but
	// both genders will be able to potentially obtain 100 on every stat.
	// This is done to affect high-level gameplay, for example gender
	// distribution in battles.
	// TODO: Stats should be affected by age. However that will be calculated
	// by using age as a modifier. Not by modifying the base stat.

	baseSkill.roll(isMale);

	health=10;
	maxHealth=10;
	hunger=0;
	thirst=0;

	tribe=0;
	civ=0;
	settlement=0;

	isSneaking=false;

	dateOfBirth.set(&globalCalendar);

	//knowledge = new Character_Knowledge;
	//knowledge->init();
	initialiseKnowledge();
	
	social.setCompatibility(globalRandom.rand8(),globalRandom.rand8());
	
}

char Character::getBaseSkill(AttributeManager::TYPE skill)
{
	return baseSkill.getSkillValue(skill);
}

void Character::setBaseSkill(AttributeManager::TYPE skill, char value)
{
	baseSkill.setSkillValue(skill,value);
}

void Character::skillUpFarming()
{
	if ( skillFarming < 100 )
	{
		++skillFarming;
	}
}

void Character::skillUpMining()
{
	if ( skillMining < 100 )
	{
		++skillMining;
	}
}

char Character::getCharisma()
{
	return baseSkill.getSkillValue(AttributeManager::CHARISMA);
}

void Character::setCharisma(char amount)
{
	baseSkill.setSkillValue(AttributeManager::CHARISMA,amount);
}

char Character::getIntelligence()
{
	return baseSkill.getSkillValue(AttributeManager::INTELLIGENCE);
}

void Character::setIntelligence(char amount)
{
	baseSkill.setSkillValue(AttributeManager::INTELLIGENCE,amount);
}

char Character::getStrength()
{
	return baseSkill.getSkillValue(AttributeManager::STRENGTH);
}

void Character::setStrength(char amount)
{
	baseSkill.setSkillValue(AttributeManager::STRENGTH,amount);
}

Vector <Character*> Character::getAllKnownCharacters()
{
	Vector <Relationship> vRelation = social.getAcquaintances();
	
	Vector <Character*> vChar;
	for (int i=0;i<vRelation.size();++i)
	{
		vChar.push(vRelation(i).destinationCharacter);
	}
	
	return vChar;
}

bool Character::hasIdea(Idea idea)
{
	for (int i=0;i<vIdea.size();++i)
	{
		if (vIdea(i).id == idea.id)
		{
			return true;
		}
	}
	
	return false;
}
void Character::giveIdea(Idea idea)
{
	Idea copyIdea = idea;
	vIdea.push(copyIdea);
}

void Character::updateSocial()
{
	social.updateLists(getBaseSkill(AttributeManager::CHARISMA));
	//social.updateLists(1);
}

void Character::shareIdeas(Character* c)
{
	//std::cout<<"Sharing ideas\n";
	
	if (vIdea.empty())
	{
		//std::cout<<"No ideas to share\n";
		return;
	}
	
	// pick random idea to share
	
	int ideaToShare=0;
	
	if (vIdea.size()>1)
	{
		ideaToShare=globalRandom.rand(vIdea.size()-1);
	}
	
	if ( c->hasIdea(vIdea(ideaToShare)))
	{
		//std::cout<<"Already knows\n";
		return;
	}
	//std::cout<<"Idea shared\n";
	c->giveIdea(vIdea(ideaToShare));
	
}

//ITEM FUNCTIONS

void Character::giveItem(Item* _item)
{
	if (_item==0) {
		return;
	}
	vInventory.push(_item);
	//world(worldX,worldY)->put(_item,x,y);
	//world(worldX,worldY)->vItem.push(_item);
}

void Character::giveMoney(int amount)
{
	money+=amount;
}

int Character::getMoney()
{
	return money;
}

bool Character::takeMoney(int amount)
{
    // Check if the character has enough money
    if (money >= amount)
    {
        // Subtract the amount from the character's money
        money -= amount;
        // Return true to indicate success
        return true;
    }
    else
    {
        // Return false if the character does not have enough money
        return false;
    }
}


void Character::takeItem(Item* _item)
{
	if (_item==nullptr)
	{ return; }
	vInventory.remove(_item);
	//world(worldX,worldY)->vItem.push(_item);
}

void Character::consume(Item* _item)
{
	Console("NUM NUM NUM");
	hunger-=_item->hungerRestore;
	if (hunger<0) {
		hunger=0;
	}
	world(worldX,worldY)->erase(_item);

	// We should probably consider ways to avoid this check, because it is rarely true.
	// However the only possibility I can think of is making the player their own special class.
	if (this == playerCharacter)
	{
		removeFromInventoryGrid(_item);
	}
}

void Character::removeFromInventoryGrid(Item* _item) /* Player-only function */
{
	for (int _y=0; _y<10; ++_y)
	{
		for (int _x=0; _x<10; ++_x)
		{
			if (inventoryGrid[_x][_y]==_item)
			{
				inventoryGrid[_x][_y]=0;
			}
		}
	}
}

bool Character::hasItemType(ItemType type)
{
	for (int i=0;i<vInventory.size();++i)
	{
		if ( vInventory(i)->type == type )
		{
			return true;
		}
	}
	return false;
}

Item* Character::getBestItemFor(enumJob job)
{
	if ( job == JOB_FARMING )
	{
		Item* bestItem = nullptr;
		int bestFarmingValue = -1;
		
		for (int i=0;i<vInventory.size();++i)
		{
			if ( vInventory(i)->farmingValue > bestFarmingValue)
			{
				bestItem=vInventory(i);
				bestFarmingValue = bestItem->farmingValue;
			}
		}
		return bestItem;
	}
	else if ( job == JOB_MINING )
	{
		Item* bestItem = nullptr;
		int bestMiningValue = -1;
		
		for (int i=0;i<vInventory.size();++i)
		{
			if ( vInventory(i)->miningValue > bestMiningValue)
			{
				bestItem=vInventory(i);
				bestMiningValue = bestItem->miningValue;
			}
		}
		return bestItem;
	}
	
	return nullptr;
}

std::string Character::getFullName() const
{
	return firstName + " " + lastName;
}

#include <sstream>

std::string Character::getBiography()
{
	std::ostringstream biography;

	const std::string gender1 = isMale ? "He" : "She";
	const std::string tense = isAlive ? "is" : "was";

	biography << getFullName();

	if (father == nullptr || mother == nullptr)
	{
		biography << " was divinely conceived.";
	}
	else
	{
		biography << " was the child of " << father->getFullName() << " and " << mother->getFullName() << ".";
		biography << " was born in BIOME, in the land of LAND.";
	}

	biography << " " << gender1 << " " << tense << (isMarried ? " married to X." : " not married.");
	
	if (tribe!=0)
	{
		biography<<" "<<gender1<<" is a member of a tribe.\n";
	}
	if (settlement!=0)
	{
		if ( settlement->government.leader == this )
		{
			biography<<" "<<gender1<<" is the leader of a settlement.\n";
		}
		else if ( settlement->government.scribe == this )
		{
			biography<<" "<<gender1<<" is the head scribe of a settlement.\n";
		}
		else if ( settlement->government.captain == this )
		{
			biography<<" "<<gender1<<" is the military captain of a settlement.\n";
		}
		else
		{
			biography<<" "<<gender1<<" is a member of a settlement.\n";
		}
		
	}
	
	

	if (!isAlive)
	{
		if ( causeOfDeath == SMITED )
		{
			biography << " " << gender1 << " died in " << world.getLandmassName(&deathLocation) << " after saying "<<
			"\"if God is real then may he strike me down right here\", upon which "<<gender1<<" was subsequently "<<
			"struck dead by a bolt of lightning.";
		}
		else
		{
			biography << " " << gender1 << " died in " << world.getLandmassName(&deathLocation) << ".";
		}
	}
	else
	{
		biography << " " << gender1 << " currently lives in " << world.getLandmassName(tribe->worldX, tribe->worldY) << ".";
	}
	
	if ( vIdea.size() > 0 )
	{
		biography <<" "<<gender1<<" currently has "<<vIdea.size()<<" ideas.\n";
	}
	
	if ( vOriginalSpecialIdea.size() > 0 )
	{
		for (int i=0;i<vOriginalSpecialIdea.size();++i)
		{
			biography <<gender1<<" "<<vOriginalSpecialIdea(i).biographyText<<".\n";
		}
	}
	
	biography<<"\n\n"<<getBestSkills()<<"\n";

	if (!vKills.empty())
	{
		biography << "\n\nKills: " << vKills.size() << "\n";
		for (const auto& kill : vKills)
		{
			biography << "   " << kill->getFullName() << "\n";
		}
	}

	return biography.str();
}

std::string Character::getBestSkills()
{
	std::vector<std::string> notableSkills;
	auto addSkillDescription = [&notableSkills](const std::string& skillName, char skillLevel)
	{
		if (skillLevel > 6)
		{
			std::string description;
			if (skillLevel == 10)
			{
				description = "legendary for their " + skillName;
			}
			else if (skillLevel == 9)
			{
				description = "renowned for their " + skillName;
			}
			else
			{
				description = "admired for their " + skillName;
			}
			notableSkills.push_back(description);
		}
	};

	addSkillDescription("strength", baseSkill.strength);
	addSkillDescription("agility", baseSkill.agility);
	addSkillDescription("charisma", baseSkill.charisma);
	addSkillDescription("intelligence", baseSkill.intelligence);
	addSkillDescription("perception", baseSkill.perception);
	addSkillDescription("endurance", baseSkill.endurance);
	addSkillDescription("courage", baseSkill.courage);

	if (notableSkills.empty())
	{
		return "This character has no notable skills.";
	}
	else
	{
		std::string description = "This character is ";
		for (size_t i = 0; i < notableSkills.size(); ++i)
		{
			if (i > 0)
			{
				if (i == notableSkills.size() - 1)
				{
					description += " and ";
				}
				else
				{
					description += ", ";
				}
			}
			description += notableSkills[i];
		}
		description += ".";
		return description;
	}
}


void Character::aiManager()
{
	// assess situation and decide current ai state
	//std::cout<<"AI manager for: "<<getFullName()<<"\n";
}

void Character::incrementTicks(int nTicks)
{
	aiManager();

	// character ai goes here
	// character could be doing one of several things:
	// crafting, making useful equipment
	// gathering resources
	// hunting
	//looking after family
	// combat
	// research


	tickCounter+=nTicks;

	hunger+=nTicks;
	if (hunger > MAX_HUNGER) {
		hunger = MAX_HUNGER; /* isAlive=false; */
	}

	thirst+=nTicks;
	if (thirst > MAX_THIRST) {
		thirst = MAX_THIRST; /* isAlive=false; */
	}

	while(tickCounter>=TICKS_PER_DAY)
	{
		++daysCounter;
		tickCounter-=TICKS_PER_DAY;
	}
	//std::cout<<"Dayscounter: "<<daysCounter<<".\n";
	while(daysCounter >= DAYS_PER_YEAR)
	{
		age++;
		daysCounter-=DAYS_PER_YEAR;

		// if ( hunger > 0 )
		// {
		// // Chance of dying at 100 hunger = 1 in 100.
		// // Chance of dying at 1 hunger = 1 in 5000
		// int starvationChance = (101-hunger)*10;

		// if ( Random::oneIn(starvationChance) )
		// {
		// //die();
		// }
		// }
	}

	wander();
}

void Character::wander()
{
	//int currentX = x;
	//int currentY = y;

	if (playerCharacter == this)
	{
		return;
	}

	if (map==0)
	{
		//std::cout<<"Error, character has no map.\n";
		return;
	}
	else
	{
		//std::cout<<"CHARMAP\n";
	}

	

	if (globalRandom.flip())
	{
		// do nothing (50% chance)
	}
	else if (globalRandom.flip())
	{
		map->remove(this);
		// alter x (25% chance)
		if (globalRandom.flip())
		{
			if (x<LOCAL_MAP_SIZE-1)
			{
				map->put(this,x+1,y);
				// ++x;
				// ++fullX;
			}
		}
		else
		{
			if (x > 0)
			{
				map->put(this,x-1,y);
				// --x;
				// --fullX;
			}
		}
	}
	else
	{
		map->remove(this);
		// alter y (25% chance)
		if (globalRandom.flip())
		{
			if (y<LOCAL_MAP_SIZE-1)
			{
				map->put(this,x,y+1);
				// ++y;
				// ++fullY;
			}
		}
		else
		{
			if (y > 0)
			{
				map->put(this,x,y-1);
				// --y;
				// --fullY;
			}
		}
	}
	updateKnowledge();


	return;
	if ( map==0 ) {
		return;
	}

	int newX = x;
	int newY = y;
	char moveDirection = '?';



	if (knowledge)
	{
		// PICK A DESTINATION IF NECESSARY
		if (map->isSafe(&(knowledge->currentGoal))==false ||
		      (knowledge->currentGoal.x == x && knowledge->currentGoal.y ==y))
		{
			HasXY* randomDestination = map->getRandomTile();

			knowledge->currentGoal.set(randomDestination);

			Pathing_Local p;
			p.init(map);
			p.pathLocal(x, y, randomDestination->x, randomDestination->y, 10, false);

			if (p.vPath.size() > 0)
			{
				knowledge->vPath = p.vPath;
				moveDirection=p.vPath.back();
				p.vPath.popBack();
			}
			else // Go somewhere else.
			{	knowledge->currentGoal.set(-1,-1);
			}

			delete randomDestination;
		}
		else
		{
			Pathing_Local p;
			p.init(map);
			bool pathingSuccess = p.pathLocal(x, y, knowledge->currentGoal.x, knowledge->currentGoal.y, 10, false);

			if (pathingSuccess == false && p.vPath.size() < 9 )
			{	knowledge->currentGoal.set(-1,-1);
			}


			if (p.vPath.size() > 0)
			{
				moveDirection=p.vPath(0);
			}
			else {
				knowledge->currentGoal.set(-1,-1);
			}
		}
	}
	else {
		std::cout<<"noknow\n";
	}

	int direction = Random::randomInt(3);

	if ( moveDirection == 'E' )
	{
		direction = 0;
	}
	else if (moveDirection == 'N')
	{
		direction = 2;
	}
	else if (moveDirection == 'S')
	{
		direction = 3;
	}
	else if (moveDirection == 'W')
	{
		direction = 1;
	}

	if ( direction==0 ) {
		++newX;
	}
	else if ( direction==1 ) {
		--newX;
	}
	else if ( direction==2 ) {
		++newY;
	}
	else {
		--newY;
	}

	if ( map->isSafe(newX,newY) && map->data->aLocalTile(newX,newY).hasMovementBlocker() == false )
	{
		map->remove(this);
		if (map->put(this,newX,newY) == false)
		{
			map->put(this,x,y);
		}

		if (Random::oneIn(10))
		{
			//delete map->aLocalTile(x,y).footprint;
			//map->aLocalTile(x,y).footprint = new Creature_Footprint;
		}
	}
	//updateKnowledge();

}

void Character::die(enumCauseOfDeath _causeOfDeath /* =UNKNOWN */)
{
	isAlive = false;
	dateOfDeath.set(&globalCalendar);
	causeOfDeath = _causeOfDeath;

	if ( tribe != 0 )
	{
		deathLocation.setXY(tribe->worldX,tribe->worldY);
	}
	else
	{
		std::cout<<"no tribe...\n";
	}
}

bool Character::marry(Character* c)
{
	if (c==0) {
		return false;
	}

	isMarried = true;
	c->isMarried=true;
	spouse = c;
	c->spouse = this;

	if (isMale == true && c->isMale == false)
	{
		c->lastName = lastName;
	}
	else if (isMale == false && c->isMale == true)
	{
		lastName = c->lastName;
	}

	dateOfMarriage.set(&globalCalendar);
	c->dateOfMarriage.set(&globalCalendar);

	// If the spouse is from a different tribe, move them to the new one.
	if ( tribe != c->tribe )
	{
		c->tribe->removeCharacter(c);
		tribe->addCharacter(c);
	}
	
	social.addFamily(c);
	c->social.addFamily(this);

	return true;
}

Vector <Character*> * Character::getDescendants(Vector <Character*> * vDescendants)
{
	if (vDescendants==0) {
		return 0;
	}

	/* GET ALL CHILDREN, THEN GET ALL CHILDREN'S CHILDREN. RETURN 0 IF NO CHILDREN. */
	if (vChildren.size() == 0)
	{
		return vDescendants;
	}

	//Vector <Character*> * vChildren = new Vector <Character*>;
	//auto vDescendants = new Vector <Character*>;

	// for (auto i=0;i<vChildren.size();++i)
	// {
	// }

	//for(auto const& value: vChildren.data) { }
	//for(auto const& v: vChildren) { std::cout<<"Person: "<<v->firstName<<".\n"; }


	// Push all character's children onto vector.
	for(auto const& v: vChildren) {
		vDescendants->push(v);
		v->getDescendants(vDescendants);
	}


	//Push all children's descendants onto vector.
	//for(auto const& v: vChildren) { v->getDescendants(vDescendants); }

	vDescendants->removeDuplicates();

	return vDescendants;
}

Vector <Character*> * Character::getRelatives()
{
	Vector <Character*> vCloseAncestors;

	//auto vCloseAncestors = new Vector <Character*>;
	auto vAncestors = new Vector <Character*>;

	// The character cannot marry any descendants of their grandparents. So first we get all 4 grandparents. If there are no grandparents, we work from parents. If there are no parents, we work from this.
	if ( father != 0)
	{
		if (father->father != 0)
		{
			vCloseAncestors.push(father->father);
		}
		if(father->mother != 0)
		{
			vCloseAncestors.push(father->mother);
		}
		if ( father->father == 0 && father->mother == 0)
		{
			vCloseAncestors.push(father);
		}
	}
	else
	{
		vCloseAncestors.push(this);
	}
	if ( mother != 0)
	{
		if (mother->father != 0)
		{
			vCloseAncestors.push(father->father);
		}
		if(mother->mother != 0)
		{
			vCloseAncestors.push(mother->mother);
		}
		if ( mother->father == 0 && mother->mother == 0)
		{
			vCloseAncestors.push(mother);
		}
	}
	else
	{
		vCloseAncestors.push(this);
	}

	// Get all descendants of all these relatives.
	for(auto const& v: vCloseAncestors) {
		vAncestors->push(v);
		v->getDescendants(vAncestors);
	}

	vAncestors->removeDuplicates();

	return vAncestors;
}

void Character::starve()
{
	//--health;
	++hunger;

	if (hunger > 100)
	{
		hunger = 100;
	}

	//if ( health <= 0)
	//{
	//std::cout<<"Starved to death.\n";
	//	isAlive = false;
	//}
}

bool Character::canMarry(Character* c)
{
	//std::cout<<"Checking if can marry.\n";


	//Cannot marry null pointer or yourself or underage or same sex.
	if(c==0 || c==this || isAlive==false || c->isAlive==false || age<16 || c->age<16 || isMale==c->isMale || spouse !=0 || c->spouse!=0 ) {
		return false;
	}

	//auto vRelatives = getRelatives();
	//if ( vRelatives->contains(c) )
	//{
	//	delete vRelatives;
	//	return false;
	//}

	return true;
}

Character* Character::giveBirth()
{

	Character * babby = new Character;
	babby->init();
	babby->lastName = lastName;
	// Inherit tribe membership from mother I guess.
	babby->tribe = tribe;
	babby->settlement = settlement;

	vChildren.push(babby);
	spouse->vChildren.push(babby);

	if(isMale)
	{
		babby->father=this;
		if ( spouse!=0 )
		{
			babby->mother=this->spouse;
		}
	}
	else
	{
		babby->mother=this;
		if ( spouse!=0 )
		{
			babby->father=this->spouse;
		}
	}

	return babby;
}

void Character::attack(Character* target)
{
	if (target == 0 || target->isAlive == false ) {
		return;
	}
	//For now we will just use strength to calculate outcome.

	if ( baseSkill.strength >= target->baseSkill.strength && Random::flip() )
	{


		//std::cout<<firstName<<" killed "<<target->firstName<<".\n";
		target->die(COMBAT);
		vKills.add(target);
		return;
	}
	else if ( baseSkill.strength < target->baseSkill.strength )
	{
		if ( Random::flip() )
		{
			//std::cout<<target->firstName<<" killed "<<firstName<<" in self-defence.\n";
			target->vKills.add(this);
			die(COMBAT);
		}
		else
		{
		}

	}
	//std::cout<<"Attack missed.\n";


}

//Get attacked by a Creature
void Character::getAttacked (Creature* _creature, Creature_Attack* _atk)
{
	if ( health == 0 ) {
		return;
	}

	if ( health > 0 ) {
		--health;
	}

	if (health==0)
	{
		isAlive=false;
		Console("You are kill");
	}
}







// LOCATION

bool Character::isOnMap(int _mapX, int _mapY)
{
	//For now we are assuming the Character is on the same map as his tribe.

	return (worldX == _mapX && worldY == _mapY);

	// if ( tribe != 0 )
	// {
	// return tribe->isOnMap(_mapX,_mapY);
	// }

	// return false;
}


// KNOWLEDGE

void Character::initialiseKnowledge()
{
	if ( knowledge == 0 )
	{
		knowledge = new Character_Knowledge;
		knowledge->init();
	}
}

char Character::hasSeen( World_Local* _map, int _x, int _y, bool isSubterranean )
{
	if (knowledge==0) {
		return false;
	}


	return ( knowledge->hasSeen(_map,_x,_y,isSubterranean) );
}


// reveal the map within the given area, mostly for debugging
// setting it too high probably does bad things
void Character::revealMap(unsigned int _range)
{
	if ( knowledge == 0 ) {
		return;
	}
	if ( tribe == 0 ) {
		return;
	}

	idleCounter=0;

	unsigned long int startX = fullX;
	unsigned long int endX = fullX+_range;

	unsigned long int startY = fullY;
	unsigned long int endY = fullY+_range;

	if ( startX < _range )
	{
		startX = 0;
	}
	else
	{
		startX = fullX-_range;
	}


	if ( startY < _range )
	{
		startY = 0;
	}
	else
	{
		startY = fullY-_range;
	}


	if (map)
	{

		for (unsigned long int _y=startY; _y<endY; ++_y)
		{
			for (unsigned long int _x=startX; _x<endX; ++_x)
			{
				if (world.isSafe(_x,_y))
				{
					knowledge->addTile(_x,_y);
				}
			}
		}
	}

}

//Update knowledge with current instance.
void Character::updateKnowledge()
{
	if ( knowledge == 0 ) {
		return;
	}
	if ( tribe == 0 ) {
		return;
	}

	idleCounter=0;

	//learn about all surrounding tiles.

	// The basic 3x3 box of guaranteed sight.


	//For now this simply wipes LOS from last turn.
	//knowledge->updateLOS();

	//knowledge->addTile(world(worldX,worldY), x,y);

	//std::cout<<"Adding tile: "<<fullX<<", "<<fullY<<"\n";
	knowledge->addTile(fullX,fullY,isUnderground);

	if (playerCharacter!=this)
	{
		return;
	}

	// knowledge->addTile(tribe->getCurrentMap(), x-1,y);
	// knowledge->addTile(tribe->getCurrentMap(), x+1,y);
	// knowledge->addTile(tribe->getCurrentMap(), x,y-1);
	// knowledge->addTile(tribe->getCurrentMap(), x-1,y-1);
	// knowledge->addTile(tribe->getCurrentMap(), x+1,y-1);
	// knowledge->addTile(tribe->getCurrentMap(), x,y+1);
	// knowledge->addTile(tribe->getCurrentMap(), x-1,y+1);
	// knowledge->addTile(tribe->getCurrentMap(), x+1,y+1);

	// Now we need to run a proper line of sight algorithm. We can start with raytrace.
	//MAX_VIEW_RANGE

	// for (int i=0;i<MAX_VIEW_RANGE;++i)
	// {
	// int actualTileX = x+i;

	// if (tribe->getCurrentMap()->isBlockingView(x+i,y))
	// {
	// break;
	// }
	// //knowledge->addTile(tribe->getCurrentMap(), x+i,y);

	// }


	//Vector <HasXY*> * vVisibleTiles = world(worldX,worldY)->rayTraceLOS(x,y,MAX_VIEW_RANGE);
	//Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world(worldX,worldY)->rayTraceLOS(fullX,fullY,MAX_VIEW_RANGE);

	//std::cout<<"coordinates to raytrace from: "<<fullX<<", "<<fullY<<".\n";

	Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(fullX,fullY,MAX_VIEW_RANGE/2,false,isUnderground);

	if ( vVisibleTiles!=0 )
	{
		for (int i=0; i<vVisibleTiles->size(); ++i)
		{
			//std::cout<<"ADDING\n";
			knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
			delete (*vVisibleTiles)(i);
		}
	}

	// Add this tile for later processing when idle.
	if ( isSneaking )
	{
		vMovesToProcessSneak.push(new HasXY2 <unsigned long int> (fullX,fullY));
	}
	else
	{
		vMovesToProcess.push(new HasXY2 <unsigned long int> (fullX,fullY));
	}
}

//Update knowledge with current instance.
bool Character::updateKnowledgeIdle()
{
	// Implement a basic delay to prevent idle flickering in.
	++idleCounter;
	if (idleCounter > 10) {
		idleCounter=10;
	}

	if ( vMovesToProcess.size() == 0 && vMovesToProcessSneak.size() == 0 ) {
		return false;
	}
	if ( knowledge == 0 ) {
		return false;
	}
	if ( tribe == 0 ) {
		return false;
	}

	//Update the LOS backlog during idle time.

	//For now this simply wipes LOS from last turn.
	knowledge->updateLOS();

	if (vMovesToProcess.size() > 0)
	{
		vMovesToProcess.shuffle();

		auto moveToProcess = vMovesToProcess(0);

		knowledge->addTile(moveToProcess->x,moveToProcess->y,isUnderground);


		Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(moveToProcess->x,moveToProcess->y,MAX_VIEW_RANGE,false,isUnderground);

		if ( vVisibleTiles!=0 )
		{
			for (int i=0; i<vVisibleTiles->size(); ++i)
			{
				//std::cout<<"ADDING\n";
				knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y,isUnderground);
				delete (*vVisibleTiles)(i);
			}
		}

		delete moveToProcess;
		vMovesToProcess.removeSlot(0);
		return true;
	}
	else if ( vMovesToProcessSneak.size() > 0)
	{
		vMovesToProcessSneak.shuffle();

		auto moveToProcess = vMovesToProcessSneak(0);

		knowledge->addTile(moveToProcess->x,moveToProcess->y,isUnderground);

		Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(moveToProcess->x,moveToProcess->y,MAX_VIEW_RANGE,true,isUnderground);

		if ( vVisibleTiles!=0 )
		{
			for (int i=0; i<vVisibleTiles->size(); ++i)
			{
				//std::cout<<"ADDING\n";
				knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y,isUnderground);
				delete (*vVisibleTiles)(i);
			}
		}

		delete moveToProcess;
		vMovesToProcessSneak.removeSlot(0);
		return true;
	}
	return false;
}


// ABSTRACT FUNCTIONS MONTHLY

// Research is based partially on intelligence. More people = more research.
// Base chance of success is 1% for level 10 intelligence, and it halves for each level below.
// Return true if a breakthrough occurs and the civ/settlement will determine outcome.
// There should probably be a system for diminishing returns as pop gets larger.
// Yes this could be ported out to the Settlement to manage, but for now I'll keep it per-Character to see how it goes

bool Character::abstractResearchMonth()
{
	// Remove any ideas which the settlement already has
	if ( settlement != 0 )
	{
		for (int i=0;i<vIdea.size(); ++i)
		{
			if ( settlement->hasIdea(vIdea(i)) )
			{
				vIdea.removeSlot(i);
			}
		}
	}
	
	
	// Base chance for level 1 (1% / 2^9)
	double baseChance = (0.01 / 512) * RESEARCH_SPEED_MODIFIER; // Approximately 0.0000195

	// Calculate the chance for the current intelligence level
	// The maximum value for rand is 4294967294, so we scale the base chance accordingly
	unsigned long long breakthroughChance = static_cast<unsigned long long>(baseChance * std::pow(2, baseSkill.intelligence - 1) * 4294967294);

	// Check for breakthrough
	bool hasIdea = globalRandom.rand32() < breakthroughChance;
	
	
	if (hasIdea)
	{
		// Give the Character an Idea which will hopefully become a tech
		if (globalRandom.flip())
		{
			Idea idea (this, Idea::IDEA_TYPE::IDEA_MINING);
			vIdea.push(idea);
			vOriginalIdea.push(idea);
		}
		else
		{
			Idea idea (this, Idea::IDEA_TYPE::IDEA_AGRICULTURE);
			vIdea.push(idea);
			vOriginalIdea.push(idea);
		}

		Console("IDEA");
	}
	
	return hasIdea;
}

void Character::abstractSocial(Character* character)
{
	if (character==0 || character==this)
	{
		return;
	}
	social.interact(character);
	
	return;
}

// INHERITED FUNCTIONS


Texture* Character::currentTexture ()
{
	if ( isAlive == false ) {
		return &TEX_NPC_DED;
	}
	return &TEX_NPC;
}

std::string Character::getName()
{
	return "Character";
}

// COUT OPERATOR

std::ostream& operator<<(std::ostream& stream, const Character& c)
{
	return stream<<c.getFullName();
}

// TABLE INTERFACE

/* TABLE INTERFACE */

std::string Character::getColumn(std::string _column)
{
	if ( _column=="firstname" )
	{
		return firstName;
	}
	else if ( _column=="lastname" )
	{
		return lastName;
	}
	if ( _column=="age" )
	{
		return DataTools::toString(age);
	}
	if ( _column=="sex" )
	{
		if (isMale) {
			return "Male";
		}
		return "Female";
	}

	if ( _column=="strength" )
	{
		return DataTools::toString(baseSkill.strength);
	}

	if ( _column=="kills" )
	{
		return DataTools::toString(vKills.size());
	}

	if ( _column=="favourite" )
	{
		if ( isFavourite ) {
			return "T";
		}
		return "";
	}
	if ( _column=="race" )
	{
		if (tribe==0)
		{
			return "?";
		}
		return tribe->getColumn("race");
	}
	if ( _column == "skillMetalsmithing" )
	{
		return DataTools::toString(skillMetalsmithing.level);
	}
	
	// if ( _column=="coordinates" )
	// {
	// return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	// }
	// if ( _column=="territory" )
	// {
	// return DataTools::toString(vTerritory.size());
	// }
	// // else if ( _column=="id" )
	// // {
	// // return DataTools::toString(id);
	// // }

	return "?";
}
std::string Character::getColumnType(std::string _column)
{
	if ( _column == "age" || _column == "territory" || _column == "food" || _column == "strength" || _column == "kills" || _column == "skillMetalsmithing" )
	{
		return "number";
	}
	return "string";
}

#endif