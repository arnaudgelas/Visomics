/*=========================================================================

  Program: Visomics

  Copyright (c) Kitware, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

// Qt includes
#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLayout>
#include <QTableWidget>
//#include <QHeaderView>

// Visomics includes
#include "voApplication.h"
#include "voAnalysisDriver.h"
#include "voDataObject.h"
#include "voIOManager.h"
#include "voKEGGTableView.h"
#include "voUtils.h"

// VTK includes
#include <vtkTable.h>
#include <vtkVariant.h>

// --------------------------------------------------------------------------
class voKEGGTableViewPrivate
{
public:
  voKEGGTableViewPrivate();

  QTableWidget* TableWidget;
};

// --------------------------------------------------------------------------
// voKEGGTableViewPrivate methods

// --------------------------------------------------------------------------
voKEGGTableViewPrivate::voKEGGTableViewPrivate()
{
  this->TableWidget = 0;
}

// --------------------------------------------------------------------------
// voKEGGTableView methods

// --------------------------------------------------------------------------
voKEGGTableView::voKEGGTableView(QWidget* newParent):
    Superclass(newParent), d_ptr(new voKEGGTableViewPrivate)
{
}

// --------------------------------------------------------------------------
voKEGGTableView::~voKEGGTableView()
{
  Q_D(voKEGGTableView);

  delete d->TableWidget;
}

// --------------------------------------------------------------------------
QList<QAction*> voKEGGTableView::actions()
{
  QList<QAction*> actionList = this->Superclass::actions();

  QAction * exportToCsvAction = new QAction(QIcon(":/Icons/csv_text.png"), "Export as CSV", this);
  exportToCsvAction->setToolTip("Export current table view as CSV text file.");
  connect(exportToCsvAction, SIGNAL(triggered()), this, SLOT(onExportToCsvActionTriggered()));
  actionList << exportToCsvAction;

  return actionList;
}

// --------------------------------------------------------------------------
void voKEGGTableView::onExportToCsvActionTriggered()
{
  QString defaultFileName = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)
      + "/" + voUtils::cleanString(this->objectName()) + ".csv";

  QString fileName = QFileDialog::getSaveFileName(
        0, tr("Save table data..."), defaultFileName, "Comma Separated Value (*.csv)");
  if(fileName.isEmpty())
    {
    return;
    }
  vtkTable * table = vtkTable::SafeDownCast(this->dataObject()->dataAsVTKDataObject());
  Q_ASSERT(table);
  voIOManager::writeTableToCVSFile(table, fileName);
}

// --------------------------------------------------------------------------
void voKEGGTableView::setupUi(QLayout *layout)
{
  Q_D(voKEGGTableView);

  d->TableWidget = new QTableWidget();
  layout->addWidget(d->TableWidget);

  QObject::connect(d->TableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
                   this, SLOT(onItemDoubleClicked(QTableWidgetItem*)));
}

// --------------------------------------------------------------------------
QString voKEGGTableView::hints()const
{
  return "<img src=\":/Icons/Bulb.png\">&nbsp;If you <b>double-click</b> on a pathway cell, a "
      "<b>KEGG Pathway</b> analysis will be run.";
}

// --------------------------------------------------------------------------
void voKEGGTableView::setDataObjectInternal(const voDataObject& dataObject)
{
  Q_D(voKEGGTableView);

  vtkTable * table = vtkTable::SafeDownCast(dataObject.dataAsVTKDataObject());
  if (!table)
    {
    qCritical() << "voKEGGTableView - Failed to setDataObject - vtkTable data is expected !";
    return;
    }

  vtkIdType num_rows = table->GetNumberOfRows();
  vtkIdType num_cols = table->GetNumberOfColumns();
  d->TableWidget->setRowCount(num_rows);
  d->TableWidget->setColumnCount(num_cols - 1);
  for (vtkIdType c = 1; c < num_cols; ++c)
    {
    d->TableWidget->setHorizontalHeaderItem(c-1, new QTableWidgetItem(QString(table->GetColumnName(c))));
    for (vtkIdType r = 0; r < num_rows; ++r)
      {
      QStringList rawTexts = QString(table->GetValue(r, c).ToString()).split("#");
      QTableWidgetItem* item = new QTableWidgetItem();
      item->setText(rawTexts.at(0));
      item->setToolTip(rawTexts.value(1, ""));
      item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable); // Item is view-only
      d->TableWidget->setItem(r, c-1, item);
      }
    }
  for (vtkIdType r = 0; r < num_rows; ++r)
    {
    d->TableWidget->setVerticalHeaderItem(r, new QTableWidgetItem(QString(table->GetValue(r, 0).ToString())));
    }
  d->TableWidget->resizeColumnsToContents();
}

// --------------------------------------------------------------------------
void voKEGGTableView::onItemDoubleClicked(QTableWidgetItem* item)
{
  if (item->text().startsWith("path:"))
    {
    QHash<QString, QVariant> parameters;
    parameters.insert("pathway_id", QVariant(item->text()));

    voApplication::application()->analysisDriver()->runAnalysisForCurrentInput("KEGG Pathway", parameters);
    }
}
