#include <QGuiApplication>

#include "MainWindow.h"

int main( int argc, char* argv[] )
{
  QGuiApplication application( argc, argv );

  // Initialization ----------------------------------------------------

  MainWindow mainWindow;
  mainWindow.show();

  return( application.exec() );
}
