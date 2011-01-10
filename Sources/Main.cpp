

// Visomics includes
#include "voApplication.h"
#include "voMainWindow.h"

int main(int argc, char* argv[])
{
  voApplication app(argc, argv);

  Q_INIT_RESOURCE(Visomics);

  bool exitWhenDone = false;
  app.initialize(exitWhenDone);
  if (exitWhenDone)
    {
    return EXIT_SUCCESS;
    }
  
  voMainWindow mainwindow;
  mainwindow.show();
  
  return app.exec();
}