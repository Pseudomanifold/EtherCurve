#include <QApplication>

#include "MainWindow.h"

int main( int argc, char* argv[] )
{
  QApplication application( argc, argv );

  // Initialization ----------------------------------------------------

  MainWindow mainWindow;
  mainWindow.show();

  return( application.exec() );
}
