#ifndef LIMITSETTINGSMANAGER_H
#define LIMITSETTINGSMANAGER_H

#include <QObject>

class LimitSettingsManager : public QObject
{
   Q_OBJECT
public:
   enum LimitType { DailyLimit, WeekDayLimit };

   explicit LimitSettingsManager(QObject *parent = 0);
   ~LimitSettingsManager();

   void readLimit(const QString &username, LimitType limitType);
   void saveLimit(const QString &username, LimitType limitType, int limitMinutes);
   void enableLimit(const QString &username);
   void disableLimit(const QString &username);
   bool isLimitEnabled(const QString &username);

signals:

public slots:
};

#endif // LIMITSETTINGSMANAGER_H
