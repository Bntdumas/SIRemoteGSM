#ifndef SINUMBERCHOOSERDIALOG_H
#define SINUMBERCHOOSERDIALOG_H

#include <QDialog>
#include <QListView>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QStringListModel>

class SINumberChooserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SINumberChooserDialog(QWidget *parent = 0);

    void setSINumbers(const QStringList& numbers);
    QString selectedNumber() const { return m_selectedNumber; }

private:
    QStringListModel* m_model;
    QListView* m_view;
    QHBoxLayout* m_layout;
    QDialogButtonBox* m_btnBox;

    QString m_selectedNumber;

private slots:
    void doubleClicked(const QModelIndex& index);
};

#endif // SINUMBERCHOOSERDIALOG_H
