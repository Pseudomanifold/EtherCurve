#include "PCAPWrapper.h"

#include <cstring>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <unistd.h>

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

void PCAPWrapper::open( const QString& deviceName )
{
  if( _pcapHandle )
  {
    // TODO: Complain about attempting to open a wrapper that is already open.
    return;
  }

  QByteArray byteArray = deviceName.toLocal8Bit();
  const char* device   = byteArray.data();

  _pcapHandle = pcap_open_live( device,
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

int PCAPWrapper::getMTU( const QString& deviceName )
{
  if( deviceName.isEmpty() )
    return( -1 );

  QByteArray byteArray = deviceName.toLocal8Bit();
  const char* device   = byteArray.data();

  ifreq interfaceRequest;

  memset( &interfaceRequest, 0, sizeof( ifreq ) );

  // Prepare for lookup
  strncpy( interfaceRequest.ifr_name,
           device,
           sizeof( interfaceRequest.ifr_name) );

  int fd = socket( AF_INET, SOCK_DGRAM, 0 );

  if( ioctl( fd, SIOCGIFMTU, &interfaceRequest ) == -1 )
    return( -1 );

  ::close( fd );

  return( interfaceRequest.ifr_mtu );
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
