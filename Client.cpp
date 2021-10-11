#include "Client.h"
#include "MainWindow.h"
#include "FieldPlayer.h"
#include "FieldEnemy.h"
//Конструктор клиента
Client::Client(const QString& lineIp, MainWindow* mainWindowPtr, FieldPlayer* fieldPlayerPtr, FieldEnemy* fieldEnemyPtr) :
mainWindowPtr(mainWindowPtr), fieldPlayerPtr(fieldPlayerPtr), fieldEnemyPtr(fieldEnemyPtr)
{
	//Создаём новый сокет
	qTCPSocketPtr = new QTcpSocket(this);
	//Подключаемся к серверу
	qTCPSocketPtr->connectToHost(lineIp, 2323);
	//Подключаем сигналы сокета к слотам
	connect(qTCPSocketPtr, &QAbstractSocket::connected, this, &Client::slotConnected);
	connect(qTCPSocketPtr, &QAbstractSocket::disconnected, this,	&Client::slotDisconnected);
	connect(qTCPSocketPtr, &QAbstractSocket::readyRead, this, &Client::slotReadServer);
	connect(qTCPSocketPtr, &QAbstractSocket::errorOccurred, this, &Client::slotError);
	if (qTCPSocketPtr->waitForConnected(10000))
		isConnected = true;
	else
	{
		isConnected = false;
		delete qTCPSocketPtr;
	}
}
//Функция получения статуса клиента
bool Client::getStatus()
{
	return isConnected;
}
//Функция отправки строки на сервер
void Client::sendServer(const QString& sendString)
{
	//Создаём вспомогательный поток
	QByteArray arrBlock = nullptr;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_0);
	//Добавляем вспомогательные элементы в выходной поток
	out << quint16(0) << sendString;
	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));
	//Записываем данные в сокет и отправляем
	qTCPSocketPtr->write(arrBlock);
}
//Слот чтения с сервера
void Client::slotReadServer()
{
	//Создаём вспомогательный поток, ему задаём версию фреймворка
	QDataStream in(qTCPSocketPtr);
	in.setVersion(QDataStream::Qt_6_0);
	sizeNextBlock = 0;
	//Создаём строку ответа
	QString response;
	//Читаем то, что нам прислал сервер
	while (true)
	{
		//Если размер следующего блока неизвестен, то
		if (!sizeNextBlock)
		{
			//Если блок меньше чем два байта, то выходим из цикла
			if (qTCPSocketPtr->bytesAvailable() < sizeof(quint16)) break;
			in >> sizeNextBlock;
		}
		//Если размер следующего блока больше, чем количество байтов ожидающих чтения, то выходим из цикла
		if (qTCPSocketPtr->bytesAvailable() < sizeNextBlock) break;
		//Создаём вспомогательный буфер, в который мы передаём содержимое потока
		QString buffer;
		in >> buffer;
		//Содержимое буфера передаём в строку ответа
		response.append(buffer);
		//Размер следующего блока неизвестен, пусть он будет равен нулю
		sizeNextBlock = 0;
	}
	qDebug() << response;
	if (response.contains("!HIT!"))
		fieldEnemyPtr->setShot(ModeShot::hit);
	else if (response.contains("!KILL!"))
		fieldEnemyPtr->setShot(ModeShot::kill);
	else
		fieldPlayerPtr->setShot(response);
}
//Слот подключения клиента к серверу
void Client::slotConnected()
{
	QMessageBox::information(mainWindowPtr,
		"Информация",
		"Сервер успешно подключен");
}
//Слот отключения клиента от сервера
void Client::slotDisconnected()
{
	QMessageBox::information(mainWindowPtr,
		"Информация",
		"Клиент успешно отключился от сервера");
}
//Слот ошибки со стороны клиента
void Client::slotError(QAbstractSocket::SocketError err)
{
	QString strError =
		"Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
		              "Сервер не найден." :
		              err == QAbstractSocket::RemoteHostClosedError ?
		              "Сервер закрыл соедининение." :
		              err == QAbstractSocket::ConnectionRefusedError ?
		              "В соединении было отказано." :
		              QString(qTCPSocketPtr->errorString()));
	QMessageBox::critical(mainWindowPtr, "Ошибка", strError);
	mainWindowPtr->setDefault();
}
