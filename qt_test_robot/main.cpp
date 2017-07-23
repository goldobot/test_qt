#if defined(_WIN32) || defined(_WIN64) 
#include <windows.h>
#endif

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtCore/QThread>

#include "QtTestComponentManager.h"

int SecuredMain(int &argc, char **argv)
{
  try
  {
#if defined(_WIN32) || defined(_WIN64) 
    WSADATA wsaData;
    WSAStartup(0x202, &wsaData);
#endif

    // create the Qt application (which handle the UI main loop)
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("GoldoSoft");
    QCoreApplication::setApplicationName("TestQt");

    // build the main frame
    QT_TEST::QtTestComponentManager mainObject;

    // enter the UI main loop
    return app.exec();
  }

  catch(...)
  {
    // unmanaged exception caught, return generic error
    return -1;
  }
}

#if defined(_WIN32) || defined(_WIN64) 

PCHAR *CommandLineToArgvA(PCHAR CmdLine, int *_argc)
{
  PCHAR* argv;
  PCHAR  _argv;
  ULONG   len;
  ULONG   argc;
  CHAR   a;
  ULONG   i, j;

  BOOLEAN  in_QM;
  BOOLEAN  in_TEXT;
  BOOLEAN  in_SPACE;

  len = strlen(CmdLine);
  i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

  argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
  i + (len+2)*sizeof(CHAR));

  _argv = (PCHAR)(((PUCHAR)argv)+i);

  argc = 0;
  argv[argc] = _argv;
  in_QM = FALSE;
  in_TEXT = FALSE;
  in_SPACE = TRUE;
  i = 0;
  j = 0;

  while(a = CmdLine[i])
  {
    if(in_QM)
    {
      if(a == '\"')
        in_QM = FALSE;

      else
      {
        _argv[j] = a;
        j++;
      }
    }

    else
    {
      switch(a)
      {
      case '\"':
        in_QM = TRUE;
        in_TEXT = TRUE;

        if(in_SPACE)
        {
          argv[argc] = _argv+j;
          argc++;
        }

        in_SPACE = FALSE;
        break;

      case ' ':
      case '\t':
      case '\n':
      case '\r':
        if(in_TEXT)
        {
          _argv[j] = '\0';
          j++;
        }

        in_TEXT = FALSE;
        in_SPACE = TRUE;
        break;

      default:
        in_TEXT = TRUE;

        if(in_SPACE)
        {
          argv[argc] = _argv+j;
          argc++;
        }

        _argv[j] = a;
        j++;
        in_SPACE = FALSE;
        break;
      }
    }

    i++;
  }

  _argv[j] = '\0';
  argv[argc] = NULL;

  (*_argc) = argc;
  return argv;
}


#pragma warning( disable : 4100 )

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  char **argv = NULL;
  int argc = 0;

  argv = CommandLineToArgvA(GetCommandLine(), &argc);
  if (argv == NULL)
   return -2;

  int retVal = SecuredMain(argc, argv);

  LocalFree(argv);
  return retVal;
}

#pragma warning( default : 4100 )

#endif /* defined(_WIN32) || defined(_WIN64) */

int main(int argc, char *argv[])
{
  return SecuredMain(argc, argv);
}
