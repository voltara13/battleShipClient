#include "FieldEnemy.h"
#include "MainWindow.h"
#include "Client.h"
//Конструктор игрового поля врага
FieldEnemy::FieldEnemy(MainWindow* mainWindowPtr) : Field(mainWindowPtr)
{
	for (int i = 0; i < dimensions; ++i)
		for (int j = 0; j < dimensions; ++j)
		{
			auto* qPushButtonPtr = getQPushButtonPtr(i, j);
			if (qPushButtonPtr)
				connect(qPushButtonPtr, &QPushButton::clicked, this, &FieldEnemy::clicked);
		}
}
//Функция установки выстрела
void FieldEnemy::setShot(const ModeShot& modeShot)
{
	matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).value = 1;
	auto* qPushButtonPtr = getQPushButtonPtr(pos.ROW_INDEX, pos.COLUMN_INDEX);
	qPushButtonPtr->setEnabled(false);
	countHit++;
	switch (modeShot)
	{
	case ModeShot::hit:
		qPushButtonPtr->setStyleSheet("background-color: orange");
		break;
	case ModeShot::kill:
		int rowIndexTemp = pos.ROW_INDEX;
		int columnIndexTemp = pos.COLUMN_INDEX;
		goUp(rowIndexTemp, columnIndexTemp);
		goDown(rowIndexTemp, columnIndexTemp, true);
		break;
	}
	if (countHit == 20)
	{
		QMessageBox::information(mainWindowPtr,
			"Победа!",
			"Поздравляем, Вы победили!");
		mainWindowPtr->restart();
	}
}
//Функция сброса игрового поля ко значения по-умолчанию
void FieldEnemy::setDefault()
{
	Field::setDefault();
}
//Слот нажатия на кнопку ячейки
void FieldEnemy::clicked()
{
	auto* qPushButtonPtr = qobject_cast<QPushButton*>(sender());
	qPushButtonPtr->setStyleSheet("background-color: blue;");
	qPushButtonPtr->setEnabled(false);
	pos = returnPosIndex(qPushButtonPtr);
	matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).value = -1;
	matrix(pos.ROW_INDEX, pos.COLUMN_INDEX).isShot = true;
	mainWindowPtr->getClient()->sendServer(QString("%1 %2").arg(pos.ROW_INDEX).arg(pos.COLUMN_INDEX));
}
