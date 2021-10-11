#include "MainWindow.h"
#include "Client.h"
#include "FieldPlayer.h"
#include "FieldEnemy.h"
//Конструктор главного окна
MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	//Общая разметка окна
	qVBoxLayout = new QVBoxLayout(this);
	//Разметка полей и кнопок
	fieldPlayer = new FieldPlayer(this);
	qVBoxLayoutClientButton->addItem(new QSpacerItem(
		20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
	qVBoxLayoutClientButton->addWidget(qPushButtonStart);
	qVBoxLayoutClientButton->addWidget(qPushButtonRestart);
	qVBoxLayoutClientButton->addWidget(qPushButtonRandom);
	qVBoxLayoutClientButton->addWidget(qPushButtonConnect);
	qVBoxLayoutClientButton->addWidget(qPushButtonDisconnect);
	qVBoxLayoutClientButton->addWidget(qPushButtonExit);
	qVBoxLayoutClientButton->addItem(new QSpacerItem(
		20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
	qHBoxLayoutGridBattleShip->addLayout(qVBoxLayoutClientButton);
	fieldEnemy = new FieldEnemy(this);
	qVBoxLayout->addLayout(qHBoxLayoutGridBattleShip);
	//Соединяем кнопки со слотами
	QObject::connect(qPushButtonStart, &QPushButton::clicked, this, &MainWindow::start);
	QObject::connect(qPushButtonRestart, &QPushButton::clicked, this, &MainWindow::restart);
	QObject::connect(qPushButtonRandom, &QPushButton::clicked, this, &MainWindow::random);
	QObject::connect(qPushButtonConnect, &QPushButton::clicked, this, &MainWindow::connect);
	QObject::connect(qPushButtonDisconnect, &QPushButton::clicked, this, &MainWindow::disconnect);
	QObject::connect(qPushButtonExit, &QPushButton::clicked, this, &MainWindow::exit);
	//Создаём горячие клавиши
	keyF1 = new QShortcut(this);
	keyS = new QShortcut(this);
	keyR = new QShortcut(this);
	keyN = new QShortcut(this);
	keyC = new QShortcut(this);
	keyD = new QShortcut(this);
	esc = new QShortcut(this);
	//Связываем горячие клавиши с клавишами
	keyF1->setKey(Qt::Key_F1);
	keyS->setKey(Qt::Key_S);
	keyR->setKey(Qt::Key_R);
	keyN->setKey(Qt::Key_N);
	keyC->setKey(Qt::Key_C);
	keyD->setKey(Qt::Key_D);
	esc->setKey(Qt::Key_Escape);
	//Связываем горячие клавиши со слотами
	QObject::connect(keyF1, &QShortcut::activated, this, &MainWindow::help);
	QObject::connect(keyS, &QShortcut::activated, this, &MainWindow::start);
	QObject::connect(keyR, &QShortcut::activated, this, &MainWindow::restart);
	QObject::connect(keyN, &QShortcut::activated, this, &MainWindow::random);
	QObject::connect(keyC, &QShortcut::activated, this, &MainWindow::connect);
	QObject::connect(keyD, &QShortcut::activated, this, &MainWindow::disconnect);
	QObject::connect(esc, &QShortcut::activated, this, &MainWindow::exit);
	//Сбрасываем состояния в режим по-умолчанию
	setDefault();
	setWindowTitle("Морской бой");
}
//Функция возврата разметки полей и кнопок
QHBoxLayout* MainWindow::getQHBoxLayoutGridBattleShip()
{
	return qHBoxLayoutGridBattleShip;
}
//Функция возврата указателя на клиент
Client* MainWindow::getClient()
{
	return client;
}
//Функция сброса состояния в режим по-умолчанию
void MainWindow::setDefault()
{
	qPushButtonStart->setEnabled(false);
	keyS->setEnabled(false);
	qPushButtonRestart->setEnabled(false);
	keyR->setEnabled(false);
	qPushButtonRandom->setEnabled(false);
	keyN->setEnabled(false);
	qPushButtonConnect->setEnabled(true);
	keyC->setEnabled(true);
	qPushButtonDisconnect->setEnabled(false);
	keyD->setEnabled(false);
	setDefaultField();
}
//Функция сброса состояния в пред-стартовый режим
void MainWindow::setStart()
{
	qPushButtonStart->setEnabled(true);
	keyS->setEnabled(true);
	fieldPlayer->setEnabled(false);
}
//Функция сброса полей в режимы по-умолчанию
void MainWindow::setDefaultField()
{
	fieldPlayer->setDefault();
	fieldEnemy->setDefault();
}
//Слот вызова окна помощи
void MainWindow::help()
{
	QMessageBox::information(this,
		"Помощь",
		"Игра морской бой\nРазмещаются:\n"
		"1 корабль — ряд из 4 клеток («четырёхпалубный»; линкор)\n"
		"2 корабля — ряд из 3 клеток («трёхпалубные»; крейсера)\n"
		"3 корабля — ряд из 2 клеток («двухпалубные»; эсминцы)\n"
		"4 корабля — 1 клетка («однопалубные»; торпедные катера)\n"
		"При размещении корабли не могут касаться друг друга сторонами и углами.\n"
		"Также можно воспользоваться кнопкой \"Случайно\", которая разместит Ваши корабли случайным образом.\n"
		"После размещения кораблей - для начала игры - неоходимо нажать кнопку \"Начать игру\".\n"
		"При попадании в корабль противника — на чужом поле ставится квадрат закрашивается оранжевым, при холостом выстреле - синим. Попавший стреляет ещё раз.\n"
		"Игра завершается после уничтожения всех кораблей противника.");
}
//Слот начала сражения
void MainWindow::start()
{
	qPushButtonStart->setEnabled(false);
	keyS->setEnabled(false);
	qPushButtonRandom->setEnabled(false);
	keyN->setEnabled(false);
	fieldEnemy->setEnabled(true);
}
//Слот рестарта сражения
void MainWindow::restart()
{
	setDefaultField();
	qPushButtonStart->setEnabled(false);
	keyS->setEnabled(false);
	qPushButtonRestart->setEnabled(true);
	keyR->setEnabled(true);
	qPushButtonRandom->setEnabled(true);
	keyN->setEnabled(true);
	qPushButtonConnect->setEnabled(false);
	keyC->setEnabled(false);
	qPushButtonDisconnect->setEnabled(true);
	keyD->setEnabled(true);
	fieldPlayer->setEnabled(true);
	client->sendServer("!RESTART!");
}
//Слот случайной расстановки кораблей
void MainWindow::random()
{
	setStart();
	fieldPlayer->setRandom();
}
//Слот соединения с сервером
void MainWindow::connect()
{
	bool ok;
	QString lineIp = QInputDialog::getText(this, tr("Подключение к серверу"),
		tr("IP Адресс:"), QLineEdit::Normal, "localhost", &ok);
	if (ok && !lineIp.isEmpty())
	{
		client = new Client(lineIp, this, fieldPlayer, fieldEnemy);
		if (client->getStatus())
			restart();
		else
			delete client;
	}
}
//Слот разрыва соединения с сервером
void MainWindow::disconnect()
{
	delete client;
	setDefault();
}
//Слот выхода из приложения
void MainWindow::exit()
{
	::exit(0);
}



