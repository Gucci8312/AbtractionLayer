#include "Snct_DXResource.h"
#include "Interface/ISnct_DXDevice.h"
#include "Interface/ISnct_DXContext.h"

class SnctDXObject
{
public:
	virtual bool Create(ISnctDXDevice* pDevice) = 0;
	virtual void Draw(ISnctDXContext* pCmdList) = 0;
private:
};