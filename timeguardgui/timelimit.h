#ifndef TIMELIMIT_H
#define TIMELIMIT_H

#include <QObject>
#include <QTime>
#include <QStringList>

class TimeLimit : public QObject
{
  Q_OBJECT
public:
  explicit TimeLimit(QObject *parent = 0);
  TimeLimit(int timeSeconds, QObject *parent = 0);
  TimeLimit(QString timeString, QObject *parent = 0);
  TimeLimit(QTime qtime, QObject *parent = 0);
  TimeLimit(const TimeLimit &timeLimit, QObject *parent = 0);

  int getTimeRemaining() const;
  TimeLimit convertToWeeklyTimeRemaining(int daysInWeek = 7);
  TimeLimit convertToMonthlyTimeRemaining(int daysInMonth);
  void setTimeRemaining(int timeLimit);
  bool isTimeOut();
  void secondsElapsed(int seconds);
  int fromString(QString timeString);
  QString toString();
  QString toString(int timeSeconds);
  bool operator<(const TimeLimit &that) const;
  TimeLimit & operator=(TimeLimit rhs);
signals:

public slots:

private:
  int timeRemaining;
  enum SecondMultipliers { DAY = 24*60*60, HOUR = 60*60, MINUTE = 60, SECOND = 1 };
};

#endif // TIMELIMIT_H
