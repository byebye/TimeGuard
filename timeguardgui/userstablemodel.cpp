#include "userstablemodel.h"
#include <QPushButton>
#include <QDebug>

UsersTableModel::UsersTableModel(QObject *parent) :
  QAbstractTableModel(parent),
  headerValues({"", tr("Username"), tr("Limit status"), tr("Configuration files"), tr("Today's' limit"), tr("Time used today")}),
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
        return gridData[row][col];
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
          return gridData[row][0];
      break;
  }
  return QVariant();
}

bool UsersTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//  if(role == Qt::EditRole)
//  {
//    gridData[index.row()][index.column()] = value.toString();
//    emit dataChanged(index, index);
//  }
  int row = index.row();
  int col = index.column();
  if(role == Qt::CheckStateRole)
  {
    if((Qt::CheckState) value.toInt() == Qt::Checked)
    {
      gridData[row][col] = Qt::Checked;
//      selectedRows[index.row()] = Qt::Checked;
    }
    else
    {
      gridData[row][col] = Qt::Unchecked;
//      selectedRows[index.row()] = Qt::Unchecked;
    }
    return true;
  }
  return false;
}

bool UsersTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row + count - 1);
  rowsNumber += count;
  for(int i = 0; i < count; ++i)
  {
    QVector<QVariant> dataToInsert;
    dataToInsert.push_back(Qt::Unchecked);
    for(int j = 0; j < columnCount(); ++j)
      dataToInsert.push_back(QString("%1,%2").arg(row + i).arg(j));
    gridData.insert(gridData.begin() + row + i, dataToInsert);
//    selectedRows.insert(selectedRows.begin() + row + i, Qt::Unchecked);
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
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  if(index.column() == 0)
    flags |= Qt::ItemIsUserCheckable;
  return flags;
}
