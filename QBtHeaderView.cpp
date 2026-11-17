#include "QBtHeaderView.h"
#include <QPainter>
#include <QMouseEvent>

QBtHeaderView::QBtHeaderView( Qt::Orientation orientation, QWidget* parent )
   : QHeaderView( orientation, parent )
   , hoverSection_( -1 )
{
   setMouseTracking( true );
}

QSize QBtHeaderView::sizeHint() const
{
   QSize hint = QHeaderView::sizeHint();
   if( orientation() == Qt::Horizontal ) {
      hint.setHeight( HEADER_HEIGHT );
   }
   return hint;
}

void QBtHeaderView::paintSection( QPainter* painter, const QRect& rect, int logicalIndex ) const
{
   if( !rect.isValid() ) return;

   painter->save();

   // Get colors from the current palette (automatically updates with theme)
   const QPalette pal = palette();
   const QColor base = pal.color( QPalette::Base );
   const QColor text = pal.color( QPalette::Text );
   
   // Compute mixed colors for subtle borders
   const auto mix = [&]( qreal t ) {
      return QColor::fromRgbF( base.redF()   + ( text.redF()   - base.redF()   ) * t,
                               base.greenF() + ( text.greenF() - base.greenF() ) * t,
                               base.blueF()  + ( text.blueF()  - base.blueF()  ) * t );
   };

   // Draw background
   painter->fillRect( rect, base );

   // Draw text
   QColor textColor = ( logicalIndex == hoverSection_ ) ? text : mix( 0.55 );
   painter->setPen( textColor );
   
   QFont f = font();
   f.setWeight( QFont::DemiBold );
   painter->setFont( f );
   
   const QString headerText = model() ? model()->headerData( logicalIndex, orientation(), Qt::DisplayRole ).toString() : QString();
   
   // Reserve space for sort indicator if this section has it
   QRect textRect = rect.adjusted( 6, 0, -6, 0 );
   if( logicalIndex == sortIndicatorSection() && isSortIndicatorShown() ) {
      textRect.adjust( 0, 0, -20, 0 );  // Reserve 20 pixels for sort indicator
   }
   
   const int flags = Qt::AlignLeft | Qt::AlignVCenter;
   painter->drawText( textRect, flags, headerText );

   // Draw sort indicator if this is the sorted column
   if( logicalIndex == sortIndicatorSection() && isSortIndicatorShown() ) {
      drawSortIndicator( painter, rect, logicalIndex, textColor );
   }

   // Draw right border (except for last section)
   if( logicalIndex < count() - 1 ) {
      painter->setPen( QPen( mix( 0.10 ), 1 ) );
      painter->drawLine( rect.topRight(), rect.bottomRight() );
   }

   // Draw bottom border
   painter->setPen( QPen( mix( 0.15 ), 1 ) );
   painter->drawLine( rect.bottomLeft(), rect.bottomRight() );

   painter->restore();
}

void QBtHeaderView::drawSortIndicator( QPainter* painter, const QRect& rect, int logicalIndex, const QColor& textColor ) const
{
   const int arrowSize = 6;
   const int margin = 4;
   
   // Position arrow on the right side of the text
   int x = rect.right() - arrowSize - margin;
   int y = rect.center().y();
   
   painter->setPen( QPen( textColor, 1.5 ) );
   
   // Draw two-line arrow (^ or v shape)
   if( sortIndicatorOrder() == Qt::AscendingOrder ) {
      // Up arrow: ^
      painter->drawLine( x, y + 2, x + arrowSize / 2, y - 2 );
      painter->drawLine( x + arrowSize / 2, y - 2, x + arrowSize, y + 2 );
   } else {
      // Down arrow: v
      painter->drawLine( x, y - 2, x + arrowSize / 2, y + 2 );
      painter->drawLine( x + arrowSize / 2, y + 2, x + arrowSize, y - 2 );
   }
}

void QBtHeaderView::leaveEvent( QEvent* event )
{
   if( hoverSection_ != -1 ) {
      hoverSection_ = -1;
      viewport()->update();
   }
   QHeaderView::leaveEvent( event );
}

void QBtHeaderView::mouseMoveEvent( QMouseEvent* event )
{
   const int section = logicalIndexAt( event->pos() );
   if( section != hoverSection_ ) {
      hoverSection_ = section;
      viewport()->update();
   }
   QHeaderView::mouseMoveEvent( event );
}
