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
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>
#include <QPixmap>
#include <QVariant>

// Visomics includes
#include "voDataObject.h"
#include "voKEGGImageView.h"

// VTK includes
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>

// --------------------------------------------------------------------------
class voKEGGImageViewPrivate
{
public:
  voKEGGImageViewPrivate();

  QGraphicsScene* Scene;
  QGraphicsView* View;
};

// --------------------------------------------------------------------------
// voKEGGImageViewPrivate methods

// --------------------------------------------------------------------------
voKEGGImageViewPrivate::voKEGGImageViewPrivate()
{
  this->Scene = 0;
  this->View = 0;
}

// --------------------------------------------------------------------------
// voKEGGImageView methods

// --------------------------------------------------------------------------
voKEGGImageView::voKEGGImageView(QWidget * newParent):
    Superclass(newParent), d_ptr(new voKEGGImageViewPrivate)
{
}

// --------------------------------------------------------------------------
voKEGGImageView::~voKEGGImageView()
{
  Q_D(voKEGGImageView);

  delete d->Scene;
  delete d->View;
}

// --------------------------------------------------------------------------
void voKEGGImageView::setupUi(QLayout *layout)
{
  Q_D(voKEGGImageView);

  d->Scene = new QGraphicsScene();

  d->View = new QGraphicsView();
  d->View->setScene(d->Scene);
  d->View->setDragMode(QGraphicsView::ScrollHandDrag);
  layout->addWidget(d->View);
}

// --------------------------------------------------------------------------
void voKEGGImageView::setDataObjectInternal(const voDataObject& dataObject)
{
  Q_D(voKEGGImageView);

  if (!dataObject.data().canConvert<QPixmap>())
    {
    qCritical() << "voKEGGImageView - Failed to setDataObject - dataObject does not contain a QPixmap";
    return;
    }
  QPixmap pixmap = dataObject.data().value<QPixmap>();

  d->Scene->addPixmap(pixmap);
}
