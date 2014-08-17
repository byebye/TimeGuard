#include "userstablemodel.h"
#include <QPushButton>
#include <QDebug>

UsersTableModel::UsersTableModel(QObject *parent) :
  QAbstractTableModel(parent),
  headerValues({"", tr("Username"), tr("Limit status"), tr("Today limit"), tr("Time used today")}),
  rowsNumber(0),
  columnsNumber(headerValues.size())
{
}

int UsersTableModel::rowCount(const QModelIndex &parent) const
{
   return rowsNumber;
}

int UsersTableModel::columnCount(const QModelIndex &parent) const
{
  return columnsNumber;
}

QVariant UsersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role == Qt::DisplayRole)
  {
    if(orientation == Qt::Horizontal)
      return headerValues[section];
    else
      return section + 1;
  }
  return QVariant();
}

QVariant UsersTableModel::data(const QModelIndex &index, int role) const
{
  int row = index.row();
  int col = index.column();
  switch(role)
  {
    case Qt::DisplayRole:
      if(col >= 1)
        return gridData[row][col-1];
      break;
    case Qt::FontRole:
      break;
    case Qt::BackgroundRole:
      break;
    case Qt::TextAlignmentRole:
      if(col == 0)
        return Qt::AlignCenter;
      break;
    case Qt::CheckStateRole:
      if(col == 0)
        return selectedRows[row];
      break;
  }
  return QVariant();
}

bool UsersTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int row = index.row();
  int col = index.column();
  if(col == 0 && role == Qt::CheckStateRole)
  {
    if(static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked)
      selectedRows[row] = Qt::Checked;
    else
      selectedRows[row] = Qt::Unchecked;
    return true;
  }
  return false;
}

bool UsersTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row + count - 1);
  rowsNumber += count;
  endInsertRows();
  return true;
}

bool UsersTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, row + count - 1);
  rowsNumber -= count;
  auto begin = gridData.begin() + row;
  auto end = begin + count;
  gridData.erase(begin, end);
  endRemoveRows();
  return true;
}

Qt::ItemFlags UsersTableModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  if(index.column() == 0)
    flags |= Qt::ItemIsUserCheckable;
  return flags;
}

void UsersTableModel::setUsersData(QVector<QVector<QVariant>> &settings)
{
  const int usersNumber = settings.size();
  selectedRows.fill(Qt::Unchecked, usersNumber);
  gridData.resize(usersNumber);
  for(int i = 0; i < usersNumber; ++i)
    for(auto setting : settings[i])
      gridData[i].push_back(setting);
  insertRows(0, usersNumber);
}

QStringList UsersTableModel::getSelectedUsers()
{
  QStringList selectedUsers;
  for(int i = 0; i < selectedRows.size(); ++i)
    if(selectedRows[i] == Qt::Checked)
      selectedUsers.push_back(gridData[i][0].toString());
  return selectedUsers;
}
