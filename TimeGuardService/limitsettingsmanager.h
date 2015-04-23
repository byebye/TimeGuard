#ifndef LIMITSETTINGSMANAGER_H
#define LIMITSETTINGSMANAGER_H

#include <QObject>
#include <QJsonObject>
#include "user.h"

enum class LimitType { DailyLimit, WeekDayLimit };

class LimitSettingsManager : public QObject
{
   Q_OBJECT
public:

   explicit LimitSettingsManager(QObject *parent = 0);
   ~LimitSettingsManager();

   int readLimit(const User &user, LimitType limitType);
   bool save(const User &user, const QString &option, const QVariant &value);
   bool saveLimit(const User &user, LimitType limitType, int limitSeconds);
   bool enableLimit(const User &user);
   bool disableLimit(const User &user);
   bool isLimitEnabled(const User &user);
   void generateDefaultSettingsFile(const User &user);
signals:

public slots:
   QJsonObject readSettings(const User &user);
   bool writeSettings(const User &user, const QJsonObject &settings);
   QString optionToStringName(LimitType limitType);
   QString generateSettingsFileName(const QString &userName);
private:

};

#endif // LIMITSETTINGSMANAGER_H
