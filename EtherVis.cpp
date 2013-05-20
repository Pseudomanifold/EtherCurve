#include <QApplication>

#include "DeviceList.h"
#include "MainWindow.h"

int main( int argc, char* argv[] )
{
  QApplication application( argc, argv );

  // Create a device list ----------------------------------------------

  DeviceList deviceList;
  int result = deviceList.exec();

  QString selectedDeviceName;

  if( result == QDialog::Rejected )
    return( 0 );
  else
    selectedDeviceName = deviceList.getSelectedDeviceName();

  // Initialization ----------------------------------------------------

  MainWindow mainWindow( selectedDeviceName );
  mainWindow.show();

  return( application.exec() );
}
