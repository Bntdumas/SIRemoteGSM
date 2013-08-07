#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent = 0);
    
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public slots:
    void resort();
};

#endif // SORTFILTERPROXYMODEL_H
