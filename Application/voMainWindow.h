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
#include <QScopedPointer>
#include <QMainWindow>
#include <QMap>

class QDockWidget;
class QSplitter;
class QTreeWidget;
class QTableWidget;

class voAnalysisPanel;
class voAnalysis;
class voView;

class voMainWindowPrivate;

class voMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  typedef QMainWindow Superclass;
  voMainWindow(QWidget * newParent = 0);
  virtual ~voMainWindow();
  
public slots:
  void onFileOpenActionTriggered();
  void onViewErrorLogActionTriggered();

  void about();

  void loadSampleDataset();

protected slots:
  void onAnalysisSelected(voAnalysis* analysis);
  void onActiveAnalysisChanged(voAnalysis* analysis);
  void onAboutToRunAnalysis(voAnalysis* analysis);

  void setViewActions(const QString& objectUuid, voView* newView);

protected:
  QScopedPointer<voMainWindowPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(voMainWindow);
  Q_DISABLE_COPY(voMainWindow);
};
