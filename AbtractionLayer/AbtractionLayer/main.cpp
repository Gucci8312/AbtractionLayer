#include "main.h"

//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main()
{
	SnctApplication app(g_screenWidth, g_screenHeight, "AbtractionLayer");

	//execution
	app.Run();

	return 0;
}