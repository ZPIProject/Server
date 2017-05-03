#include "ServerManager.h"
#include "DatabaseManager.h"
int main()
{
	DatabaseManager man = DatabaseManager();
	man.AddExp("Valium", 1000);
	ServerManager sv;
	sv.run();
}