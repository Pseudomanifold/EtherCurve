#include <QGuiApplication>

#include "MainWindow.hh"

int main( int argc, char* argv[] )
{
  QGuiApplication application( argc, argv );

  // Initialization ----------------------------------------------------

  MainWindow mainWindow;
  mainWindow.show();

  return( application.exec() );
}
