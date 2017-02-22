#include "QBtCommand.h"
#include "QBtSettings.h"
#include "QBTMacTools.h"
#include "QBtEventsController.h"
#include "QBtEvent.h"


bool isCommand(int in_argc, char **in_argv)
{
    if(in_argc < 2) {
        return false;
    }
    QBtSettings stt;
    stt.save("Command", QVariant(QString(in_argv[1])));
    activateAnotherInstance();
    return true;
}

void handleCommand()
{
    QBtSettings stt;
    QVariant commandData;
    if(stt.read("Command", commandData)) {
        QBtEventsController::instance()->send_event( QBtEvent::OPEN_DIR, commandData );
    }
    stt.remove("Command");
}
