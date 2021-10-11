#ifndef COURSE_MAINWINDOW_H
#define COURSE_MAINWINDOW_H

#include <QLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QShortcut>

class Client;
class FieldPlayer;
class FieldEnemy;
//Класс главного окна
class MainWindow : public QWidget
{
private:
	Q_OBJECT
	//Объявляем разметку
	QVBoxLayout* qVBoxLayout = nullptr;
	QVBoxLayout* qVBoxLayoutClientButton = new QVBoxLayout;
	QHBoxLayout* qHBoxLayoutGridBattleShip = new QHBoxLayout;
	//Объявляем кнопки
	QPushButton* qPushButtonStart = new QPushButton("Начать играть");
	QPushButton* qPushButtonRestart = new QPushButton("Рестарт");
	QPushButton* qPushButtonRandom = new QPushButton("Случайно");
	QPushButton* qPushButtonConnect = new QPushButton("Подключиться");
	QPushButton* qPushButtonDisconnect = new QPushButton("Отключиться");
	QPushButton* qPushButtonExit = new QPushButton("Выход");
	//Объявляем горячие клавиши
	QShortcut* keyF1 = nullptr;
	QShortcut* keyS = nullptr;
	QShortcut* keyR = nullptr;
	QShortcut* keyN = nullptr;
	QShortcut* keyC = nullptr;
	QShortcut* keyD = nullptr;
	QShortcut* esc = nullptr;
	//Объявляем указатели на элементы приложения
	Client* client = nullptr;
	FieldPlayer* fieldPlayer = nullptr;
	FieldEnemy* fieldEnemy = nullptr;

public:
	explicit MainWindow(QWidget* parent = nullptr);
	QHBoxLayout* getQHBoxLayoutGridBattleShip();
	Client* getClient();
	void setStart();
	void setDefault();
	void setDefaultField();

private slots:
	void help();
	void start();
	void connect();
	void disconnect();
	void exit();

public slots:
	void restart();
	void random();
};

#endif //COURSE_MAINWINDOW_H
