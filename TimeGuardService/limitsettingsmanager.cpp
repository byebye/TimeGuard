#include "limitsettingsmanager.h"

LimitSettingsManager::LimitSettingsManager(QObject *parent) : QObject(parent)
{

}

LimitSettingsManager::~LimitSettingsManager()
{

}

int LimitSettingsManager::readLimit(const User &user, LimitSettingsManager::LimitType limitType)
{
   return 0;
}

void LimitSettingsManager::saveLimit(const User &user, LimitSettingsManager::LimitType limitType, int limitMinutes)
{

}

void LimitSettingsManager::enableLimit(const User &user)
{

}

void LimitSettingsManager::disableLimit(const User &user)
{

}

bool LimitSettingsManager::isLimitEnabled(const User &user)
{
   return false;
}

