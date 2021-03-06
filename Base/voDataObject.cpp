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
#include <QUuid>
#include <QVariant>

// Visomics includes
#include "voDataObject.h"

// VTK includes
#include <vtkDataObject.h>

class voDataObjectPrivate
{
public:
  voDataObjectPrivate();

  QString                        Name;
  QString                        Uuid;
  QVariant                       Data;
};

// --------------------------------------------------------------------------
// voDataObjectPrivate methods

// --------------------------------------------------------------------------
voDataObjectPrivate::voDataObjectPrivate()
{
  qRegisterMetaType<vtkVariant>("vtkVariant");
  this->Uuid = QUuid::createUuid().toString();
}
  
// --------------------------------------------------------------------------
// voDataObject methods

// --------------------------------------------------------------------------
voDataObject::voDataObject(QObject* newParent) :
    Superclass(newParent), d_ptr(new voDataObjectPrivate)
{
}

// --------------------------------------------------------------------------
voDataObject::voDataObject(const QString& newName, const QVariant& newData, QObject* newParent):
    Superclass(newParent), d_ptr(new voDataObjectPrivate)
{
  Q_D(voDataObject);
  d->Name = newName;
  this->setData(newData);
}

// --------------------------------------------------------------------------
voDataObject::voDataObject(const QString& newName, vtkDataObject * newData, QObject* newParent):
    Superclass(newParent), d_ptr(new voDataObjectPrivate)
{
  Q_D(voDataObject);
  d->Name = newName;
  this->setData(newData);
}

// --------------------------------------------------------------------------
voDataObject::~voDataObject()
{
  Q_D(voDataObject);
  if (this->isVTKDataObject())
    {
    this->dataAsVTKDataObject()->Delete();
    }
  else if (d->Data.canConvert<QObject*>())
    {
    QObject * object = d->Data.value<QObject*>();
    if (!object->parent())
      {
      delete object;
      }
    }
}

// --------------------------------------------------------------------------
QString voDataObject::name()const
{
  Q_D(const voDataObject);
  return d->Name;
}

// --------------------------------------------------------------------------
void voDataObject::setName(const QString& newName)
{
  Q_D(voDataObject);
  d->Name = newName;
}

// --------------------------------------------------------------------------
QString voDataObject::type()const
{
  Q_D(const voDataObject);
  if (this->isVTKDataObject())
    {
    return this->dataAsVTKDataObject()->GetClassName();
    }
  else
    {
    return QLatin1String(d->Data.typeName());
    }
}

// --------------------------------------------------------------------------
QString voDataObject::uuid()const
{
  Q_D(const voDataObject);
  return d->Uuid;
}

// --------------------------------------------------------------------------
QVariant voDataObject::data()const
{
  Q_D(const voDataObject);
  return d->Data;
}

// --------------------------------------------------------------------------
void voDataObject::setData(const QVariant& newData)
{
  Q_D(voDataObject);
  d->Data = newData;
  this->setProperty("data", newData);
}

// --------------------------------------------------------------------------
vtkDataObject * voDataObject::toVTKDataObject(voDataObject* dataObject)
{
  if (!dataObject)
    {
    return 0;
    }
  if (!dataObject->data().canConvert<vtkVariant>()
      || !dataObject->data().value<vtkVariant>().IsVTKObject())
    {
    qWarning() << "voDataObject::dataAsVTKDataObject() failed - Stored data type is"
               << dataObject->data().typeName();
    return 0;
    }
  vtkObjectBase * objectBase = dataObject->data().value<vtkVariant>().ToVTKObject();
  if (!vtkDataObject::SafeDownCast(objectBase))
    {
    qWarning() << "voDataObject::dataAsVTKDataObject() failed - Stored VTK data type is"
               << objectBase->GetClassName();
    return 0;
    }
  return vtkDataObject::SafeDownCast(objectBase);
}

// --------------------------------------------------------------------------
vtkDataObject* voDataObject::dataAsVTKDataObject()const
{
  return voDataObject::toVTKDataObject(const_cast<voDataObject*>(this));
}

// --------------------------------------------------------------------------
void voDataObject::setData(vtkDataObject * newData)
{
  if(newData)
    {
    newData->Register(0);
    }
  this->setData(QVariant::fromValue(vtkVariant(newData)));
}

// --------------------------------------------------------------------------
bool voDataObject::isVTKDataObject(voDataObject * dataObject)
{
  if (!dataObject)
    {
    return false;
    }
  if (!dataObject->data().canConvert<vtkVariant>()
      || !dataObject->data().value<vtkVariant>().IsVTKObject())
    {
    return false;
    }
  if (!vtkDataObject::SafeDownCast(dataObject->data().value<vtkVariant>().ToVTKObject()))
    {
    return false;
    }
  return true;
}

// --------------------------------------------------------------------------
bool voDataObject::isVTKDataObject()const
{
  return voDataObject::isVTKDataObject(const_cast<voDataObject*>(this));
}
