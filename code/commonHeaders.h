#ifndef COMMONHEADERS_H
#define COMMONHEADERS_H

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>

#include <QMainWindow>

#include <QString>
#include <QLabel>
#include <QWidget>
#include <QFile>
#include <QDir>
#include <QMap>
#include <QTreeWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QComboBox>
#include <QCheckBox>
#include <QStringList>
#include <QTextStream>
#include <QAbstractButton>
#include <QSignalMapper>
#include <QtXml/QDomDocument>

using namespace std;

#include "element.h"
#include "answer.h"
#include "question.h"
#include "quiz.h"
#include "user.h"
#include "quizlist.h"
#include "userslist.h"

namespace roles{
    enum roles
    {
        START,
        ID=1000,
        POINTER,
        TOPELEMENT,
        END
    };
}

namespace answersColumns
{
    enum answersColumns
    {
        START=-1,
        ISPROPER,
        TEXT,
        END
    };
}

#endif // COMMONHEADERS_H
