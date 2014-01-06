#include "MainWindow.h"

// ---------------------------------------------------------------------

MainWindow::MainWindow( const QString& deviceName )
{
  // TODO: Make configurable
  _width  = 512;
  _height = 512;

  _pcapWrapper = new PCAPWrapper( this );
  _pcapWrapper->open( deviceName );

  _packetRenderer = new PacketRenderer( this );
  _packetRenderer->setPacketWidth( _width / 64.f  );
  _packetRenderer->setPacketHeight( _height / 64.f );
  _packetRenderer->setMTU( _pcapWrapper->getMTU( deviceName ) );

  QObject::connect( _pcapWrapper,
                    SIGNAL( newPacket( const pcap_pkthdr*, const uchar* ) ),
                    this,
                    SLOT( onNewPacket( const pcap_pkthdr*, const uchar* ) ) );

  _graphicsScene = new QGraphicsScene( this );
  _graphicsView  = new QGraphicsView( _graphicsScene, this );

  _graphicsView->setBackgroundBrush( Qt::black);
  _graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  _graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  _graphicsView->setFrameShape( QFrame::NoFrame );
  _graphicsView->setAlignment( Qt::AlignLeft | Qt::AlignTop );

  this->setMinimumSize( _width, _height );
  this->setMaximumSize( _width, _height );
  this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

  this->setCentralWidget( _graphicsView );

  _currentIndex = 0;

  this->hilbertCurve( 0.0, 0.0,
                      1.0, 0.0,
                      0.0, 1.0,
                      6,
                      _coordinates );
}

// ---------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

// ---------------------------------------------------------------------

void MainWindow::onNewPacket( const pcap_pkthdr* packetHeader,
                              const uchar* packetData )
{
  qreal x;
  qreal y;

  this->mapToCurve( _currentIndex,
                    x, y );

  QGraphicsItem* renderedPacket = _packetRenderer->render( x, y,
                                                           packetHeader,
                                                           packetData );

  // Let's ignore packets we cannot render.
  if( !renderedPacket )
    return;

  // Remove existing items ---------------------------------------------

  QGraphicsItem* item = _graphicsScene->itemAt( renderedPacket->boundingRect().center(),
                                                QTransform() );

  if( item )
    _graphicsScene->removeItem( item );

  // Update scene ------------------------------------------------------

  _graphicsScene->addItem( renderedPacket );

  _currentIndex++;
  _currentIndex = _currentIndex % _coordinates.size();
}

// ---------------------------------------------------------------------

void MainWindow::hilbertCurve( double x0, double y0,
                               double xi, double xj,
                               double yi, double yj,
                               int n,
                               std::vector<std::pair<double, double> >& coordinates )
{
  if( n <= 0 )
  {
    double X = x0 + 0.5 * ( xi + yi );
    double Y = y0 + 0.5 * ( xj + yj );

    coordinates.push_back( std::make_pair( X,
                                           Y ) );
  }
  else
  {
    hilbertCurve( x0,                       y0,                        0.5 * yi,  0.5 * yj,  0.5 * xi,  0.5 * xj, n-1, coordinates );
    hilbertCurve( x0 + 0.5 * xi,            y0 + 0.5 * xj,             0.5 * xi,  0.5 * xj,  0.5 * yi,  0.5 * yj, n-1, coordinates );
    hilbertCurve( x0 + 0.5 * xi + 0.5 * yi, y0 + 0.5 * xj + 0.5 * yj,  0.5 * xi,  0.5 * xj,  0.5 * yi,  0.5 * yj, n-1, coordinates );
    hilbertCurve( x0 + 0.5 * xi + yi,       y0 + 0.5 * xj + yj,       -0.5 * yi, -0.5 * yj, -0.5 * xi, -0.5 * xj, n-1, coordinates );
  }
}

// ---------------------------------------------------------------------

void MainWindow::mapToCurve( std::size_t index,
                             qreal& x, qreal& y )
{
  std::pair<double, double> coordinate = _coordinates.at( index );

  x = static_cast<qreal>( coordinate.first ) * _width;
  y = static_cast<qreal>( coordinate.second ) * _height;
}

// ---------------------------------------------------------------------
