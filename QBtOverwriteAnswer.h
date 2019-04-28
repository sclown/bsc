#ifndef QBTOVERWRITEANSWER_H
#define QBTOVERWRITEANSWER_H

#include <QString>

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

    QBtOverwriteAnswer() :
        resolved_(false),
        dontAsk_(false),
        action_(ASK) {

    }
    QBtOverwriteAnswer(bool dontAsk, Action action, QString newName) :
        resolved_(true),
        dontAsk_(dontAsk),
        action_(action),
        newName_(newName) {

    }

    void reset() {
        resolved_ = false;
        newName_ = "";
    }
    bool valid() {
        return dontAsk_ || resolved_;
    }
    bool hasNewName() {
        return resolved_ && newName_.size();
    }
    Action action() {
        return action_;
    }
    QString newName() {
        return newName_;
    }
private:
    bool resolved_;
    bool dontAsk_;
    Action action_;
    QString newName_;

};

#endif // QBTOVERWRITEANSWER_H
