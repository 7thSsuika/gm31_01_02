#include "gameObject.h"

void GameObject::SetDestroy()
{
	m_Destroy = true;
}

bool GameObject::Destroy()
{
	if (m_Destroy)
	{
		Uninit();
		delete this;
		return true;
	}
	else
	{
		return false;
	}
}
