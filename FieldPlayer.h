#ifndef COURSE_FIELDPLAYER_H
#define COURSE_FIELDPLAYER_H

#include <QRandomGenerator>

#include "Field.h"

class MainWindow;
class Client;
//Класс игрового поля игрока
class FieldPlayer : public Field
{
private:
	//Объявляем переменные-точки
	QPoint first = { -1, -1};
	QPoint second = { -1, -1};
	//Объявляем массив количества кораблей
	QList<int> shipList = { 4, 3, 2, 1 };

	bool checkShip();
	void addShip(const int& length);

public:
	explicit FieldPlayer(MainWindow* mainWindowPtr);
	void setShot(const QString& string);
	void setRandom();
	void setDefault() override;

private slots:
	void clicked();
};


#endif //COURSE_FIELDPLAYER_H
