#ifndef COURSE_FIELD_H
#define COURSE_FIELD_H

#include <QGridLayout>
#include <QPushButton>
#include <QGenericMatrix>
#include <QLabel>
#include <QPoint>
#include <QDebug>

#define ROW_INDEX rx()
#define COLUMN_INDEX ry()
constexpr int dimensions = 10;

class MainWindow;
//Перечисление типа попадания
enum class ModeShot
{
	hit,
	kill
};
//Структура ячейки
struct Cell
{
	int value;
	bool isShot;
	Cell(int value = 0, int isShot = false) : value(value), isShot(isShot) {}
};
//Класс поля
class Field : public QWidget
{
private:
	Q_OBJECT

protected:
	//Объявляем матрицу поля
	QGenericMatrix<dimensions, dimensions, Cell> matrix;
	//Объявляем стиль ячейки
	QString styleSheet =
			"QPushButton {"
			"background-color: lightblue;"
			"border: none; }"
			"QPushButton:hover {"
			"background-color: blue; }"
			"QPushButton:pressed {"
			"background-color: yellow; }";
	//Объявляем разметку ячеек
	QGridLayout* qGridLayout = new QGridLayout;
	//Объявляем указатель на главное окно
	MainWindow* mainWindowPtr = nullptr;
	int countHit = 0;

	explicit Field(MainWindow* mainWindowPtr);
	QPushButton* getQPushButtonPtr(const int& rowIndex, const int& columnIndex);
	QPoint returnPosIndex(QWidget* qWidgetPtr);
	void goDown(int& rowIndex, int& columnIndex, const bool& draw = false);
	bool goUp(int& rowIndex, int& columnIndex);
	virtual void setDefault();

public:
	void setEnabled(const bool& enabled);
};
#endif //COURSE_FIELD_H
