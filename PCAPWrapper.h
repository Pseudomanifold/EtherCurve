#ifndef PCAPWrapper_hh__
#define PCAPWrapper_hh__

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

#include <QObject>
#include <QSocketNotifier>
#include <QString>

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

  void open( const QString& deviceName );
  void close();

  /**
    @param device Device name
    @returns MTU of current device or -1 in case of errors
  */

  static int getMTU( const QString& deviceName );

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
