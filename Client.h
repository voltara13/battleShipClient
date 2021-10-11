#ifndef COURSE_CLIENT_H
#define COURSE_CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>

class MainWindow;
class FieldPlayer;
class FieldEnemy;
//Класс клиента для соединения с сервером
class Client : public QWidget
{
private:
	Q_OBJECT
	//Объявляем указатели на элементы приложения
	MainWindow* mainWindowPtr = nullptr;
	FieldPlayer* fieldPlayerPtr = nullptr;
	FieldEnemy* fieldEnemyPtr = nullptr;
	QTcpSocket* qTCPSocketPtr = nullptr;
	//Объявляем
	quint16 sizeNextBlock = 0;
	bool isConnected = false;

public:
	explicit Client(const QString& lineIp, MainWindow* mainWindowPtr, FieldPlayer* fieldPlayerPtr, FieldEnemy* fieldEnemyPtr);
	void sendServer(const QString& sendString);
	bool getStatus();

private slots:
	void slotReadServer();
	void slotConnected();
	void slotDisconnected();
	void slotError(QAbstractSocket::SocketError err);
};

#endif //COURSE_CLIENT_H
