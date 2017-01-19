#include "SprTreePath.h"
#include "STPBufPool.h"

#include <iostream>

namespace s2
{

SprTreePath::SprTreePath()
	: m_val(0)
	, m_buf(NULL)
{
}

SprTreePath::SprTreePath(const SprTreePath& path)
	: m_val(path.m_val)
	, m_buf(NULL)
{
	if (path.m_buf) {
		m_buf = STPBufPool::Instance()->Fetch(path.m_buf->cap);
		m_buf->CopyFrom(path.m_buf);
	}
}

SprTreePath& SprTreePath::operator = (const SprTreePath& path)
{
	STPBufPool* pool = STPBufPool::Instance();

	m_val = path.m_val;
	if (m_buf) 
	{
		if (path.m_buf)
		{
			if (m_buf->cap != path.m_buf->cap) {
				pool->Return(m_buf);
				m_buf = pool->Fetch(path.m_buf->cap);
			}
			m_buf->CopyFrom(path.m_buf);
		}
		else
		{
			STPBufPool::Instance()->Return(m_buf);
			m_buf = NULL;
		}
	}
	else
	{
		if (path.m_buf) {
			m_buf = pool->Fetch(path.m_buf->cap);
			m_buf->CopyFrom(path.m_buf);
		}
	}

	return *this;
}

SprTreePath::~SprTreePath()
{
	if (m_buf) {
		STPBufPool::Instance()->Return(m_buf);
	}
}

void SprTreePath::Push(int id) 
{
	if (!m_buf)
	{
		m_buf = STPBufPool::Instance()->Fetch(2);
	}
	else if (m_buf->num >= m_buf->cap)
	{
		STPBuffer* new_buf = STPBufPool::Instance()->Fetch(m_buf->cap * 2);
		new_buf->CopyFrom(m_buf);
		STPBufPool::Instance()->Return(m_buf);
		m_buf = new_buf;
	}

	m_buf->ids[m_buf->num++] = id;
	m_val += id * m_buf->num;
}

std::ostream & operator << (std::ostream& os, const SprTreePath& path)
{
	os << "[path: ";
	for (int i = 0, n = path.m_buf->num; i < n; ++i) {
		os << path.m_buf->ids[i] << ", ";
	}
	os << "]";
	return os;
}

}