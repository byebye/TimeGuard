#include "userstablemodel.h"
#include <QPushButton>

UsersTableModel::UsersTableModel(QObject *parent) :
  QAbstractTableModel(parent),
  rowsNumber(0), columnsNumber(3)
{
  headerValues = {tr("Username"), tr("Limit"), tr("Files")};
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
  rowsNumber += count;
  for(int i = 0; i < count; ++i)
  {
    QVector<QString> dataToInsert;
    for(int j = 0; j < columnCount(); ++j)
      dataToInsert.push_back(QString("%1,%2").arg(row + i).arg(j));
    gridData.insert(gridData.begin() + row + i, dataToInsert);
  }
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
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
