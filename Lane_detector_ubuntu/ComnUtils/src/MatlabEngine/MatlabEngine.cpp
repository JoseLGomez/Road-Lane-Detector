#include "MatlabEngine.h"
#ifndef NOT_USEMATLAB
#pragma comment(lib, "libeng.lib")
#endif

CMatlabEngine::CMatlabEngine()
{
}

CMatlabEngine::~CMatlabEngine()
{
	//Finish();
}

void CMatlabEngine::Initialize()
{
#ifndef NOT_USEMATLAB
	if (!(ep = engOpen("\0"))) {
		ErrorQuit(VSys_Error,"\nCan't start MATLAB engine\n");
	}
#endif
}

void CMatlabEngine::ExecuteCommand(const string strCmd)
{
#ifndef NOT_USEMATLAB
	char* cmd=(char*)strCmd.c_str();
	engEvalString(ep, cmd);
#endif

}

void CMatlabEngine::RunMFile(const string path, const string fileName)
{
#ifndef NOT_USEMATLAB
	string str1="cd('"+path+"')";
	Println("\n> > %s",str1.c_str());
	ExecuteCommand(str1);
	Println("\n> > %s",fileName.c_str());
	ExecuteCommand(fileName);
#endif

}

void CMatlabEngine::Finish()
{
#ifndef NOT_USEMATLAB
	engClose(ep);
#endif
}