#pragma once

#include <vector>
#include <list>
#include <mutex>

#include "NormalEnemy.h"
#include "Plane.h"

enum {enemy};

class Level
{
public:
	Level();
	~Level();

	virtual void Update(Plane* pPlane);
	virtual void Draw(CDC* pDC);
	virtual std::string Pack();
	virtual BOOL Unpack(std::string&);
	virtual void Clear();
	virtual void Sync();

public:
	std::vector<std::list<GameObject*>> m_ObjList;
	std::vector<std::list<GameObject*>> m_ObjList_buffer;
	std::mutex m_ObjList_buffer_lock;

	UINT m_nEnemyCnt;
	UINT m_nCurEnemyCnt;
	UINT m_nHard;
	UINT m_nScore;
};

