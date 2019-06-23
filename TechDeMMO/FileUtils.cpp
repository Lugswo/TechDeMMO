#include "FileUtils.h"

#include <fstream>
#include <ShlObj.h>

#include "TraceLog.h"

bool CheckForFile(const std::string & filename)
{
  std::ifstream f(filename);
  return f.good();
}

const std::string GetMyDocumentsPath()
{
  char my_documents[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

  if (result != S_OK)
  {
    TraceLog::Log(TRACE_LEVEL::WARN, "My Documents path not found!");
    return "";
  }

  return my_documents;
}
