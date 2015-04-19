#ifndef LIMITSETTINGSMANAGER_H
#define LIMITSETTINGSMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include "user.h"

class LimitSettingsManager : public QObject
{
   Q_OBJECT
public:
   enum LimitType { DailyLimit, WeekDayLimit };

   explicit LimitSettingsManager(QObject *parent = 0);
   ~LimitSettingsManager();

   int readLimit(const User &user, LimitType limitType);
   bool saveLimit(const User &user, LimitType limitType, int limitSeconds);
   bool enableLimit(const User &user);
   bool disableLimit(const User &user);
   bool isLimitEnabled(const User &user);
   void generateDefaultSettingsFile(const User &user);
signals:

public slots:
   QJsonDocument readSettings(const User &user);
   bool writeSettings(const User &user, const QJsonDocument &settings);
   QString optionToStringName(LimitType limitType);
   QString generateSettingsFileName(const QString &userName);
private:

};

#endif // LIMITSETTINGSMANAGER_H
