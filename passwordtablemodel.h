#ifndef PASSWORDTABLEMODEL_H
#define PASSWORDTABLEMODEL_H

#include <QAbstractTableModel>

#include "passwordentry.h"

class PasswordTableModel : public QAbstractTableModel {
public:
    PasswordTableModel(QObject *parent = nullptr);
    virtual int rowCount(
        const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(
        const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<PasswordEntry *> entries;
};

#endif  // PASSWORDTABLEMODEL_H
