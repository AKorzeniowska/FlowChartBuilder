#pragma once

#ifdef DLLDIR_EX
#define DLLDIR  __declspec(dllexport)   // export DLL information

#else
#define DLLDIR  __declspec(dllimport)   // import DLL information

#endif 

//class DLLDIR LogicSegment
//{
//public:
    //LogicSegment(void);

extern "C" {
    DLLDIR void createGraphData(char arg[]); 
    DLLDIR int testDLL(int a); 
}
//};
