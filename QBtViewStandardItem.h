#ifndef INCLUDED_QBtViewStandardItem_h
#define INCLUDED_QBtViewStandardItem_h

/*------- include files:
-------------------------------------------------------------------*/
#include <QStandardItem>


/*------- class declaration:
-------------------------------------------------------------------*/
class QBtViewStandardItem : public QStandardItem
{
//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtViewStandardItem() : QStandardItem()
   {
      setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
   }
};

#endif // INCLUDED_QBtViewStandardItem_h
