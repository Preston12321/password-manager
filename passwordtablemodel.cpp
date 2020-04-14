#include "passwordtablemodel.h"

#include <QtDebug>

PasswordTableModel::PasswordTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
    entries = QList<PasswordEntry *>();

    // Fill with fake entries for now
    for (int i = 1; i <= 25; i++) {
        auto entry = new PasswordEntry();
        entry->refreshLastUpdated();
        entry->setNameOrURL(QString::number(i));
        entries.append(entry);
    }
}

int PasswordTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return entries.size();
}

int PasswordTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant PasswordTableModel::data(const QModelIndex &index, int role) const {
    if (index.row() >= entries.size()) {
        return QVariant();
    }

    auto entry = entries.at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case 0:
                    return QVariant(entry->getNameOrURL());
                case 1:
                    return QVariant(entry->getLastUpdated().toString());
                default:
                    return QVariant();
            }
        case Qt::DecorationRole:
            return QVariant();
        case Qt::EditRole:
            switch (index.column()) {
                case 0:
                    return QVariant(entry->getNameOrURL());
                default:
                    return QVariant();
            }
        case Qt::ToolTipRole:
            return QVariant();
        case Qt::StatusTipRole:
            return QVariant();
        case Qt::WhatsThisRole:
            return QVariant();
        case Qt::SizeHintRole:
            return QVariant();
        default:
            return QVariant();
    }
}

QVariant PasswordTableModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const {
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case 0:
                    return orientation == Qt::Orientation::Horizontal
                               ? QVariant("Name/URL")
                               : QVariant();
                case 1:
                    return orientation == Qt::Orientation::Horizontal
                               ? QVariant("Last Updated")
                               : QVariant();
                default:
                    return QVariant();
            }
        case Qt::DecorationRole:
            return QVariant();
        case Qt::EditRole:
            return QVariant();
        case Qt::ToolTipRole:
            return QVariant();
        case Qt::StatusTipRole:
            return QVariant();
        case Qt::WhatsThisRole:
            return QVariant();
        case Qt::SizeHintRole:
            return QVariant();
        default:
            return QVariant();
    }
}

bool PasswordTableModel::setData(const QModelIndex &index,
                                 const QVariant &value, int role) {
    if (index.row() >= entries.size()) {
        return false;
    }

    auto entry = entries.at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return false;
        case Qt::DecorationRole:
            return false;
        case Qt::EditRole:
            switch (index.column()) {
                case 0:
                    entry->setNameOrURL(value.toString());
                    emit dataChanged(index, index);
                    return true;
                default:
                    return false;
            }
        case Qt::ToolTipRole:
            return false;
        case Qt::StatusTipRole:
            return false;
        case Qt::WhatsThisRole:
            return false;
        case Qt::SizeHintRole:
            return false;
        default:
            return false;
    }
}

Qt::ItemFlags PasswordTableModel::flags(const QModelIndex &index) const {
    if (index.row() >= entries.size()) {
        return Qt::ItemFlag::NoItemFlags;
    }

    auto flags = Qt::ItemFlags();
    flags.setFlag(Qt::ItemFlag::ItemIsEnabled);
    flags.setFlag(Qt::ItemFlag::ItemIsSelectable);

    if (index.column() == 0) {
        flags.setFlag(Qt::ItemFlag::ItemIsEditable);
    }

    return flags;
}
