#ifndef PCAPWrapper_hh__
#define PCAPWrapper_hh__

#include <QObject>
#include <QSocketNotifier>

#include <pcap/pcap.h>

/**
  @class PCAPWrapper
  @brief Wrapper for dragging pcap kicking and screaming into C++-land
*/

class PCAPWrapper : public QObject
{
  Q_OBJECT

public:
  PCAPWrapper( QObject* parent = 0);
  virtual ~PCAPWrapper();

  void open();
  void close();

signals:

  void newPacket();
  void newPacket( const pcap_pkthdr* packetHeader,
                  const uchar* packetData );

private slots:

  /**
    Handles notifications by the socket notifier. The notifiers always signals
    whenever some packets are ready to be read from the PCAP handle.
  */

  void onSocketActivated();

private:

  /**
    Handles new PCAP packets. This function is signalled via the socket
    notifier whenever a new packet needs to be processed.

    @param userData     Contains pointer to PCAPWrapper instance
    @param packetHeader Header of PCAP packet
    @param packetData   Raw PCAP packet data
  */

  static void handlePacket( uchar* userData,
                            const pcap_pkthdr* packetHeader,
                            const uchar* packetData );

  void cleanup();

  char* _pcapErrorBuffer;
  pcap_t* _pcapHandle;

  QSocketNotifier* _socketNotifier;
};

#endif
