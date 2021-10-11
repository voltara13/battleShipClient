#ifndef COURSE_FIELDENEMY_H
#define COURSE_FIELDENEMY_H

#include "Field.h"
#include <QDebug>

class MainWindow;
//Класс игрового поля врага
class FieldEnemy : public Field
{
private:
	//Объявляем точку нажатия
	QPoint pos = {-1, -1};

public:
	explicit FieldEnemy(MainWindow* mainWindowPtr);
	void setShot(const ModeShot& modeShot);
	void setDefault() override;

private slots:
	void clicked();
};


#endif //COURSE_FIELDENEMY_H
