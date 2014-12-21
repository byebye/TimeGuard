#include "timelimit.h"
#include <QDebug>

TimeLimit::TimeLimit(QObject *parent) :
  TimeLimit(0, parent)
{
}

TimeLimit::TimeLimit(int timeSeconds, QObject *parent) :
  timeRemaining(timeSeconds), QObject(parent)
{
}

TimeLimit::TimeLimit(QString timeString, QObject *parent) :
  TimeLimit(fromString(timeString), parent)
{
}

TimeLimit::TimeLimit(QTime qtime, QObject *parent) :
  TimeLimit(qtime.toString("hh:mm:ss"), parent)
{
}

TimeLimit::TimeLimit(const TimeLimit &timeLimit, QObject *parent) :
  TimeLimit(timeLimit.getTimeRemaining(), parent)
{
}

int TimeLimit::getTimeRemaining() const {
  return timeRemaining;
}

TimeLimit TimeLimit::convertToWeeklyTimeRemaining(int daysInWeek) {
  timeRemaining *= daysInWeek;
  return *this;
}

TimeLimit TimeLimit::convertToMonthlyTimeRemaining(int daysInMonth) {
  timeRemaining *= daysInMonth;
  return *this;
}

void TimeLimit::setTimeRemaining(int timeLimit)
{
  timeRemaining = timeLimit;
}

bool TimeLimit::isTimeOut()
{
  return timeRemaining <= 0;
}

void TimeLimit::secondsElapsed(int seconds)
{
  timeRemaining -= seconds;
}

int TimeLimit::fromString(QString timeString)
{
  const int multipliers[] = { HOUR, MINUTE, SECOND };
  QStringList timeParts = timeString.split(":", QString::SkipEmptyParts);
  int timeSeconds = 0;
  for(int i = 0; i < timeParts.size(); ++i)
    timeSeconds += timeParts[i].toInt() * multipliers[i];
  return timeSeconds;
}

QString TimeLimit::toString(int timeSeconds)
{
  int hours = timeSeconds / HOUR;
  timeSeconds %= HOUR;
  int minutes = timeSeconds / MINUTE;
  timeSeconds %= MINUTE;
  int seconds = timeSeconds;
  return QString("%1:%2:%3")
      .arg(hours, 2, 10, QChar('0'))
      .arg(minutes, 2, 10, QChar('0'))
      .arg(seconds, 2, 10, QChar('0'));
}

QString TimeLimit::toString()
{
  return toString(timeRemaining);
}

bool TimeLimit::operator <(const TimeLimit &that) const
{
  return (getTimeRemaining() < that.getTimeRemaining());
}

TimeLimit & TimeLimit::operator =(TimeLimit rhs)
{
  if(&rhs != this)
    this->setTimeRemaining(rhs.getTimeRemaining());
  return *this;
}
