#include "sinumberchooserdialog.h"

SINumberChooserDialog::SINumberChooserDialog(QWidget *parent) :
    QDialog(parent)
{
    m_model = new QStringListModel(this);
    m_view = new QListView(this);
    m_view->setModel(m_model);
    connect(m_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));

    m_btnBox = new QDialogButtonBox(this);
    m_btnBox->setStandardButtons(QDialogButtonBox::Cancel);
    connect(m_btnBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_layout = new QHBoxLayout;
    m_layout->addWidget(m_view);
    m_layout->addWidget(m_btnBox);
    this->setLayout(m_layout);
    this->setMinimumSize(QSize(200, 300));
}

void SINumberChooserDialog::setSINumbers(const QStringList &numbers)
{
    QStringList tmp = numbers;
    tmp.sort();
    m_model->setStringList(tmp);
}

void SINumberChooserDialog::doubleClicked(const QModelIndex &index)
{
    m_selectedNumber = index.data().toString();
    this->accept();
}
