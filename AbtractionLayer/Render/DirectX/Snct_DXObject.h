#include "Snct_DXResource.h"
#include "Interface/ISnct_DXDevice.h"
#include "Interface/ISnct_DXContext.h"

#include "../../Snct_Utility.h"

class SnctDXObject
{
public:
	virtual bool Create(ISnctDXDevice* pDevice) = 0;
	virtual void Draw(ISnctDXContext* pCmdList) = 0;

protected:
	Transform m_Transform;
	Vertices m_Vertices;
	Indices	m_Indices;

private:

};