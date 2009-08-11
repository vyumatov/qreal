/** @file uml_nodeelement.h
 * 	@brief Класс, представляющий объект на диаграмме
 * */

#pragma once

#include "uml_element.h"
#include "uml_edgeelement.h"
#include "sdfrenderer.h"
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <QtGui/QWidget>

/** @brief Размер порта объекта */
const int kvadratik = 5;

namespace UML {
	/** @class NodeElement
	* 	@brief Класс, представляющий объект на диаграмме
	 * */
	class StatLine;

	class ElementTitle : public QGraphicsTextItem
	{
		Q_OBJECT
	public:
		ElementTitle() {}
		~ElementTitle() {}
	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
		{
			QGraphicsTextItem::mousePressEvent(event);
			if (!(event->modifiers() & Qt::ControlModifier))
				scene()->clearSelection();
			parentItem()->setSelected(true);
		}
		virtual void focusOutEvent(QFocusEvent *event)
		{
			QGraphicsTextItem::focusOutEvent(event);
			setTextInteractionFlags(Qt::NoTextInteraction);
		}
	};

	class NodeElement :  public QObject, public Element
	{
		Q_OBJECT

	public:
		/** @brief Конструктор */
		NodeElement();

		/** @brief Деструктор */
		virtual ~NodeElement();

		void complexInlineEditing();

		/** @brief Отрисовать объект */
		virtual void paint(QPainter *p, /**< Объект, осуществляющий отрисовку элементов */
						   const QStyleOptionGraphicsItem *opt, /**< Настройки отрисовки */
						   QWidget *w, /**< Виджет, на котором осуществляется отрисовка */
						   SdfRenderer *portrenderer /**< Рендерер портов)*/);

		/** @brief Получить область, в рамках которой осуществляется отрисовка объекта
			 *	@brief @return Область, в рамках которой осуществляется отрисовка объекта
			 * */
		QRectF boundingRect() const;

		/** @brief Получить область, в рамках которой возможна параметризация статического SVG
			 *	@brief @return Область, в рамках которой возможна параметризация статического SVG
			 * */
		QRectF contentsRect() const;

		/** @brief Обновить данные элемента */
		virtual void updateData();

		/** @brief Получить расположение порта
			 *	@brief @return Координаты порта
			 * */
		const QPointF getPortPos(qreal id /**< Идентификатор порта */) const;

		/** @brief Получить идентификатор порта
			 *	@brief @return Идентификатор порта
			 * */
		qreal getPortId(const QPointF &location /**< Расположение порта */) const;

		/** @brief Добавить связь */
		void addEdge(EdgeElement *edge);

		/** @brief Убрать связь */
		void delEdge(EdgeElement *edge);

		void setPortsVisible(bool value);

	public slots:
		void changeName();

	protected:
		/** @brief Обработать событие наведения на объект курсора мыши */
		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать событие отпускания кнопки мыши */
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать событие нажатия кнопки мыши */
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать изменение данных объекта
			 *	@brief @return Измененные данные
			 * */
		virtual QVariant itemChange(GraphicsItemChange change, /**< Тип изменений */
									const QVariant &value /**< Величина изменения */
									);

		bool mPortsVisible;

		/** @brief Список точечных портов */
		QList<QPointF> mPointPorts;
		/** @brief Список портов-линий */
		QList<StatLine> mLinePorts;
		/** @brief Область, в которой возможно отображение текста, параметризующего SVG */
		QRectF mContents;

		bool mLockChangeName;
		bool mLockUpdateText;

		// TODO: Этого тут вообще быть не должно.
		ElementTitle mDocVis;
		ElementTitle mDocType;
		QString mTypeText;
		QString mVisText;

		ElementTitle mTitle;

	private:
		/** @brief Направление растяжения элемента */
		enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };

		/** @brief Получить объект, расположенный в данной точке сцены
			*	@brief @return Объект, расположенный в данной точке сцены
			* */
		NodeElement *getNodeAt( const QPointF &position /**< Точка на сцене */);

		QLineF newTransform(const StatLine& port)  const;

		/** @brief Список ассоциированных с объектом связей */
		QList<EdgeElement *> mEdgeList;

		/** @brief Направление растяжения */
		DragState mDragState;

		/** @brief Описание двухмерной трансформации объекта */
		QTransform mTransform;
	};

	/** @brief Описание линейного порта, реагирующего на абсолютные координаты */
	struct StatLine
	{
		QLineF line;
		bool prop_x1;
		bool prop_y1;
		bool prop_x2;
		bool prop_y2;

		StatLine() : line(QLineF(0, 0, 0, 0)), prop_x1(false), prop_y1(false),
			prop_x2(false), prop_y2(false)
		{
		}

		operator QLineF () const
		{
			return line;
		}

		void operator = (QLineF const &l)
		{
			line = l;
			prop_x1 = false;
			prop_x2 = false;
			prop_y1 = false;
			prop_y2 = false;
		}
	};
}
