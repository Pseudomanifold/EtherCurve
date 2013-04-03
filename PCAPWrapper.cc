#include "PCAPWrapper.hh"

#include <iostream>

// ---------------------------------------------------------------------

PCAPWrapper::PCAPWrapper( QObject* parent )
  : QObject( parent ),
    _pcapErrorBuffer( new char[ PCAP_ERRBUF_SIZE ] ),
    _pcapHandle( 0 ),
    _socketNotifier( 0 )
{
}

// ---------------------------------------------------------------------

PCAPWrapper::~PCAPWrapper()
{
  this->cleanup();
}

// ---------------------------------------------------------------------

void PCAPWrapper::open()
{
  if( _pcapHandle )
  {
    // TODO: Complain about attempting to open a wrapper that is already open.
    return;
  }

  const char* device = "wlan0";

  _pcapHandle = pcap_open_live( device, // TODO: Make configurable
                                BUFSIZ,
                                0,      // TODO: Make configurable
                                -1,
                                _pcapErrorBuffer );

  if( _pcapHandle )
  {
    int fileDescriptor = pcap_get_selectable_fd( _pcapHandle );

    _socketNotifier    = new QSocketNotifier( fileDescriptor,
                                              QSocketNotifier::Read,
                                              this );

    QObject::connect( _socketNotifier,
                      SIGNAL( activated( int ) ),
                      this,
                      SLOT( onSocketActivated() ) );
  }
}

// ---------------------------------------------------------------------

void PCAPWrapper::close()
{
  if( _pcapHandle )
    pcap_close( _pcapHandle );

  _pcapHandle = 0;
}

// ---------------------------------------------------------------------

void PCAPWrapper::onSocketActivated()
{
  pcap_dispatch( _pcapHandle,
                 -1,
                 &PCAPWrapper::handlePacket,
                 reinterpret_cast<uchar*>( this ) );
}

// ---------------------------------------------------------------------

void PCAPWrapper::handlePacket( uchar* userData,
                                const pcap_pkthdr* packetHeader,
                                const uchar* packetData )
{
  PCAPWrapper* pcapWrapper = reinterpret_cast<PCAPWrapper*>( userData );

  emit( pcapWrapper->newPacket() );
  emit( pcapWrapper->newPacket( packetHeader,
                                packetData ) );
}

// ---------------------------------------------------------------------

void PCAPWrapper::cleanup()
{
  this->close();

  delete[]( _pcapErrorBuffer );
}

// ---------------------------------------------------------------------
