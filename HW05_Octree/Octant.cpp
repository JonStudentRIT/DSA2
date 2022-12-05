#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{

	/*
	* This constructor is meant to be used ONLY on the root node, there is already a working constructor
	* that will take a size and a center to create a new space
	*/
	// initial values set
	Init();
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;
	m_pRoot = this;
	m_lChild.clear();
	
	// list of all the maximum
	std::vector<vector3> localList;
	// assign the list of maximum
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i)
	{
		RigidBody* localRig = m_pEntityMngr->GetEntity(i)->GetRigidBody();
		vector3 max = localRig->GetMaxGlobal();
		localList.push_back(max);
	}

	//rigidbodies of the list of maximum
	RigidBody* localRig = new RigidBody(localList);

	// find the max value for the octant size
	m_fSize = localRig->GetHalfWidth().x * 2;

	if (m_fSize < localRig->GetHalfWidth().y * 2)
	{
		m_fSize = localRig->GetHalfWidth().y * 2;
	}
	if (m_fSize < localRig->GetHalfWidth().z * 2)
	{
		m_fSize = localRig->GetHalfWidth().z * 2;
	}

	// set the min max and center data
	m_v3Center = localRig->GetCenterLocal();
	m_v3Min = m_v3Center - vector3(m_fSize);
	m_v3Max = m_v3Center + vector3(m_fSize);

	m_uOctantCount++; //When we add an octant we increment the count
	ConstructTree(m_uMaxLevel); //Construct the children
	
	//remove the allocated Rigidbody
	delete localRig;
	localRig = nullptr;
}

bool Octant::IsColliding(uint a_uRBIndex)
{
	// the rigidbody to check
	RigidBody* localRig = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();

	// the Axis Alligned Bounding Box to check
	if (m_v3Min.x > localRig->GetMaxGlobal().x ||
		m_v3Min.y > localRig->GetMaxGlobal().y ||
		m_v3Min.z > localRig->GetMaxGlobal().z ||
		m_v3Max.x < localRig->GetMinGlobal().x ||
		m_v3Max.y < localRig->GetMinGlobal().y ||
		m_v3Max.z < localRig->GetMinGlobal().z)
	{
		return false;
	}
	else
	{
		return true;
	}
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// display each octant by ID
	for (int i = 0; i < m_uChildren; i++)
	{
		// call the recursive display
		m_pChild[i]->Display(a_v3Color);
	}
}
void Octant::Display(vector3 a_v3Color)
{
	// display each octant recursivly from the child to the root
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	// render the wire cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * 
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::Subdivide(void)
{
	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;
	
	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;
	
	// set the child count to 8
	m_uChildren = 8;

	// create a new octant for each child array
	m_pChild[0] = new Octant(m_v3Center + vector3(m_fSize / 4, m_fSize / 4, m_fSize / 4), m_fSize / 2);
	m_pChild[1] = new Octant(m_v3Center + vector3(m_fSize / 4, m_fSize / 4, -1 * (m_fSize / 4)), m_fSize / 2);
	m_pChild[2] = new Octant(m_v3Center + vector3(m_fSize / 4, -1 * (m_fSize / 4), m_fSize / 4), m_fSize / 2);
	m_pChild[3] = new Octant(m_v3Center + vector3(m_fSize / 4, -1 * (m_fSize / 4), -1 * (m_fSize / 4)), m_fSize / 2);
	m_pChild[4] = new Octant(m_v3Center + vector3(-1 * (m_fSize / 4), m_fSize / 4, m_fSize / 4), m_fSize / 2);
	m_pChild[5] = new Octant(m_v3Center + vector3(-1 * (m_fSize / 4), m_fSize / 4, -1 * (m_fSize / 4)), m_fSize / 2);
	m_pChild[6] = new Octant(m_v3Center + vector3(-1 * (m_fSize / 4), -1 * (m_fSize / 4), m_fSize / 4), m_fSize / 2);
	m_pChild[7] = new Octant(m_v3Center + vector3(-1 * (m_fSize / 4), -1 * (m_fSize / 4), -1 * (m_fSize / 4)), m_fSize / 2);
	
	// divide up the children 
	for (int i = 0; i < m_uChildren; i++)
	{
		// the level the child is on
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		// the parent of the next level child is the child that is being divided up
		m_pChild[i]->m_pParent = this;
		// the root is passed to the next generation
		m_pChild[i]->m_pRoot = m_pRoot;
		// check if children need to be subdivided
		if (m_pChild[i]->ContainsAtLeast(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
	}
}
bool Octant::ContainsAtLeast(uint a_nEntities)
{
	// How many are in this octant
	int count = 0;
	// search through all of the enities
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		// check if its colliding
		if (IsColliding(i))
		{
			// increase the number of entities in this octant
			count++;
		}
		// if we have reached our max so return to subdivide the child further
		if (count > a_nEntities)
		{
			return true;
		}
	}
	// this octant is less than or equal to the ideal count
	return false;
}
void Octant::AssignIDtoEntity(void)
{
	// if this child has children
	if (m_uChildren != 0)
	{
		// search each of the children for more children
		for (int i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
	}
	// if there are no children 
	else
	{
		// look through all of the entities
		for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			// check each collision in case and entity collides with multiple octants
			if (IsColliding(i))
			{
				// set the entity ID to the octant its stored in
				m_pEntityMngr->AddDimension(i, m_uID);
				// add this entity to the entity list
				m_EntityList.push_back(i);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Octant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void Octant::Swap(Octant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
void Octant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
void Octant::ConstructTree(uint a_nMaxLevel)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel; //Make sure you mark which is the maximum level you are willing to reach
	m_uOctantCount = 1;// We will always start with one octant

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this octant is empty
	KillBranches();
	m_lChild.clear();

	//If we have more entities than those that we ideally want in this octant we subdivide it
	if (ContainsAtLeast(m_uIdealEntityCount))
	{
		Subdivide();
	}
	AssignIDtoEntity();//Add octant ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	//Will create the octant object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}
Octant::Octant(Octant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
Octant& Octant::operator=(Octant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}
Octant::~Octant() { Release(); };
//Accessors
float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
bool Octant::IsLeaf(void) { return m_uChildren == 0; }
Octant* Octant::GetParent(void) { return m_pParent; }
//--- other methods
Octant * Octant::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
void Octant::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}
void Octant::DisplayLeaves(vector3 a_v3Color)
{
	/*
	* Recursive method
	* it will traverse the tree until it find leaves and once found will draw them
	*/
	uint nLeafs = m_lChild.size(); //get how many children this will have (either 0 or 8)
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		m_lChild[nChild]->DisplayLeaves(a_v3Color);
	}
	//Draw the cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
}
void Octant::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}
	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
