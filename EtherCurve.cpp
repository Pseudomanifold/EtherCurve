/*
  The MIT License (MIT)

  Copyright (c) 2014 Bastian Rieck

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

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
