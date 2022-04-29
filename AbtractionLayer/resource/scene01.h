#include "../AbtractionLayer/Snct_Scene.h"

#include "box.h"
#include "camera.h"

class Scene01 : public ISnctScene {

	//------------------------------------------------------------------------------
	/// Scene objects
	//------------------------------------------------------------------------------
	void SceneObjects() override {

		AddObject<OCamera>	("camera");
		AddObject<OBox>		("box");
	}
};