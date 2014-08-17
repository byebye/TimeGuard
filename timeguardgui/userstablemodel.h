#ifndef USERSTABLEMODEL_H
#define USERSTABLEMODEL_H

#include <QAbstractTableModel>

class UsersTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit UsersTableModel(QObject *parent = 0);
  int rowCount(const QModelIndex &parent = QModelIndex()) const ;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
  Qt::ItemFlags flags(const QModelIndex &index) const;
  void setUsersData(QVector<QVector<QVariant>> &settings);
signals:

public slots:

private:
  QVector<QVector<QVariant>> gridData;
  QVector<Qt::CheckState> selectedRows;
  QStringList headerValues;
  int rowsNumber;
  int columnsNumber;
};

#endif // USERSTABLEMODEL_H
