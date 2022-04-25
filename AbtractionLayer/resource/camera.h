#include "../AbtractionLayer/Snct_Object.h"

class OCamera : public ISnctObject
{
public:
	void Initialize()	override 
	{
		m_name = "camera";
		camera.position = FLOAT3(0.0f, 5.0f, -5.0f);
	}
	void Update()		override { m_pRender->SetCamera(&camera); }
	void Draw()			override {}
	void Finalize()		override {};

protected:

private:
	Camera camera;

};