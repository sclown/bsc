#ifndef INCLUDED_QBtHeaderView_h
#define INCLUDED_QBtHeaderView_h

/********************************************************************
 * Custom header view that draws borders and styling without relying
 * on stylesheets, avoiding theme change caching issues.
 *******************************************************************/

#include <QHeaderView>

class QBtHeaderView : public QHeaderView
{
   Q_OBJECT

public:
   explicit QBtHeaderView( Qt::Orientation orientation, QWidget* parent = nullptr );

protected:
   void paintSection( QPainter* painter, const QRect& rect, int logicalIndex ) const override;
   void leaveEvent( QEvent* event ) override;
   void mouseMoveEvent( QMouseEvent* event ) override;
   QSize sizeHint() const override;

private:
   mutable int hoverSection_;
   static constexpr int HEADER_HEIGHT = 28;
   
   void drawSortIndicator( QPainter* painter, const QRect& rect, int logicalIndex, const QColor& textColor ) const;
};

#endif // INCLUDED_QBtHeaderView_h
