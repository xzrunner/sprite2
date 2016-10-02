#ifndef _SPRITE2_NETWORK_MESH_H_
#define _SPRITE2_NETWORK_MESH_H_

#include "Mesh.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Symbol;
class NetworkShape;

class NetworkMesh : public VIRTUAL_INHERITANCE Mesh
{
public:
	NetworkMesh();
	NetworkMesh(const NetworkMesh& mesh);
	NetworkMesh(const Symbol* base);
	NetworkMesh& operator = (const NetworkMesh& mesh);
	virtual ~NetworkMesh();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual NetworkMesh* Clone() const { return new NetworkMesh(*this); }

	void SetShape(NetworkShape* shape);
	const NetworkShape* GetShape() const { return m_shape; }
	
protected:
	void RefreshTriangles();

private:
	void Init(const NetworkMesh& mesh);

protected:
	NetworkShape* m_shape;

	VI_DUMMY_FUNC

}; // NetworkMesh

}

#endif // _SPRITE2_NETWORK_MESH_H_