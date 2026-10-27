#ifndef QBTOVERWRITEANSWER_H
#define QBTOVERWRITEANSWER_H

#include <QString>

/*------- class declaration:
-------------------------------------------------------------------
 * The user's decision about one conflicting item. With dontAsk set
 * the same action is to be applied to every later conflict as well.
-------------------------------------------------------------------*/
class QBtOverwriteAnswer
{
public:
    enum ErrorType {
        EXIST,
        READ_ERROR,
        WRITE_ERROR
    };

    enum Action {
        ASK = 0,
        CANCEL,
        SKIP,
        OVERWRITE,
        UPDATE,
        RENAME,
        AUTORENAME
    };

    QBtOverwriteAnswer( bool dontAsk, Action action, QString newName ) :
        dontAsk_( dontAsk ),
        action_( action ),
        newName_( newName ) {
    }

    bool dontAsk() const {
        return dontAsk_;
    }
    Action action() const {
        return action_;
    }
    QString newName() const {
        return newName_;
    }
private:
    bool    dontAsk_;
    Action  action_;
    QString newName_;
};

#endif // QBTOVERWRITEANSWER_H
