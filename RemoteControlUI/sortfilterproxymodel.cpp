#include "sortfilterproxymodel.h"

#include <QTime>
#include <QDebug>

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool SortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant lData = sourceModel()->data(left, sortRole());
    QVariant rData = sourceModel()->data(right, sortRole());

    if(lData.type() == QVariant::Time) {
        return lData.toTime() < rData.toTime();
    } else {
        return QSortFilterProxyModel::lessThan(left, right);
    }
}

void SortFilterProxyModel::resort()
{
    sort(0, Qt::DescendingOrder);
}
