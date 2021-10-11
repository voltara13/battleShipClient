#include "Field.h"
#include "MainWindow.h"
#include "Client.h"
//Конструктор игрового поля
Field::Field(MainWindow* mainWindowPtr) : mainWindowPtr(mainWindowPtr)
{
	matrix.fill(Cell());
	for (int i = 0; i < dimensions; ++i)
	{
		//Добавление названий клеток сверху
		auto qLabelH = new QLabel(QChar(65 + i));
		qLabelH->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
		qLabelH->setContentsMargins(0, 0, 0, 3);
		qGridLayout->addWidget(qLabelH, 0, i + 1, 1, 1);
		//Добавление названий клеток слева
		auto qLabelV = new QLabel(QString::number(i + 1));
		qLabelV->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
		qLabelV->setContentsMargins(0, 0, 3, 0);
		qGridLayout->addWidget(qLabelV, i + 1, 0, 1, 1);
		//Добавление кнопок в ячейки
		for (int j = 0; j < dimensions; ++j)
		{
			auto qPushButton = new QPushButton();
			qPushButton->setEnabled(false);
			qPushButton->setStyleSheet(styleSheet);
			qPushButton->setFixedSize(50, 50);
			qGridLayout->addWidget(qPushButton, i + 1, j + 1);
		}
	}
	//Добавление отступов
	qGridLayout->addItem(new QSpacerItem(
		20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
		0, 0, 1, dimensions + 2);
	qGridLayout->addItem(new QSpacerItem(
		20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
		dimensions + 1, 0, 1, dimensions + 2);
	qGridLayout->addItem(new QSpacerItem(
		40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),
		1, 0, dimensions, 1);
	qGridLayout->addItem(new QSpacerItem(
		40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum),
		1, dimensions + 1, dimensions, 1);
	qGridLayout->setSpacing(0);
	//Добавление разметки игрового поля к разметке главного окна
	mainWindowPtr->getQHBoxLayoutGridBattleShip()->addLayout(qGridLayout);
}
//Функция получения указателя кнопки игрового поля
QPushButton* Field::getQPushButtonPtr(const int& rowIndex, const int& columnIndex)
{
	auto* qPushButtonItemPtr = qGridLayout->itemAtPosition(rowIndex + 1, columnIndex + 1);
	auto* qPushButtonPtr = dynamic_cast<QPushButton*>(qPushButtonItemPtr->widget());
	if (qPushButtonPtr)
		return qPushButtonPtr;
	return nullptr;
}
//Функция возврата позиции ячейки относительного игрового поля
QPoint Field::returnPosIndex(QWidget* qWidgetPtr)
{
	QPoint pointPos;
	int temp = 0;
	qGridLayout->getItemPosition(qGridLayout->indexOf(qWidgetPtr), &pointPos.rx(), &pointPos.ry(), &temp, &temp);
	pointPos.rx()--;
	pointPos.ry()--;
	return pointPos;
}
//Функция прохождения вниз-вправо для определения уничтожения корабля
void Field::goDown(int& rowIndex, int& columnIndex, const bool& draw)
{
	while (true)
	{
		auto* qPushButtonPtr = getQPushButtonPtr(rowIndex, columnIndex);
		if (draw)
			qPushButtonPtr->setStyleSheet("background-color: red;");
		if (
			rowIndex + 1 < dimensions &&
				matrix(rowIndex + 1, columnIndex).value > 0 &&
				matrix(rowIndex + 1, columnIndex).isShot)
			rowIndex++;
		else if (
			columnIndex + 1 < dimensions &&
				matrix(rowIndex, columnIndex + 1).value > 0 &&
				matrix(rowIndex, columnIndex + 1).isShot)
			columnIndex++;
		else
			break;
	}
}
//Функция прохождения вверх-влево для определения уничтожения корабля
bool Field::goUp(int& rowIndex, int& columnIndex)
{
	int count = 1;
	while (true)
	{
		if (
			rowIndex - 1 > -1 &&
				matrix(rowIndex - 1, columnIndex).value > 0 &&
				matrix(rowIndex - 1, columnIndex).isShot)
		{
			rowIndex--;
			count++;
		}
		else if (
			columnIndex - 1 > -1 &&
				matrix(rowIndex, columnIndex - 1).value > 0 &&
				matrix(rowIndex, columnIndex - 1).isShot)
		{
			columnIndex--;
			count++;
		}
		else
			break;
	}
	return count == matrix(rowIndex, columnIndex).value;
}
//Функция изменения состояния кнопок
void Field::setEnabled(const bool& enabled)
{
	for (int row = 0; row < dimensions; ++row)
		for (int column = 0; column < dimensions; ++column)
		{
			auto* qPushButtonPtr = getQPushButtonPtr(row, column);
			if (qPushButtonPtr)
				qPushButtonPtr->setEnabled(enabled);
		}
}
//Виртуальная функция сброса игрового поля ко значения по-умолчанию
void Field::setDefault()
{
	countHit = 0;
	matrix.fill(Cell());
	for (int row = 0; row < dimensions; ++row)
		for (int column = 0; column < dimensions; ++column)
		{
			auto* qPushButtonPtr = getQPushButtonPtr(row, column);
			if (qPushButtonPtr)
			{
				qPushButtonPtr->setEnabled(false);
				qPushButtonPtr->setStyleSheet(styleSheet);
			}
		}
}
