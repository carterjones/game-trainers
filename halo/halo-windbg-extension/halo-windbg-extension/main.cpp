#include <Windows.h>
#include <dbgeng.h>

HRESULT CALLBACK DebugExtensionInitialize(PULONG version, PULONG flags) {
  // Called when the extension is loaded
  // Extension can use it to initialize any global variables
  // Should return the extension version in *Version
  // Should not query for debug session information
  //   Debug session may not be active at this point

  return S_OK;
}

HRESULT CALLBACK help(PDEBUG_CLIENT client, PCSTR args) {
  
  return S_OK;
}
