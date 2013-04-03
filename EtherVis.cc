#include <QApplication>

#include "MainWindow.hh"

int main( int argc, char* argv[] )
{
  QApplication application( argc, argv );

  // Initialization ----------------------------------------------------

  MainWindow mainWindow;
  mainWindow.show();

  return( application.exec() );
}
