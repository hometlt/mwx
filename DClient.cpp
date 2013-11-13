#include "dclient.h"


// ----------------------------------------------------------------------
DServer::DServer(int nPort, QWidget* pwgt):QWidget(pwgt),nPort(nPort),m_nNextBlockSize(0)
{
/*!!!*/m_pTcpSocket=NULL;
    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);
    m_ptxtInput = new QLineEdit;
    caption=new QLabel();


    //Layout setup/////////////////////////
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(caption );
    pvbxLayout->addWidget(m_ptxt);
    pvbxLayout->addWidget(m_ptxtInput);
    setLayout(pvbxLayout);
}

// ----------------------------------------------------------------------
/*virtual*/ void DServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(deleteClientSocket()) );
    connect(pClientSocket, SIGNAL(readyRead()), this,          SLOT(slotReadClient()) );

    pClientSockets.append(pClientSocket);

    sendMessageToSocket(pClientSocket, "Server Response: Connected!");
    m_ptxt->append(QString::number( pClientSocket->localPort())+" is Connected");

     emit NewConnection();
}

// ----------------------------------------------------------------------
void DServer::deleteClientSocket()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    pClientSockets.remove(pClientSockets.indexOf(pClientSocket));
    pClientSocket->disconnect();
    pClientSocket->deleteLater();
    m_ptxt->append(QString::number( pClientSocket->localPort())+" is disconnected");
}

 // ----------------------------------------------------------------------
void DServer::slotReadServer()
{
    readSocket(m_pTcpSocket);
}

// ----------------------------------------------------------------------
void DServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    readSocket(pClientSocket);
}
// ----------------------------------------------------------------------

void DServer::readSocket(QTcpSocket* pSocket)
{
    QDataStream in(pSocket);

    in.setVersion(QDataStream::Qt_4_5);
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (pSocket->bytesAvailable() < sizeof(quint16)) {break; }
            in >> m_nNextBlockSize;
        }

        if (pSocket->bytesAvailable() < m_nNextBlockSize) { break;  }
        QTime   time;
        int msgtype;
        in >> time >> msgtype;
        QString strMessage ;
        switch(msgtype)
        {
        case msgAction:
            {
                DMWAction action;
                in>>action;
                strMessage =time.toString() + " <Action>"+" button=" + QString::number(action.actionButton)+" x="+QString::number(action.xIndex)+" y="+QString::number(action.yIndex);
                emit ActionReceived(action);
                break;
            }
        case msgString:
            {
                QString str;
                in>>str;
                strMessage =time.toString() + " <Message>" + str;
                emit MessageReceived(str);
                break;
            }
        case msgReplay:
            {
                DMWRePlay* replay=new DMWRePlay();
                in>>replay;
                strMessage =time.toString() + " <Replay>" + " width="+QString::number(replay->width())+"height="+QString::number(replay->height())+" mines="+QString::number(replay->mines())+" scheme="+replay->scheme();
                emit ReplayReceived(replay);
                break;
            }

        }
        m_ptxt->append(strMessage);
        m_nNextBlockSize = 0;
    //    sendMessageToSocket(pClientSocket, "Server Response: Received \"" + str + "\""  );
    }
}
// ------------------------------------------------------------------
void DServer::slotSendMessageToClients()
{
    foreach(QTcpSocket*pClient,pClientSockets)
    {
        sendMessageToSocket(pClient, m_ptxtInput->text());
    }
}
// ----------------------------------------------------------------------
void DServer::slotSendMessageToServer()
{
    sendMessageToSocket(m_pTcpSocket, m_ptxtInput->text());
    m_ptxtInput->setText("");
}
// ----------------------------------------------------------------------
void DServer::sendMessageToSocket(QTcpSocket* pSocket, const QString& str)
{
    QByteArray  arrBlock;QDataStream out(&arrBlock, QIODevice::WriteOnly);out.setVersion(QDataStream::Qt_4_5);
    out << quint16(0) << QTime::currentTime() <<msgString<< str;
    out.device()->seek(0);out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}

// ----------------------------------------------------------------------
void DServer::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxt->append(strError);
}


// ----------------------------------------------------------------------
void DServer::slotSendMWActionToServer(  DMWAction& action)
{
    action.sended=true;
   slotSendMWActionToSocket(m_pTcpSocket,action);
}

// ------------------------------------------------------------------
void DServer::slotSendMWActionToClients( const DMWAction& action)
{
    foreach(QTcpSocket*pClient,pClientSockets)
    {
        slotSendMWActionToSocket(pClient,action);
    }
}
// ------------------------------------------------------------------
void DServer::slotSendMWActionToSocket(QTcpSocket* pSocket,  const DMWAction& action)
{
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        out << quint16(0) << QTime::currentTime() <<msgAction<<action;

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        pSocket->write(arrBlock);

}
// ------------------------------------------------------------------
void DServer::slotSendMWReplayToSocket(QTcpSocket* pSocket, const DMWRePlay* replay)
{
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        out << quint16(0) << QTime::currentTime() <<msgReplay<<replay;

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        pSocket->write(arrBlock);
}
// ------------------------------------------------------------------
void DServer::slotSendMWReplayToClients( const DMWRePlay* replay)
{
    foreach(QTcpSocket*pClient,pClientSockets)
    {
        slotSendMWReplayToSocket(pClient,replay);
    }
}
// ------------------------------------------------------------------
void DServer::slotConnected()
{
    m_ptxt->append("Received the connected() signal");
}
// ------------------------------------------------------------------
void DServer::startServer()
{
    caption->setText("<H1>Server</H1>");
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort))
    {
        QMessageBox::critical(0, "Server Error","Unable to start the server:"+ m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }

    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection())  );


    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();


     m_ptxt->append(tr("The server is running\nIP: %1\nport: %2\n").arg(ipAddress).arg(m_ptcpServer->serverPort()));
     connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(slotSendMessageToClients()));

 }

// ------------------------------------------------------------------
void DServer::initClient()
{
    caption->setText("<H1>Client Settings</H1>");
    m_ptxt->append(tr("Enter the hostname:"));

    connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(startClient()));

}

// ------------------------------------------------------------------
void DServer::startClient()
{
    startClient(m_ptxtInput->text());
    m_ptxtInput->setText("");
}

// ------------------------------------------------------------------
void DServer::startClient( QString strHost)
{
    m_ptxt->append(tr("hostname:")+strHost);
    caption->setText("<H1>Client</H1>");
    disconnect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(startClient()));
    m_pTcpSocket = new QTcpSocket(this);

     m_pTcpSocket->connectToHost(strHost, nPort);
     connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
     connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadServer()));
     connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(slotError(QAbstractSocket::SocketError)) );

    connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(slotSendMessageToServer()));

}
