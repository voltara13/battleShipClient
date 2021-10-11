#include "FieldPlayer.h"
#include "MainWindow.h"
#include "Client.h"
//Конструктор игрового поля игрока
FieldPlayer::FieldPlayer(MainWindow* mainWindowPtr) : Field(mainWindowPtr)
{
	for (int row = 0; row < dimensions; ++row)
		for (int column = 0; column < dimensions; ++column)
		{
			auto* qPushButtonPtr = getQPushButtonPtr(row, column);
			if (qPushButtonPtr)
				connect(qPushButtonPtr, &QPushButton::clicked, this, &FieldPlayer::clicked);
		}
}
//Функция проверки возможности установки корабля
bool FieldPlayer::checkShip()
{
	for (int row = first.ROW_INDEX; row <= second.ROW_INDEX; ++row)
		if (matrix(row, first.COLUMN_INDEX).value != 0)
			return false;
	for (int column = first.COLUMN_INDEX; column <= second.COLUMN_INDEX; ++column)
		if (matrix(first.ROW_INDEX, column).value != 0)
			return false;
	return true;
}
//Функция установки корабля в поле
void FieldPlayer::addShip(const int& length)
{
	for (int row = first.ROW_INDEX; row <= second.ROW_INDEX; ++row)
	{
		matrix(row, first.COLUMN_INDEX).value = length;
		getQPushButtonPtr(row, first.COLUMN_INDEX)->setStyleSheet("background-color: yellow;");
		for (int rowAround = row - 1; rowAround <= row + 1; ++rowAround)
			for (int columnAround = first.COLUMN_INDEX - 1; columnAround <= first.COLUMN_INDEX + 1; ++columnAround)
				if (rowAround > -1 && rowAround < 10 && columnAround > -1 && columnAround < 10 && !matrix(rowAround, columnAround).value)
					matrix(rowAround, columnAround).value = -1;
	}
	for (int column = first.COLUMN_INDEX; column <= second.COLUMN_INDEX; ++column)
	{
		matrix(first.ROW_INDEX, column).value = length;
		getQPushButtonPtr(first.ROW_INDEX, column)->setStyleSheet("background-color: yellow;");
		for (int columnAround = column - 1; columnAround <= column + 1; ++columnAround)
			for (int rowAround = first.ROW_INDEX - 1; rowAround <= first.ROW_INDEX + 1; ++rowAround)
				if (columnAround > -1 && columnAround < 10 && rowAround > -1 && rowAround < 10 && !matrix(rowAround, columnAround).value)
					matrix(rowAround, columnAround).value = -1;
	}
}
//Функция установки выстрела
void FieldPlayer::setShot(const QString& string)
{
	QStringList list = QString(string).split(" ", Qt::SkipEmptyParts);
	int rowIndex = list[0].toInt();
	int columnIndex = list[1].toInt();
	matrix(rowIndex, columnIndex).isShot = true;
	auto* qPushButtonPtr = getQPushButtonPtr(rowIndex, columnIndex);
	//Если есть значение в ячейке, то обрабатываем это
	if (matrix(rowIndex, columnIndex).value > 0)
	{
		countHit++;
		qPushButtonPtr->setStyleSheet("background-color: orange;");
		//Спускаемся вниз-вправо для определения уничтожения
		goDown(rowIndex, columnIndex);
		//Спускаемся вверх-влево для определения уничтожения
		if (goUp(rowIndex, columnIndex))
		{
			//Спускаемся вниз-вправо для закраски корабля
			goDown(rowIndex, columnIndex, true);
			mainWindowPtr->getClient()->sendServer("!KILL!");
		}
		else
			mainWindowPtr->getClient()->sendServer("!HIT!");
		if (countHit == 20)
		{
			QMessageBox::information(mainWindowPtr,
				"Поражение!",
				"К сожалению, Вы проиграли. Попробуйте ещё раз!");
			mainWindowPtr->restart();
		}
	}
	else
		qPushButtonPtr->setStyleSheet("background-color: blue;");
}
//Функция случайной расстановки кораблей
void FieldPlayer::setRandom()
{
	setDefault();
	int length;
	int angle;
	while (*std::max_element(shipList.begin(), shipList.end()))
	{
		first = {
			QRandomGenerator::global()->bounded(0, dimensions),
			QRandomGenerator::global()->bounded(0, dimensions)};
		second = {
			QRandomGenerator::global()->bounded(0, dimensions),
			QRandomGenerator::global()->bounded(0, dimensions)};
		QLineF line(first, second);
		length = line.length() + 1;
		angle = line.angle();
		if (first.ROW_INDEX > second.ROW_INDEX)
		{
			QPoint temp = first;
			first = second;
			second = temp;
		}
		if (first.COLUMN_INDEX > second.COLUMN_INDEX)
		{
			QPoint temp = first;
			first = second;
			second = temp;
		}
		if (length <= 4 && angle % 90 == 0 && shipList[length - 1] && checkShip())
		{
			qDebug() << (QString("FIRST:: %1;%2 SECOND:: %3;%4 LENGTH:: %5 ANGLE:: %6")
				.arg(first.ROW_INDEX)
				.arg(first.COLUMN_INDEX)
				.arg(second.ROW_INDEX)
				.arg(second.COLUMN_INDEX)
				.arg(length)
				.arg(angle));
			shipList[length - 1]--;
			addShip(length);
		}
	}
}
//Функция сброса игрового поля ко значения по-умолчанию
void FieldPlayer::setDefault()
{
	shipList = { 4, 3, 2, 1 };
	first = {-1, -1};
	second = {-1, -1};
	Field::setDefault();
}
//Слот нажатия на кнопку ячейки
void FieldPlayer::clicked()
{
	QPushButton* qPushButtonPtr = qobject_cast<QPushButton*>(sender());
	first = second;
	second = returnPosIndex(qPushButtonPtr);
	int length;
	int angle;
	if (first.ROW_INDEX != -1)
	{
		QLineF line(first, second);
		length = line.length() + 1;
		angle = line.angle();
		if (first.ROW_INDEX > second.ROW_INDEX)
		{
			QPoint temp = first;
			first = second;
			second = temp;
		}
		if (first.COLUMN_INDEX > second.COLUMN_INDEX)
		{
			QPoint temp = first;
			first = second;
			second = temp;
		}
		if (length <= 4 && angle % 90 == 0 && shipList[length - 1] && checkShip())
		{
			qDebug() << (QString("FIRST:: %1;%2 SECOND:: %3;%4 LENGTH:: %5 ANGLE:: %6")
				.arg(first.ROW_INDEX)
				.arg(first.COLUMN_INDEX)
				.arg(second.ROW_INDEX)
				.arg(second.COLUMN_INDEX)
				.arg(length)
				.arg(angle));
			shipList[length - 1]--;
			addShip(length);
		}
		first = { -1, -1};
		second = first;
		if (!*std::max_element(shipList.begin(), shipList.end()))
		{
			setEnabled(false);
			mainWindowPtr->setStart();
			for (int i = 0; i < dimensions; ++i)
			{
				QDebug debug = qDebug();
				for (int j = 0; j < dimensions; ++j)
					debug << matrix(i, j).value;
			}
		}
	}
}
