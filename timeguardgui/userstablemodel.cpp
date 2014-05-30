#include "userstablemodel.h"
#include <QPushButton>

UsersTableModel::UsersTableModel(QObject *parent) :
  QAbstractTableModel(parent)
{
  headerValues << tr("Username") << tr("Limit") << tr("Files");
  for(int i = 0; i < 2; ++i)
  {
    gridData.push_back(QVector<QString>());
    for(int j = 0; j < 3; ++j)
      gridData[i].push_back(QString("%1,%2").arg(i).arg(j));
  }
}

int UsersTableModel::rowCount(const QModelIndex &parent) const
{
   return 2;
}

int UsersTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant UsersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role == Qt::DisplayRole)
  {
    if(orientation == Qt::Horizontal)
      return section;
    else
      return section + 1;
  }
  return QVariant();
}

QVariant UsersTableModel::data(const QModelIndex &index, int role) const
{
  if(role == Qt::DisplayRole)
    return gridData[index.row()][index.column()];
  return QVariant();

}

bool UsersTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if(role == Qt::EditRole)
  {
    gridData[index.row()][index.column()] = value.toString();
    emit dataChanged(index, index);
  }
  return true;
}

bool UsersTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row + count - 1);

  endInsertRows();
  return true;
}

bool UsersTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, row + count - 1);

  endRemoveRows();
  return true;
}

Qt::ItemFlags UsersTableModel::flags(const QModelIndex &index) const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
