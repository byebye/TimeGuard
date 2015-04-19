#ifndef LIMITSETTINGSMANAGER_H
#define LIMITSETTINGSMANAGER_H

#include <QObject>
#include "user.h"

class LimitSettingsManager : public QObject
{
   Q_OBJECT
public:
   enum LimitType { DailyLimit, WeekDayLimit };

   explicit LimitSettingsManager(QObject *parent = 0);
   ~LimitSettingsManager();

   int readLimit(const User &user, LimitType limitType);
   void saveLimit(const User &user, LimitType limitType, int limitMinutes);
   void enableLimit(const User &user);
   void disableLimit(const User &user);
   bool isLimitEnabled(const User &user);

signals:

public slots:
};

#endif // LIMITSETTINGSMANAGER_H
