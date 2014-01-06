#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "DeviceList.h"
#include "MainWindow.h"

int main( int argc, char* argv[] )
{
  QApplication application( argc, argv );
  QApplication::setApplicationName( "EtherCurve" );
  QApplication::setApplicationVersion( "1.0" );

  // Parse command-line options ----------------------------------------

  QCommandLineOption colourFileOption( QStringList() << "c" << "colour-file",
                                       "File to load packet colours from" );
  colourFileOption.setDefaultValue( "Qualitative1.csv" );

  QCommandLineOption logscalingOption( QStringList() << "l" << "log-scale",
                                       "Use logarithmic scaling for packet sizes" );

  QCommandLineParser parser;
  parser.setApplicationDescription( "Visualizes network traffic on a network interface by using\n"\
                                    "a space-filling Hilbert curve.");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption( logscalingOption );
  parser.addOption( colourFileOption );

  parser.process( application );

  // Create a device list ----------------------------------------------

  DeviceList deviceList;
  int result = deviceList.exec();

  QString selectedDeviceName;

  if( result == QDialog::Rejected )
    return( 0 );
  else
    selectedDeviceName = deviceList.getSelectedDeviceName();

  // Initialization ----------------------------------------------------

  QString colourFile = parser.value( colourFileOption );
  bool useLogScaling = parser.isSet( logscalingOption );

  MainWindow mainWindow( selectedDeviceName,
                         colourFile,
                         useLogScaling );
  mainWindow.show();

  return( application.exec() );
}
