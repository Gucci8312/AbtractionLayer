#include "../AbtractionLayer/Snct_Object.h"

class OBox : public ISnctObject 
{
public:
	void Initialize()	override 
	{
		m_name = "box";

		m_vertices = {

			/*Front*/
			{Vertex(FLOAT4(-1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 0.0f,-1.0f, 0.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 0.0f,-1.0f, 0.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 0.0f,-1.0f, 0.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f), FLOAT2(1.0f, 1.0f))},

			{Vertex(FLOAT4(-1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 0.0f,-1.0f, 0.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 0.0f,-1.0f, 0.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 0.0f,-1.0f, 0.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f), FLOAT2(1.0f, 0.0f))},

			/*Left*/
			{Vertex(FLOAT4(-1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(-1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(-1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(-1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f), FLOAT2(1.0f, 1.0f))},

			{Vertex(FLOAT4(-1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(-1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(-1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(-1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(-1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f), FLOAT2(1.0f, 0.0f))},

			/*Back*/
			{Vertex(FLOAT4(1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 0.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 0.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 0.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f))},

			{Vertex(FLOAT4(1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 0.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 0.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(-1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 0.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f), FLOAT2(1.0f, 0.0f))},

			/*Right*/
			{Vertex(FLOAT4(1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f), FLOAT2(1.0f, 1.0f))},

			{Vertex(FLOAT4(1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f), FLOAT2(1.0f, 0.0f))},

			/*Bottom*/
			{Vertex(FLOAT4(-1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(0.0f,-1.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(-1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(0.0f,-1.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(0.0f,-1.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f))},

			{Vertex(FLOAT4(-1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(0.0f,-1.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(1.0f,-1.0f,-1.0f, 1.0f), FLOAT4(0.0f,-1.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f,-1.0f, 1.0f, 1.0f), FLOAT4(0.0f,-1.0f, 0.0f, 0.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 0.0f))},

			/*Top*/
			{Vertex(FLOAT4(-1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f))},

			{Vertex(FLOAT4(-1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 0.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f))},
			{Vertex(FLOAT4(1.0f, 1.0f,-1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 0.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 0.0f))},
		};

		m_indices = {
			 0,  1,  2,  3,  4,  5,
			 6,  7,  8,  9, 10, 11,
			12, 13, 14, 15, 16, 17,
			18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35,
		};

		m_pRender->CreateObject(m_name, &m_vertices, &m_indices);
	}
	void Update()		override 
	{
	}
	void Draw()			override 
	{
		m_pRender->SetObject(&m_transform);
		m_pRender->Draw(m_name, SNCT_DRAW_FLAG::SNCT_DRAW_FLAG_IMMADIATE);
	}

	void Finalize()		override {};

protected:

private:
	

};