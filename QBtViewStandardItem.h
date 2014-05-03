#ifndef INCLUDED_QBtViewStandardItem_h
#define INCLUDED_QBtViewStandardItem_h

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtConfig.h"
#include <QStandardItem>
#include <QPalette>


/*------- class declaration:
-------------------------------------------------------------------*/
class QBtViewStandardItem : public QStandardItem
{
//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtViewStandardItem() : QStandardItem()
   {
      setTextAlignment( Qt::AlignHCenter );
   }
};

#endif // INCLUDED_QBtViewStandardItem_h
