#include "MainWindow.hh"

#include <iostream>

// ---------------------------------------------------------------------

MainWindow::MainWindow( QWidget* parent,
                        Qt::WindowFlags flags )
  : QMainWindow( parent,
                 flags )
{
  _pcapWrapper = new PCAPWrapper( this );
  _pcapWrapper->open();

  QObject::connect( _pcapWrapper,
                    SIGNAL( newPacket( const pcap_pkthdr*, const uchar* ) ),
                    this,
                    SLOT( onNewPacket( const pcap_pkthdr*, const uchar* ) ) );
}

// ---------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

// ---------------------------------------------------------------------

void MainWindow::onNewPacket( const pcap_pkthdr* packetHeader,
                              const uchar* packetData )
{
  std::cout << "Got a new packet with "
            << packetHeader->len
            << " bytes"
            << std::endl;
}

// ---------------------------------------------------------------------
